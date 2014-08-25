#include "soso/Scene.h"

using namespace std;
using namespace cinder;
using namespace soso;

//#include "ofGraphics.h"	//for viewports
//#include "ofAppGlutWindow.h"	//for viewports

//class ViewportData _________________________________________________________________________
ViewportData::ViewportData(int iScreenNum, int iScreenX, int iScreenY, int iWorldX, int iWorldY, int iWidth, int iHeight)
{
	screenNum = iScreenNum;

	screenOrigin[0] = iScreenX;
	screenOrigin[1] = iScreenY;

	worldOrigin[0] = iWorldX;
	worldOrigin[1] = iWorldY;

	dimensions[0] = iWidth;
	dimensions[1] = iHeight;
}

ViewportData::~ViewportData(){}



//class Scene _______________________________________________________________________________
float* Scene::defaultMatrix = NULL;
ObjectMaterial* Scene::defaultMaterial = NULL;

Scene::Scene(int w, int h)
{
	sceneWidth = w;
	sceneHeight = h;

	root = make_shared<Object>();

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

	centerOffset.set(sceneWidth/2.0f, sceneHeight/2.0f, 0);

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
		defaultMaterial = new ObjectMaterial();	//v4.0

	//setRenderMode(RENDER_NORMAL);
	//setRenderMode(RENDER_ALPHA_DEPTH);	//not working yet
	setRenderMode(RENDER_ALPHA_DEPTH_SORTED);

}

Scene::~Scene(){}

void Scene::update(float iTime)
{
	//This call to root's idle propagates down to all children in the tree.
	root->idleBase(iTime);

	//Updates all object's curTime, which is used for things like messages.
	Object::curTime = iTime;
}

ObjectRef Scene::getRoot()
{
	return root;
}

void Scene::setRoot(ObjectRef iRoot)
{
	root = iRoot;
}

void Scene::setCenterOffset(float iX, float iY, float iZ)
{
	centerOffset.set(iX, iY, iZ);
}

void Scene::draw()
{
	//Necessary for proper rendering of transparency.
	ci::gl::enableAlphaBlending();

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

//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	gl::pushModelView();

	//ofTranslate(centerOffset.x, centerOffset.y, 0);	//offset root

	//Depending on the render mode, choose a draw method
	if (renderMode == RENDER_NORMAL)
		drawNormal();
	else if(renderMode == RENDER_ALPHA_DEPTH)
		drawAlphaDepth();
	else if(renderMode == RENDER_ALPHA_DEPTH_SORTED)
		drawAlphaDepthSorted();

	gl::popModelView();

	if(isScissorOn)
		glDisable(GL_SCISSOR_TEST);
}

void Scene::drawNormal()
{
	root->draw(defaultMaterial, defaultMatrix);	//v4.0
}

void Scene::drawAlphaDepth()
{
	//First draw opaque objects.
	root->draw(defaultMaterial, defaultMatrix, OF_RENDER_OPAQUE);
	//Disable depth buffer writing and draw transparent objects.
	setDepthMask(false);
	root->draw(defaultMaterial, defaultMatrix, OF_RENDER_TRANSPARENT);
	//Re-enable depth buffer writing.
	setDepthMask(true);
}

void Scene::drawAlphaDepthSorted()
{
	//Draw opaque objects
	root->draw(defaultMaterial, defaultMatrix, OF_RENDER_OPAQUE);

	//Disable depth buffer writing.
	setDepthMask(false);
	//Collect transparent objects.
	int size = root->collectNodes(OF_RENDER_TRANSPARENT, sortedObjects, 0, maxSortedObjects);
	//Sort them according to their z value.
	qsort((void *)sortedObjects, size, sizeof(Object *), Scene::depthCompareFunc);

	//Render transparent objects.
	for (int i=0; i < size; i++) {
		sortedObjects[i]->draw(defaultMaterial, defaultMatrix, OF_RENDER_ALL, true);
	}

	//Re-enable depth buffer writing.
	setDepthMask(true);
}

int Scene::depthCompareFunc(const void *iElement1, const void *iElement2)
{
	Object *o1 = *((Object **)iElement1);
	Object *o2 = *((Object **)iElement2);

	float z1  = o1->sortedObjectsWindowZ;
	float z2  = o2->sortedObjectsWindowZ;

	if(z1 < z2)
		return 1;
	else if(z1 > z2)
		return -1;
	else
		return 0;
}


void Scene::enableScissor(bool iEnable)
{
	isScissorOn = iEnable;
}

void Scene::setScissorParameters(int iX, int iY, int iW, int iH)
{
	scissorOrigin[0] = iX;
	scissorOrigin[1] = iY;
	scissorDimensions[0] = iW;
	scissorDimensions[1] = iH;
}

void Scene::setRenderMode(int iMode)
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
		sortedObjects = new (Object (*[maxSortedObjects]));
	}
}

void Scene::setMaxSortedObjects(int iMax)
{
	maxSortedObjects = iMax;
	//Call this to update sortedObjects array.
	setRenderMode(renderMode);
}

void Scene::enableDepthTest(bool iEnable)
{
	isDepthTestOn = iEnable;
}

void Scene::setDepthFunc(GLenum iFunc)
{
	depthFunc = iFunc;
}

void Scene::setDepthMask(bool iWrite)
{
	isDepthMaskOn = iWrite;
	if(isDepthMaskOn){
		glDepthMask(GL_TRUE);
	}else{
		glDepthMask(GL_FALSE);
	}
}

void Scene::enableBlending(bool iEnable)
{
	isBlendingOn = iEnable;
}

void Scene::setShadingModel(GLenum iModel)
{
	shadingModel = iModel;
}

//LM 061312
void Scene::setClearBackground(bool iClear) {
	isClearBackgroundOn = iClear;
}

//0-255
void Scene::setBackgroundColor(float iR, float iG, float iB, float iA)
{
	backgroundColor.set(iR, iG, iB, iA);
}

//LM 061312
ci::Vec4f Scene::getBackgroundColor() {
	return backgroundColor;
}

int Scene::getSceneWidth()
{
	return sceneWidth;
}

int Scene::getSceneHeight()
{
	return sceneHeight;
}

