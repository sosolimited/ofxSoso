#include "ofxScene.h"
#include "ofAppRunner.h"
//#include "ofGraphics.h"	//for viewports
//#include "ofAppGlutWindow.h"	//for viewports

//class ofxViewportData _________________________________________________________________________
ofxViewportData::ofxViewportData(int iScreenNum, int iScreenX, int iScreenY, int iWorldX, int iWorldY, int iWidth, int iHeight)
{
	screenNum = iScreenNum;
  
	screenOrigin[0] = iScreenX;
	screenOrigin[1] = iScreenY;
  
	worldOrigin[0] = iWorldX;
	worldOrigin[1] = iWorldY;
  
	dimensions[0] = iWidth;
	dimensions[1] = iHeight;
}

ofxViewportData::~ofxViewportData(){}


//class ofxScene _______________________________________________________________________________
float* ofxScene::defaultMatrix = NULL;
ofxObjectMaterial* ofxScene::defaultMaterial = NULL;

ofxScene::ofxScene(int w, int h)
{
	sceneWidth = w;
	sceneHeight = h;
	
	root = new ofxObject();
	
	isClearBackgroundOn = true;
	backgroundColor.set(255,255,255,255);
  
	isScissorOn = false;
	scissorOrigin[0] = 0;
	scissorOrigin[1] = 0;
	scissorDimensions[0] = sceneWidth;
	scissorDimensions[1] = sceneHeight;
  
	enableBlending(true);
	enableDepthTest(true);
	setDepthFunc(GL_LESS);
	setDepthMask(true);
	setShadingModel(GL_SMOOTH);
  
	sortedObjects = NULL;
	//default max number of sorted objects
	maxSortedObjects = 10000;
  
  onTopObjects = NULL;
	maxOnTopObjects = 100;
  
	centerOffset.set(sceneWidth/2.0f, sceneHeight/2.0f, 0);
  
  
  setupScreenEnabled = true;
  
  // Build renderer with width, height, and default parameters.
  renderer = new ofxSosoRenderer(sceneWidth, sceneHeight);
  
	if(!defaultMatrix){
		defaultMatrix = (float *)malloc(sizeof(float) * 16);
		defaultMatrix[0] = 1.0;
		defaultMatrix[1] = 0;
		defaultMatrix[2] = 0;
		defaultMatrix[3] = 0;
    
		defaultMatrix[4] = 0;
		defaultMatrix[5] = 1.0;
		defaultMatrix[6] = 0;
		defaultMatrix[7] = 0;
    
		defaultMatrix[8] = 0;
		defaultMatrix[9] = 0;
		defaultMatrix[10] = 1.0;
		defaultMatrix[11] = 0;
    
		defaultMatrix[12] = 0;
		defaultMatrix[13] = 0;
		defaultMatrix[14] = 0;
		defaultMatrix[15] = 1.0;
	}
	
	if(!defaultMaterial)
		defaultMaterial = new ofxObjectMaterial();
  
  // Testing - using model view matrix as default matrix.
  //defaultMatrix = renderer->getModelViewMatrix().getPtr();
  ofxObject::Mul(defaultMatrix, renderer->getModelViewMatrix().getPtr(), defaultMatrix);
  
	//setRenderMode(RENDER_NORMAL);
	//setRenderMode(RENDER_ALPHA_DEPTH);	// Not working yet.
	setRenderMode(RENDER_ALPHA_DEPTH_SORTED);
  
  
}

// Destructor.
ofxScene::~ofxScene(){
  delete root;
  //  delete defaultMaterial; //DEV: can't delete this or else my test crashes...
  delete sortedObjects;     //DEV: hopefully we can delete a double pointer like a normal pointer, like this.
  // how can i clear() this vector?
  delete onTopObjects;      //DEV: hopefully we can delete a double pointer like a normal pointer, like this.
  // how can i clear() this vector?
}

void ofxScene::update(float iTime)
{
	//This call to root's idle propagates down to all children in the tree.
	root->idleBase(iTime);
  
	//Updates all object's curTime, which is used for things like messages.
	ofxObject::curTime = iTime;
}

ofxObject* ofxScene::getRoot()
{
	return root;
}

void ofxScene::setRoot(ofxObject *iRoot)
{
	root = iRoot;
}

void ofxScene::setCenterOffset(float iX, float iY, float iZ)
{
	centerOffset.set(iX, iY, iZ);
}

void ofxScene::draw()
{
  
  // Call setupScreen on the custom renderer here.
  // This lets each ofxScene have its own custom projection and modelview setup.
  if(setupScreenEnabled){
    renderer->setupScreen();
  }
  
	//Necessary for proper rendering of transparency.
	ofEnableAlphaBlending();
  
	//Scissoring
	if(isScissorOn){
		glEnable(GL_SCISSOR_TEST);
		glScissor(scissorOrigin[0], scissorOrigin[1], scissorDimensions[0], scissorDimensions[1]);
	}else{
		glDisable(GL_SCISSOR_TEST);
	}
	//Depth test
	if(isDepthTestOn){
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(depthFunc);
	}else{
		glDisable(GL_DEPTH_TEST);
	}
	//Blending
	if(isBlendingOn){
		glEnable(GL_BLEND);
	}else{
		glDisable(GL_BLEND);
	}
	//Shading
	glShadeModel(shadingModel);
  
	//Clear background
	glClearColor(backgroundColor[0]/255.0f, backgroundColor[1]/255.0f, backgroundColor[2]/255.0f, backgroundColor[3]/255.0f);
	if(isClearBackgroundOn){
		if(isDepthTestOn)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_COLOR_BUFFER_BIT);
	}else{
		if(isDepthTestOn)
			glClear(GL_DEPTH_BUFFER_BIT);
	}
  
  // All subsequent glLoadMatrix calls by ofxObjects will be operating on the MODELVIEW matrix.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  
	
	//ofTranslate(centerOffset.x, centerOffset.y, 0);	//offset root
	
	//Depending on the render mode, choose a draw method
	if (renderMode == RENDER_NORMAL)
		drawNormal();
	else if(renderMode == RENDER_ALPHA_DEPTH)
		drawAlphaDepth();
	else if(renderMode == RENDER_ALPHA_DEPTH_SORTED)
		drawAlphaDepthSorted();
  
  // Handle on top drawing objects.
  if (onTopDrawingEnabled) {
    // Grab objects that have mDrawOnTop set to true.
    int size = root->collectNodes(OF_RENDER_ONTOP, onTopObjects, 0, maxOnTopObjects);
    
    glDisable(GL_DEPTH_TEST);
    
    // Draw them.
    for (int i=0; i < size; i++){
      ofxObject *obj = onTopObjects[i];
      obj->draw(defaultMaterial, defaultMatrix, OF_RENDER_ALL, true);
    }
    // Reset depth test if it's supposed to be true.
    if (isDepthTestOn){
      glEnable(GL_DEPTH_TEST);
    }
  }
  
	if(isScissorOn)
		glDisable(GL_SCISSOR_TEST);
}

void ofxScene::drawNormal()
{
	root->draw(defaultMaterial, defaultMatrix);	//v4.0
}

void ofxScene::drawAlphaDepth()
{
	//First draw opaque objects.
	root->draw(defaultMaterial, defaultMatrix, OF_RENDER_OPAQUE);
	//Disable depth buffer writing and draw transparent objects.
	setDepthMask(false);
	root->draw(defaultMaterial, defaultMatrix, OF_RENDER_TRANSPARENT);
	//Re-enable depth buffer writing.
	setDepthMask(true);
}

void ofxScene::drawAlphaDepthSorted()
{
	//Draw opaque objects
	root->draw(defaultMaterial, defaultMatrix, OF_RENDER_OPAQUE);
	
	//Disable depth buffer writing.
	setDepthMask(false);
	//Collect transparent objects.
	int size = root->collectNodes(OF_RENDER_TRANSPARENT, sortedObjects, 0, maxSortedObjects);
	//Sort them according to their z value.
	qsort((void *)sortedObjects, size, sizeof(ofxObject *), ofxScene::depthCompareFunc);
  
	//Render transparent objects.
	for (int i=0; i < size; i++) {
		sortedObjects[i]->draw(defaultMaterial, defaultMatrix, OF_RENDER_ALL, true);
	}
  
	//Re-enable depth buffer writing.
	setDepthMask(true);
}

int ofxScene::depthCompareFunc(const void *iElement1, const void *iElement2)
{
	ofxObject *o1 = *((ofxObject **)iElement1);
	ofxObject *o2 = *((ofxObject **)iElement2);
  
	float z1  = o1->sortedObjectsWindowZ;
	float z2  = o2->sortedObjectsWindowZ;
  
	if(z1 < z2)
		return 1;
	else if(z1 > z2)
		return -1;
	else
		return 0;
}


void ofxScene::enableScissor(bool iEnable)
{
	isScissorOn = iEnable;
}

void ofxScene::setScissorParameters(int iX, int iY, int iW, int iH)
{
	scissorOrigin[0] = iX;
	scissorOrigin[1] = iY;
	scissorDimensions[0] = iW;
	scissorDimensions[1] = iH;
}

void ofxScene::setRenderMode(int iMode)
{
	renderMode = iMode;
  
	if (sortedObjects) {
		delete sortedObjects;
		sortedObjects = NULL;
	}
  
	if(renderMode == RENDER_NORMAL){
		enableDepthTest(true);
	}else if(renderMode == RENDER_ALPHA_DEPTH){
		enableDepthTest(true);
	}else if(renderMode == RENDER_ALPHA_DEPTH_SORTED){
		enableDepthTest(true);
		sortedObjects = new (ofxObject (*[maxSortedObjects]));
	}
}

// Enable or disable calling setupScreen on custom renderer.
void ofxScene::enableSetupScreen(bool iEnable)
{
  setupScreenEnabled = iEnable;
}

// Let's the owner of the scene customize the projection and model view matrices.
void ofxScene::setScreenParams(bool iOrthographic, bool iVFlip, float iFov, float iNearDist, float iFarDist)
{
  renderer->setScreenParams(iOrthographic, iVFlip, iFov, iNearDist, iFarDist);
}




void ofxScene::setMaxSortedObjects(int iMax)
{
	maxSortedObjects = iMax;
	//Call this to update sortedObjects array.
	setRenderMode(renderMode);
}

void ofxScene::setMaxOnTopObjects(int iMax)
{
	maxOnTopObjects = iMax;
  
	enableOnTopDrawing(onTopDrawingEnabled);
}


void ofxScene::enableOnTopDrawing(bool iEnable)
{
	// Delete the old array. Both when disabling the mode, or resizing the mOntopObjects array.
	if (onTopObjects != NULL) {
		delete onTopObjects;
		onTopObjects = NULL;
	}
  
	onTopDrawingEnabled = iEnable;
  
	// Since we deleted the array above, we will always allocate the new array, if the mode is on.
	if (onTopDrawingEnabled) {
		onTopObjects = new (ofxObject (*[maxOnTopObjects]));
	}
}

void ofxScene::enableDepthTest(bool iEnable)
{
	isDepthTestOn = iEnable;
}


void ofxScene::setDepthFunc(GLenum iFunc)
{
	depthFunc = iFunc;
}

void ofxScene::setDepthMask(bool iWrite)
{
	isDepthMaskOn = iWrite;
	if(isDepthMaskOn){
		glDepthMask(GL_TRUE);
	}else{
		glDepthMask(GL_FALSE);
	}
}

void ofxScene::enableBlending(bool iEnable)
{
	isBlendingOn = iEnable;
}

void ofxScene::setShadingModel(GLenum iModel)
{
	shadingModel = iModel;
}

//LM 061312
void ofxScene::setClearBackground(bool iClear) {
	isClearBackgroundOn = iClear;
}

//0-255
void ofxScene::setBackgroundColor(float iR, float iG, float iB, float iA)
{
	backgroundColor.set(iR, iG, iB, iA);
}

//LM 061312
ofVec4f ofxScene::getBackgroundColor() {
	return backgroundColor;
}

int ofxScene::getSceneWidth()
{
	return sceneWidth;
}

int ofxScene::getSceneHeight()
{
	return sceneHeight;
}



