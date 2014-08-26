#include "ofxObject.h"
#include "ofUtils.h"
#include "ofAppRunner.h"	//for ofGetLastFrameTime()


//class ofxObjectMaterial _____________________________________________________________________________

ofxObjectMaterial::ofxObjectMaterial()
{
  ofColor color(255.0f, 255.0f, 255.0f, 255.0f);
	inheritAlphaFlag = true;
}

ofxObjectMaterial::~ofxObjectMaterial(){}

// Returns ofColor as a ofVec4f
// Convenience method for ofxMessage calculations
ofVec4f ofxObjectMaterial::getColorVec4f(){
  
  return ofVec4f(color.r, color.g, color.b, color.a);
  
}


//class ofxObject _____________________________________________________________________________

int ofxObject::numObjects = 0;
bool ofxObject::alwaysMatrixDirty = false;
bool ofxObject::prevLit = true;
float ofxObject::curTime = 0;	//Updated by ofxScene

ofxObject::ofxObject(){
  
	id = numObjects++;
	
	//transformation matrix
	matrix = (float*)malloc(sizeof(float)*16);
	LoadIdentity(matrix);
	//matrixTmp = (float*)malloc(sizeof(float)*16);
	localMatrix = (float*)malloc(sizeof(float)*16);
	LoadIdentity(localMatrix);
	
	material = new ofxObjectMaterial();
	drawMaterial = new ofxObjectMaterial();
  inheritColor = false;   // SK Added color inheritence defaults to false
  
	//rotationMatrix = NULL;
	//rotationMatrixTmp = NULL;
  
	xyzRot.set(0., 0., 0.);
	xyz.set(0.,0.,0.);
	scale.set(1.,1.,1.);
	isLit = true;
	
	hasSpecialTransparency = false;	//this is used for objects with transparent textures
	renderOntop = false;
  
	shown = true;
	renderDirty = true;
	matrixDirty = true;
	localMatrixDirty = true;
	displayList = glGenLists(1);
	displayListFlag = false;
	
	isSortedObject = false;
	sortedObjectsWindowZ = 0;
	
	timePrev = ofGetElapsedTimef();	//ofGetSystemTime()/1000.0f;
	timeElapsed = 0;
  
  //shader info
  shader = new ofShader();
  shaderEnabled = true; // Enabled by default
}

// Destructor.
ofxObject::~ofxObject()
{
  
  // 1 --- Destroy new'ed items.
  delete material;
	delete drawMaterial;
  for (auto message : messages){
    delete message;
  }
  messages.clear();
  
  // 2 --- Destroy other items.
  for (auto parent : parents){
    parent->removeChild(this);
  }
  for (int i=0; i < children.size(); i++){
    //cout<<"CHILD ["<<i<<"] - - - "<<endl;
    
    for (int j=0; j < children[i]->parents.size(); j++){
      if(children[i]->parents[j] == this){
        children[i]->parents.erase(children[i]->parents.begin() + j);
        //cout<<"PARENT ["<<j<<"] - - - "<<endl;
      }
    }
  }
  
  //DEV_jc_1: this was here already, do we get rid of these unused vars?
  // Destroy malloc'ed items.
	//if (matrix != NULL) free(matrix);
	//if (matrixTmp != NULL) free(matrixTmp);
	//if (localMatrix != NULL) free(localMatrix);
	//if (rotationMatrix != NULL) free(rotationMatrix);
	//if (rotationMatrixTmp != NULL) free(rotationMatrixTmp);;
	if (matrix) free(matrix); //LM 070612
	if (localMatrix) free(localMatrix);
}

int ofxObject::addChild(ofxObject *child)
{
	//LM 071312 return if already has child
  for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			return (1);
		}
	}
	
	children.push_back(child);
	child->parents.push_back(this);
	
	// v2.33 - need to set the child object's matrix to dirty - the parent may very well have moved!
	//child->matrixDirty = true;
	
	return (1);
}

void ofxObject::removeChildSafe(ofxObject *child)
{
  children_to_remove.push_back(child);
}

void ofxObject::removeChild(ofxObject *child)
{
  for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			children.erase(children.begin() + i);
			break;
		}
	}
	
  for (unsigned int i = 0; i < child->parents.size(); i++) {
		if (child->parents[i] == this) {
			child->parents.erase(child->parents.begin() + i);
			break;
		}
	}
}

void ofxObject::updateLocalMatrix()
{
	static float cX, sX, cY, sY, cZ, sZ;	//for xyz rotation
  
	//calculate cos + sin for rotations ONCE
	cX = (float)cos(xyzRot[0] * DEG_TO_RAD);
	sX = (float)sin(xyzRot[0] * DEG_TO_RAD);
	cY = (float)cos(xyzRot[1] * DEG_TO_RAD);
	sY = (float)sin(xyzRot[1] * DEG_TO_RAD);
	cZ = (float)cos(xyzRot[2] * DEG_TO_RAD);
	sZ = (float)sin(xyzRot[2] * DEG_TO_RAD);
	
	//build composite matrix for XYZ rotation:
	//order of transformations:  scale, rotateX, rotateY, rotateZ, translate
	localMatrix[0] = scale.x * (cY*cZ);
	localMatrix[4] = scale.y * (-cY*sZ);
	localMatrix[8] = scale.z * (sY);
	
	localMatrix[1] = scale.x * (sX*sY*cZ + cX*sZ);
	localMatrix[5] = scale.y * (-sX*sY*sZ + cX*cZ);
	localMatrix[9] = scale.z * (-sX*cY);
	
	localMatrix[2] = scale.x * (-cX*sY*cZ + sX*sZ);
	localMatrix[6] = scale.y * (cX*sY*sZ + sX*cZ);
	localMatrix[10] = scale.z * (cX*cY);
	
  
	localMatrixDirty = false;
}




void ofxObject::updateMatrices(float *iParentMatrix)
{
	static float *mat = NULL;
  
	if (iParentMatrix != NULL) {
		mat = iParentMatrix;
	}
	else {
		// create root matrix
		if (mat == NULL) {
			mat = (float *)malloc(sizeof(float) * 16);
			LoadIdentity(mat);
		}
	}
	
	float *matrix2 = updateMatrix(mat);
  
	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->updateMatrices(matrix2);
	}
}


float* ofxObject::updateMatrix(float *iParentMatrix)
{
	// if the object has multiple parents, the hierarchy tree matrix needs to be set to dirty, using mMatrixDirty.
	if (parents.size() > 1) matrixDirty = true;
  
	if (matrixDirty  ||  localMatrixDirty  || alwaysMatrixDirty) {
		if (localMatrixDirty) {
			updateLocalMatrix();
		}
    
		//matrix multiplication
		Mul(localMatrix, iParentMatrix, matrix);
    
		/*
     if(id==1)
     printf("matrix:\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n",
     matrix[0], matrix[1], matrix[2], matrix[3],
     matrix[4], matrix[5], matrix[6], matrix[7],
     matrix[8], matrix[9], matrix[10], matrix[11],
     matrix[12], matrix[13], matrix[14], matrix[15]);
     */
    
		// set matrix dirty flags of all children
		for (unsigned int i = 0; i < children.size(); i++)
			children[i]->matrixDirty = true;
    
		//if(id == 1) printf("updateMatrix() %f\n", ofGetElapsedTimef());
    
		//reset matrix dirty flag
		matrixDirty = false;
	}
  
	return matrix;
}


//v4.0 for now this just does automatic alpha inheritance
//as we need more fine control over material inheritance, we can add it
ofxObjectMaterial* ofxObject::updateMaterial(ofxObjectMaterial *iMat)
{
	//firebrand - added support for disabling alpha inheritance
  float alpha = material->color.a;
  float r = material->color.r;
  float g = material->color.g;
  float b = material->color.b;
  
	if(material->inheritAlphaFlag) alpha *= (float)iMat->color.a / 255.0f;
  if(inheritColor) {  // SK added color inheritence flag
    r *= (float)iMat->color.r / 255.0f;
    g *= (float)iMat->color.g / 255.0f;
    b *= (float)iMat->color.b / 255.0f;
  }
  
  drawMaterial->color.set(r,g,b,alpha);
	
	return drawMaterial;
}

//firebrand
void ofxObject::enableAlphaInheritance(bool iEnable)
{
	material->inheritAlphaFlag = iEnable;
}


void ofxObject::idleBase(float iTime)
{
	//timeElapsed = ofGetElapsedTimef() - timePrev; //ofGetSystemTime()/1000.0f - timePrev;
	//timePrev = ofGetElapsedTimef();	//ofGetSystemTime()/1000.0f;
  
	//timeElapsed = ofGetLastFrameTime();	//OF7
  //Calculate this locally so it's always based on the idle call times    //eg
  timeElapsed = iTime - timePrev;
  
	updateMessages();
	//call virtual
	idle(iTime);
  
	//call idle on all children
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->idleBase(iTime);
  
  // remove all marked children
  for( ofxObject *child : children_to_remove ){
    removeChild( child );
  }
  children_to_remove.clear();
  
  timePrev = iTime;   //eg
}

//----------------------------------------------------------
//void ofxObject::draw(float *_matrix){
void ofxObject::draw(ofxObjectMaterial *iMaterial, float *iMatrix, int iSelect, bool iDrawAlone)
{
	//if(id == 1) printf("i am a circle %f - %f, %f, %f\n", ofGetElapsedTimef(), color.r, color.g, color.b);
  
	//call idle whether or not the object is shown
	//PEND: this might have to move down, so it doesn't get called multiple times
	//idle(ofGetElapsedTimef());
  
  
	if(shown) {
		
		//printf("ofxObject::draw()\n");
		if(!iDrawAlone){
			float *mat = updateMatrix(iMatrix);
			ofxObjectMaterial *m = updateMaterial(iMaterial);	//v4.0
      
			predraw();
      
			if ((iSelect == OF_RENDER_TRANSPARENT) && !hasTransparency()) {
				//Don't render — Transparent render pass, but this object is opaque
			}else if ((iSelect == OF_RENDER_OPAQUE) && hasTransparency()) {
				//Don't render — Opaque render pass, but this object is transparent
			}else if ((iSelect != OF_RENDER_ONTOP) && renderOntop) {
				//Don't render — Regular pass, but this is an on top object
			}else {
				//Render!
				render();
				//for (unsigned int i = 0; i < children.size(); i++)
        //children[i]->draw(m, mat, iSelect);
			}
			//v4.0 - to get alpha inheritance working
			for (unsigned int i = 0; i < children.size(); i++)
				children[i]->draw(m, mat, iSelect);
      
			postdraw();
		}
		else{
			//iDrawAlone is true — just draw this object (no children)
			//PEND idle of children won't get called for these objects! live with it or fix it
			//v4.0 (moving children draw loop above might have fixed it)
			predraw();
			render();
			postdraw();
		}
	}
	
}





void ofxObject::predraw()
{
  //float r = ofRandom(1);
  //shader->setUniform1f("time", ofGetElapsedTimef());
  
  //shaderParams = (void*) shader->setUniform1f("percentX", r);
  
  
  // Start shader if there is one
  if (shaderEnabled){
    if (shader!=NULL){
      if(shader->isLoaded()){
      // Bind the shader
        
      shader->begin();
        
      setShaderParams();
        
       
      }
  }
  }else{

  }
  
  
	glPushName(id);
	
	ofSetColor(drawMaterial->color.r, drawMaterial->color.g, drawMaterial->color.b, drawMaterial->color.a);	//v4.0
	
	//update lighting
	if (isLit != prevLit) {
		if (isLit) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
		prevLit = isLit;
	}
	
	glLoadMatrixf(matrix);
  
	/*
   //Older way of doing transformations.
   
   ofPushMatrix();
   
   ofTranslate(xyz.x, xyz.y, xyz.z);
   //ofScale(scale.x, scale.y, scale.z);
   ofRotateX(xyzRot.x);
   ofRotateY(xyzRot.y);
   ofRotateZ(xyzRot.z);
   ofScale(scale.x, scale.y, scale.z);
   */
  
	//if(id==1)
	//printf("matrix:\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n%f\t%f\t%f\t%f\n",
	//		matrix[0], matrix[1], matrix[2], matrix[3],
	//		matrix[4], matrix[5], matrix[6], matrix[7],
	//		matrix[8], matrix[9], matrix[10], matrix[11],
	//		matrix[12], matrix[13], matrix[14], matrix[15]);
  
	
}

void ofxObject::render()
{
  if(displayListFlag)
    glCallList(displayList);
}

void ofxObject::postdraw()
{
	//ofPopMatrix();
	glPopName();
  
  // End shader if there is one
  if (shaderEnabled){
    if (shader!=NULL){
      if (shader->isLoaded()){
      // End the shader
      shader->end();
      
      }
    }
  }
}

int ofxObject::collectNodes(int iSelect, ofxObject *iNodes[], int iNumber, int iMax)
{
	int curNode = iNumber;
  
	if (iNumber >= iMax) {
		printf("ofxObject::collectNodes() cannot render more than %d objects.\n", iMax);
		return curNode;
	}
  
	// v2.25 - default - we are not sorted.
	sortedObjectsWindowZ = 0;
	isSortedObject = false;
  
	if (shown) {
		if ((iSelect == OF_RENDER_TRANSPARENT) && !hasTransparency()) {
			// Skip it — looking for transparent objects, but this one is opaque
		}
		else if	((iSelect == OF_RENDER_OPAQUE) && hasTransparency()) {
			// Skip it — looking for opaque objects, but this one is transparent
		}
		else if ((iSelect == OF_RENDER_ONTOP) && (!renderOntop)) {
			// Skip it - looking for on-top objects, but this is regular
		}
		else {
			// this is an object we want to add to the list
			iNodes[iNumber] = (ofxObject *)this;
			curNode++;
      
			if (!renderOntop) isSortedObject = true;
		}
    
		if (isSortedObject) sortedObjectsWindowZ = getWindowCoords().z;
		//continue down the tree
		for (unsigned int i = 0; i < children.size(); i++) {
			curNode = children[i]->collectNodes(iSelect, iNodes, curNode, iMax);
		}
	}
  
	return curNode;
}


ofVec3f ofxObject::getWindowCoords()
{
	double		mM[16];
	double		wx, wy, wz;
	GLint		v[4];
	double		pM[16];
  
	// this gets set once up top
	glGetIntegerv(GL_VIEWPORT, v);
	// this gets set once up top
	glGetDoublev(GL_PROJECTION_MATRIX, pM);
  
	// grab the most recent version of the MODELVIEW matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, mM);
	float *curMat = getMatrix();
	for (int i=0; i < 16; i++) mM[i] = (double)curMat[i];
	
	gluProject(0, 0, 0, mM, pM, v, &wx, &wy, &wz);
	
	return ofVec3f(wx, wy, wz);
}

float* ofxObject::getMatrix()
{
	return (float *)matrix;
}

float* ofxObject::getLocalMatrix()
{
	return (float *)localMatrix;
  
}



void ofxObject::setLighting(bool iOnOff)
{
	isLit = iOnOff;
}

// Set color, keep original alpha
void ofxObject::setColor(float r, float g, float b)
{
	material->color.set(r, g, b, material->color.a);			// AO: Keep original alpha
}

// set color with an HTML-style hex code string, keep original alpha
void ofxObject::setColor(string iHex)
{
  if(iHex[0] == '#'){
    iHex = iHex.substr(1,iHex.length());
  }
  
  ofColor c = ofColor::fromHex( strtol(iHex.c_str(),NULL,16) );
  material->color.set(c.r, c.g, c.b, material->color.a);
}

// Set color and alpha
void ofxObject::setColor(float r, float g, float b, float a)
{
	material->color.set(r, g, b, a);
}

void ofxObject::setColor(ofColor c)
{
  material->color.set(c.r, c.g, c.b, c.a);
}

// Convenience methods for getting and setting individual color components.
void ofxObject::setRed(float iRed)
{
  material->color.r = iRed;
}

void ofxObject::setGreen(float iGreen)
{
  material->color.g = iGreen;
}

void ofxObject::setBlue(float iBlue)
{
  material->color.b = iBlue;
}

float ofxObject::getRed()
{
  return material->color.r;
}

float ofxObject::getGreen()
{
  return material->color.g;
}

float ofxObject::getBlue()
{
  return material->color.b;
}

void ofxObject::setAlpha(float iA)
{
	material->color.set(material->color.r, material->color.g, material->color.b, iA);	//v4.0
}

// AO: Providing a way to access using ofVec4f
// in case we still need support
ofVec4f ofxObject::getColorVec4f()
{
	return ofVec4f(material->color.r, material->color.g, material->color.b, material->color.a);	//v4.0
}

ofColor ofxObject::getColor()
{
	return material->color;	//v4.0
}

float ofxObject::getAlpha()
{
	return material->color.a;	//v4.0
}

bool ofxObject::hasTransparency()
{
	//if(hasSpecialTransparency || (color[3] < 255.0))
	if(hasSpecialTransparency || (drawMaterial->color[3] < 255.0))	//v4.0
		return true;
	else
		return false;
}

void ofxObject::setSpecialTransparency(bool iFlag)
{
	hasSpecialTransparency = iFlag;
}

void ofxObject::setRot(float x, float y, float z)
{
	xyzRot.set(x, y, z);
	localMatrixDirty = true;
}

void ofxObject::setColor(ofVec4f c)
{
	material->color.set(c.x, c.y, c.z, c.w);	 //v4.0
}



void ofxObject::setRot(ofVec3f r)
{
	xyzRot = r;
	localMatrixDirty = true;
}


ofVec3f ofxObject::getRot()
{
	return xyzRot;
}

ofVec3f ofxObject::getTrans()
{
	return xyz;
}


void ofxObject::setTrans(float x, float y, float z)
{
	xyz.set(x, y, z);
  
	localMatrix[12] = xyz[0];
	localMatrix[13] = xyz[1];
	localMatrix[14] = xyz[2];
	matrixDirty = true;
}


void ofxObject::setTrans(ofVec3f vec)
{
	xyz = vec;
  
	localMatrix[12] = xyz[0];
	localMatrix[13] = xyz[1];
	localMatrix[14] = xyz[2];
	matrixDirty = true;
}

// Convenience methods for setting and getting single axis of translation.
float ofxObject::getX()
{
  return xyz.x;
}

float ofxObject::getY()
{
  return xyz.y;
}

float ofxObject::getZ()
{
  return xyz.z;
}

void ofxObject::setX(float iX)
{
  xyz[0] = iX;
  localMatrix[12] = xyz[0];
  matrixDirty = true;
}

void ofxObject::setY(float iY)
{
  xyz[1] = iY;
  localMatrix[13] = xyz[1];
  matrixDirty = true;
}

void ofxObject::setZ(float iZ)
{
  xyz[2] = iZ;
  localMatrix[14] = xyz[2];
  matrixDirty = true;
}


ofVec3f ofxObject::getScale()
{
	return (scale);
}


void ofxObject::setScale(float s)
{
	scale.set(s,s,s);
	localMatrixDirty = true;
}


void ofxObject::setScale(float x, float y, float z)
{
	scale.set(x,y,z);
	localMatrixDirty = true;
}


void ofxObject::setScale(ofVec3f vec)
{
	scale = vec;
	localMatrixDirty = true;
}

void ofxObject::hide()
{
	shown = false;
}


void ofxObject::show()
{
	shown = true;
}


bool ofxObject::isShown()
{
	return shown;
}

int ofxObject::getID()
{
	return id;
}

// Set's the object's shader
void ofxObject::setShader(ofShader *iShader){
  
  shader = iShader;
  
}

// Load shader from a file.  Assumes frag and vertex shaders have the same filename.
void ofxObject::loadShader(string iShaderName){
  
  if (shader == NULL){
    shader = new ofShader();
  }else{
    delete shader;
    shader = new ofShader();
  }
  shader->load(iShaderName);
  
}

// Loads shader from file.  You can individually specify
// the name of frag, vert, and geo shader.
void ofxObject::loadShader(string iFragName, string iVertName){
  
  if (shader == NULL){
    shader = new ofShader();
  }else{
    delete shader;
    shader = new ofShader();
  }
  shader->load(iFragName, iVertName);
  
}

// Enable shaders.  By default, true.
void ofxObject::setEnableShaders(bool iSet){

  shaderEnabled = iSet;

}

void ofxObject::setShaderParams(){

  //do nothing by default
}

void ofxObject::Mul(float *source1, float *source2, float *_dest)
{
	float dest[16];
	
	dest[0] = source1[0]*source2[0] + source1[1]*source2[4] + source1[2]*source2[8] + source1[3]*source1[12];
	dest[1] = source1[0]*source2[1] + source1[1]*source2[5] + source1[2]*source2[9] + source1[3]*source2[13];
	dest[2] = source1[0]*source2[2] + source1[1]*source2[6] + source1[2]*source2[10] + source1[3]*source2[14];
	dest[3] = source1[0]*source2[3] + source1[1]*source2[7] + source1[2]*source2[11] + source1[3]*source2[15];
	
	dest[4] = source1[4]*source2[0] + source1[5]*source2[4] + source1[6]*source2[8] + source1[7]*source2[12];
	dest[5] = source1[4]*source2[1] + source1[5]*source2[5] + source1[6]*source2[9] + source1[7]*source2[13];
	dest[6] = source1[4]*source2[2] + source1[5]*source2[6] + source1[6]*source2[10] + source1[7]*source2[14];
	dest[7] = source1[4]*source2[3] + source1[5]*source2[7] + source1[6]*source2[11] + source1[7]*source2[15];
	
	dest[8] = source1[8]*source2[0] + source1[9]*source2[4] + source1[10]*source2[8] + source1[11]*source2[12];
	dest[9] = source1[8]*source2[1] + source1[9]*source2[5] + source1[10]*source2[9] + source1[11]*source2[13];
	dest[10] = source1[8]*source2[2] + source1[9]*source2[6] + source1[10]*source2[10] + source1[11]*source2[14];
	dest[11] = source1[8]*source2[3] + source1[9]*source2[7] + source1[10]*source2[11] + source1[11]*source2[15];
	
	dest[12] = source1[12]*source2[0] + source1[13]*source2[4] + source1[14]*source2[8] + source1[15]*source2[12];
	dest[13] = source1[12]*source2[1] + source1[13]*source2[5] + source1[14]*source2[9] + source1[15]*source2[13];
	dest[14] = source1[12]*source2[2] + source1[13]*source2[6] + source1[14]*source2[10] + source1[15]*source2[14];
	dest[15] = source1[12]*source2[3] + source1[13]*source2[7] + source1[14]*source2[11] + source1[15]*source2[15];
	
	for (int i = 0; i < 16; i++)
		_dest[i] = dest[i];
}


void ofxObject::Transpose(float *source, float *dest)
{
	dest[0] = source[0];
	dest[1] = source[4];
	dest[2] = source[8];
	dest[3] = source[12];
	
	dest[4] = source[1];
	dest[5] = source[5];
	dest[6] = source[9];
	dest[7] = source[13];
	
	dest[8] = source[2];
	dest[9] = source[6];
	dest[10] = source[10];
	dest[11] = source[14];
	
	dest[12] = source[3];
	dest[13] = source[7];
	dest[14] = source[11];
	dest[15] = source[15];
	
}

void ofxObject::LoadIdentity(float *dest)
{
	dest[0] = 1;
	dest[1] = 0;
	dest[2] = 0;
	dest[3] = 0;
	
	dest[4] = 0;
	dest[5] = 1;
	dest[6] = 0;
	dest[7] = 0;
	
	dest[8] = 0;
	dest[9] = 0;
	dest[10] = 1;
	dest[11] = 0;
	
	dest[12] = 0;
	dest[13] = 0;
	dest[14] = 0;
	dest[15] = 1;
}



void ofxObject::updateMessages()
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i]->isEnabled){
      
			//grab commonly needed values
			float startTime = messages[i]->startTime + messages[i]->startDelay;
			float endTime = messages[i]->getFinishTime();
			float duration = messages[i]->duration;
			float time = ofxMessage::interpolateTime(messages[i]->interpolation, (curTime - startTime)/duration);
			float t = time;
			float x,y,z,w;
			//printf("t_%02d = %f\n", i, t);
      
			//adjust t for palindrome looping
			if(messages[i]->playMode == OF_LOOP_PALINDROME_PLAY){
				if(messages[i]->loopDirection) t = 1.0 - time;
			}
      
			//translation__________________________________________________________
			if(messages[i]->id == OF_TRANSLATE){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						ofVec3f *vec = (ofVec3f *)messages[i]->baseStartVals;
						if(vec){
							if(vec->x == OF_RELATIVE_VAL) x = xyz.x;
							else x = vec->x;
							if(vec->y == OF_RELATIVE_VAL) y = xyz.y;
							else y = vec->y;
							if(vec->z == OF_RELATIVE_VAL) z = xyz.z;
							else z = vec->z;
              
							messages[i]->setStartVals(x,y,z);
							//printf("message start vals = %f, %f, %f\n", x,y,z);
						}
            
						//set end values once
						ofVec3f *vecEnd = (ofVec3f *)messages[i]->baseEndVals;
						if(vecEnd){
							if(vecEnd->x == OF_RELATIVE_VAL) x = xyz.x;
							else x = vecEnd->x;
							if(vecEnd->y == OF_RELATIVE_VAL) y = xyz.y;
							else y = vecEnd->y;
							if(vecEnd->z == OF_RELATIVE_VAL) z = xyz.z;
							else z = vecEnd->z;
              
							messages[i]->setEndVals(x,y,z);
							//printf("message end vals = %f, %f, %f\n", x,y,z);
						}
            
						/*
             printf("translate start Vals = %f, %f, %f\n", x,y,z);
             printf("translate end Vals = %f, %f, %f\n",
             ((ofVec3f *)messages[i]->vals)->x,
             ((ofVec3f *)messages[i]->vals)->y,
             ((ofVec3f *)messages[i]->vals)->z);
             */
            
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setTrans((1-t)*((ofVec3f *)messages[i]->startVals)->x + t*((ofVec3f *)messages[i]->endVals)->x,
                     (1-t)*((ofVec3f *)messages[i]->startVals)->y + t*((ofVec3f *)messages[i]->endVals)->y,
                     (1-t)*((ofVec3f *)messages[i]->startVals)->z + t*((ofVec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f trans = ofxMessage::bezier(t, messages[i]->pathPoints);
						setTrans(trans.x, trans.y, trans.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f trans = ofxMessage::spline(t, messages[i]->pathPoints);
						setTrans(trans.x, trans.y, trans.z);
					}
          
				}
			}
			//rotation_____________________________________________________________
			else if(messages[i]->id == OF_ROTATE){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						ofVec3f *vec = (ofVec3f *)messages[i]->baseStartVals;
						if(vec){
							if(vec->x == OF_RELATIVE_VAL) x = xyzRot.x;
							else x = vec->x;
							if(vec->y == OF_RELATIVE_VAL) y = xyzRot.y;
							else y = vec->y;
							if(vec->z == OF_RELATIVE_VAL) z = xyzRot.z;
							else z = vec->z;
              
							messages[i]->setStartVals(x, y, z);
						}
            
						//set end values once
						ofVec3f *vecEnd = (ofVec3f *)messages[i]->baseEndVals;
						if(vecEnd){
							if(vecEnd->x == OF_RELATIVE_VAL) x = xyzRot.x;
							else x = vecEnd->x;
							if(vecEnd->y == OF_RELATIVE_VAL) y = xyzRot.y;
							else y = vecEnd->y;
							if(vecEnd->z == OF_RELATIVE_VAL) z = xyzRot.z;
							else z = vecEnd->z;
              
							messages[i]->setEndVals(x,y,z);
						}
            
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setRot((1-t)*((ofVec3f *)messages[i]->startVals)->x + t*((ofVec3f *)messages[i]->endVals)->x,
                   (1-t)*((ofVec3f *)messages[i]->startVals)->y + t*((ofVec3f *)messages[i]->endVals)->y,
                   (1-t)*((ofVec3f *)messages[i]->startVals)->z + t*((ofVec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f rot = ofxMessage::bezier(t, messages[i]->pathPoints);
						setRot(rot.x, rot.y, rot.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f rot = ofxMessage::spline(t, messages[i]->pathPoints);
						setRot(rot.x, rot.y, rot.z);
					}
				}
			}
			//scaling_______________________________________________________________
			else if(messages[i]->id == OF_SCALE){
				//printf("startTime = %f\n", startTime);
				if(curTime >= startTime){
					//printf("doing message %f\n", curTime);
					if(!messages[i]->isRunning){
						//set start values once
						float *v = (float *)messages[i]->baseStartVals;
						if(v){
							if(v[0] == OF_RELATIVE_VAL) x = scale.x;
							else x = v[0];
              
							messages[i]->setStartVals(x);
						}
            
						//set end values once
						float *vEnd = (float *)messages[i]->baseEndVals;
						if(vEnd){
							if(vEnd[0] == OF_RELATIVE_VAL) x = scale.x;
							else x = vEnd[0];
              
							messages[i]->setEndVals(x);
						}
            
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setScale((1-t)*((float *)messages[i]->startVals)[0] + t*((float *)messages[i]->endVals)[0]);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f s = ofxMessage::bezier(t, messages[i]->pathPoints);
						setScale(s.x);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f s = ofxMessage::spline(t, messages[i]->pathPoints);
						setScale(s.x);
					}
				}
			}
			//scaling in 3 dimensions differently ___________________________________
			else if(messages[i]->id == OF_SCALE3){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start avlues once
						ofVec3f *vec = (ofVec3f *)messages[i]->baseStartVals;
						if(vec){
							if(vec->x == OF_RELATIVE_VAL) x = scale.x;
							else x = vec->x;
							if(vec->y == OF_RELATIVE_VAL) y = scale.y;
							else y = vec->y;
							if(vec->z == OF_RELATIVE_VAL) z = scale.z;
							else z = vec->z;
              
							messages[i]->setStartVals(x, y, z);
						}
            
						//set end values once
						ofVec3f *vecEnd = (ofVec3f *)messages[i]->baseEndVals;
						if(vecEnd){
							if(vecEnd->x == OF_RELATIVE_VAL) x = scale.x;
							else x = vecEnd->x;
							if(vecEnd->y == OF_RELATIVE_VAL) y = scale.y;
							else y = vecEnd->y;
							if(vecEnd->z == OF_RELATIVE_VAL) z = scale.z;
							else z = vecEnd->z;
              
							messages[i]->setEndVals(x,y,z);
						}
            
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setScale((1-t)*((ofVec3f *)messages[i]->startVals)->x + t*((ofVec3f *)messages[i]->endVals)->x,
                     (1-t)*((ofVec3f *)messages[i]->startVals)->y + t*((ofVec3f *)messages[i]->endVals)->y,
                     (1-t)*((ofVec3f *)messages[i]->startVals)->z + t*((ofVec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f s = ofxMessage::bezier(t, messages[i]->pathPoints);
						setScale(s.x, s.y, s.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f s = ofxMessage::spline(t, messages[i]->pathPoints);
						setScale(s.x, s.y, s.z);
					}
				}
			}
			//color _________________________________________________________________
			else if(messages[i]->id == OF_SETCOLOR){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						ofVec3f *vec = (ofVec3f *)messages[i]->baseStartVals;
						if(vec){
							if(vec->x == OF_RELATIVE_VAL) x = material->getColorVec4f().x;
							else x = vec->x;
							if(vec->y == OF_RELATIVE_VAL) y = material->getColorVec4f().y;
							else y = vec->y;
							if(vec->z == OF_RELATIVE_VAL) z = material->getColorVec4f().z;
							else z = vec->z;
              
							messages[i]->setStartVals(x, y, z);
						}
            
						//set end values once
						ofVec3f *vecEnd = (ofVec3f *)messages[i]->baseEndVals;
						if(vecEnd){
							if(vecEnd->x == OF_RELATIVE_VAL) x = material->getColorVec4f().x;
							else x = vecEnd->x;
							if(vecEnd->y == OF_RELATIVE_VAL) y = material->getColorVec4f().y;
							else y = vecEnd->y;
							if(vecEnd->z == OF_RELATIVE_VAL) z = material->getColorVec4f().z;
							else z = vecEnd->z;
              
							messages[i]->setEndVals(x,y,z);
						}
            
						//printf("color startvals = %f, %f , %f\n", x,y,z);
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setColor((1-t)*((ofVec3f *)messages[i]->startVals)->x + t*((ofVec3f *)messages[i]->endVals)->x,
                     (1-t)*((ofVec3f *)messages[i]->startVals)->y + t*((ofVec3f *)messages[i]->endVals)->y,
                     (1-t)*((ofVec3f *)messages[i]->startVals)->z + t*((ofVec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f c = ofxMessage::bezier(t, messages[i]->pathPoints);
						setColor(c.x, c.y, c.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f c = ofxMessage::spline(t, messages[i]->pathPoints);
						setColor(c.x, c.y, c.z);
					}
				}
			}      // color with alpha channel
      else if(messages[i]->id == OF_SETCOLOR4){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						ofVec4f *vec = (ofVec4f *)messages[i]->baseStartVals;
						if(vec){
							if(vec->x == OF_RELATIVE_VAL) x = material->getColorVec4f().x;
							else x = vec->x;
							if(vec->y == OF_RELATIVE_VAL) y = material->getColorVec4f().y;
							else y = vec->y;
							if(vec->z == OF_RELATIVE_VAL) z = material->getColorVec4f().z;
							else z = vec->z;
              if(vec->w == OF_RELATIVE_VAL) w = material->getColorVec4f().w;
							else w = vec->w;
              
							messages[i]->setStartVals(x, y, z, w);
						}
            
						//set end values once
						ofVec4f *vecEnd = (ofVec4f *)messages[i]->baseEndVals;
						if(vecEnd){
							if(vecEnd->x == OF_RELATIVE_VAL) x = material->getColorVec4f().x;
							else x = vecEnd->x;
							if(vecEnd->y == OF_RELATIVE_VAL) y = material->getColorVec4f().y;
							else y = vecEnd->y;
							if(vecEnd->z == OF_RELATIVE_VAL) z = material->getColorVec4f().z;
							else z = vecEnd->z;
              if(vecEnd->w == OF_RELATIVE_VAL) w = material->getColorVec4f().w;
							else w = vecEnd->w;
              
							messages[i]->setEndVals(x,y,z,w);
						}
            
						//printf("color startvals = %f, %f , %f\n", x,y,z);
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setColor((1-t)*((ofVec4f *)messages[i]->startVals)->x + t*((ofVec4f *)messages[i]->endVals)->x,
                     (1-t)*((ofVec4f *)messages[i]->startVals)->y + t*((ofVec4f *)messages[i]->endVals)->y,
                     (1-t)*((ofVec4f *)messages[i]->startVals)->z + t*((ofVec4f *)messages[i]->endVals)->z,
                     (1-t)*((ofVec4f *)messages[i]->startVals)->w + t*((ofVec4f *)messages[i]->endVals)->w);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f c = ofxMessage::bezier(t, messages[i]->pathPoints);
						setColor(c.x, c.y, c.z, c.w);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f c = ofxMessage::spline(t, messages[i]->pathPoints);
						setColor(c.x, c.y, c.z, c.w);
					}
				}
			}
			//alpha__________________________________________________________________
			else if(messages[i]->id == OF_SETALPHA){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						float *v = (float *)messages[i]->baseStartVals;
						if(v){
							if(v[0] == OF_RELATIVE_VAL) x = material->getColorVec4f().w;
							else x = v[0];
              
							messages[i]->setStartVals(x);
						}
            
						//set end values once
						float *vEnd = (float *)messages[i]->baseEndVals;
						if(vEnd){
							if(vEnd[0] == OF_RELATIVE_VAL) x = material->getColorVec4f().w;
							else x = vEnd[0];
              
							messages[i]->setEndVals(x);
						}
            
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setAlpha((1-t)*((float *)messages[i]->startVals)[0] + t*((float *)messages[i]->endVals)[0]);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ofVec4f s = ofxMessage::bezier(t, messages[i]->pathPoints);
						setAlpha(s.x);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ofVec4f s = ofxMessage::spline(t, messages[i]->pathPoints);
						setAlpha(s.x);
					}
				}
			}
			//show____________________________________________________________________
			else if(messages[i]->id == OF_SHOW){
				if(curTime >= startTime){
					show();
					messages[i]->isEnabled = false;
				}
			}
			//hide____________________________________________________________________
			else if(messages[i]->id == OF_HIDE){
				if(curTime >= startTime){
          hide();
					messages[i]->isEnabled = false;
				}
			}
      
			//function________________________________________________________________
			else if(messages[i]->id == OF_FUNCTION){
				if(curTime >= startTime){
          messages[i]->functionPtr(messages[i]->startVals);
					messages[i]->isEnabled = false;
				}
			}
      
      
			//cleanup messages _____________________________________________________________________________________
			//if the message is done and no longer needed, delete it
			if(time == 1.0){ //only delete it when it's reached completion
				messages[i]->isEnabled = false;
				messages[i]->isRunning = false;
        
				if(messages[i]->autoDelete){
					deleteMessage(messages[i]);
				}
        
				if(i < messages.size()){	//just in case a message just got deleted above
					//handle special looping behaviors
					if(messages[i]->playMode == OF_LOOP_PLAY){
						messages[i]->isEnabled = true;
						messages[i]->isRunning = false;
						messages[i]->setStartTime(curTime);
					}
					else if(messages[i]->playMode == OF_LOOP_PALINDROME_PLAY){
						messages[i]->loopDirection = !messages[i]->loopDirection;
						messages[i]->isEnabled = true;
						messages[i]->isRunning = false;
						messages[i]->setStartTime(curTime);
					}
				}
			}
		}
	}
}

bool ofxObject::isAnimating()
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i]->isRunning)
			return true;
	}
	return false;
}

void ofxObject::deleteMessage(ofxMessage *iMessage)
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(iMessage == messages[i]){
			delete iMessage;
			messages.erase(messages.begin() + i);
			//printf("ofxObject::deleteMessage()\n");
      
			return;
		}
	}
}

//Remove it, but don't delete it.
bool ofxObject::removeMessage(ofxMessage *iMessage)
{
	//printf("ofxObject::removeMessage()\n");
	for(unsigned int i=0; i < messages.size(); i++){
		if(iMessage == messages[i]){
			messages.erase(messages.begin() + i);
			//printf("succeeded\n");
			return true;
		}
	}
	return false;
}


void ofxObject::stopMessages(int iMessageType)
{
	bool deleteFlag = true;
  //DEV: refactor this loop not to iterate backwards?
	for(unsigned int i=0; i < messages.size(); i++){
		if(iMessageType > -1){
			if(messages[i]->id != iMessageType)
				deleteFlag = false;
		}
		if(deleteFlag){
			delete(messages[i]);
			messages.erase(messages.begin() + i);
			i--;
		}
	}
}


ofxMessage* ofxObject::doMessage0f(int iID, float iDelay, float iDuration, int iInterp)
{
	ofxMessage *message =new ofxMessage(iID, NULL, iInterp, iDuration, iDelay);
	messages.push_back(message);
  
	return message;
}

ofxMessage* ofxObject::doMessage1f(int iID, float iDelay, float iDuration, int iInterp, float iVal)
{
	float *args = new float[1];
	args[0] = iVal;
  
	ofxMessage *message = new ofxMessage(iID, (void *)args, iInterp, iDuration, iDelay);
  message->setStartTime(curTime);
	messages.push_back(message);
  
	return message;
}

ofxMessage* ofxObject::doMessage3f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2)
{
	ofVec3f *args = new ofVec3f();
	args->set(iVal0, iVal1, iVal2);
  
	ofxMessage *message = new ofxMessage(iID, (void *)args, iInterp, iDuration, iDelay);
  message->setStartTime(curTime);
	messages.push_back(message);
  
	return message;
}

ofxMessage* ofxObject::doMessage4f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2, float iVal3)
{
	ofVec4f *args = new ofVec4f();
	args->set(iVal0, iVal1, iVal2, iVal3);
  
	ofxMessage *message =new ofxMessage(iID, (void *)args, iInterp, iDuration, iDelay);
  message->setStartTime(curTime);
	messages.push_back(message);
  
	return message;
}

ofxMessage* ofxObject::doMessageNf(int iID, float iDelay, float iDuration, int iInterp, int iPath, vector<ofVec4f> iPathPoints)
{
	ofxMessage *message = new ofxMessage(iID, iInterp, iPath, iPathPoints, iDuration, iDelay);
  message->setStartTime(curTime);
	messages.push_back(message);
  
	return message;
}

//Sse a message that has already been made
//sets startTime of iMessage to current time
ofxMessage* ofxObject::doMessage(ofxMessage *iMessage)
{
	if (iMessage->id == OF_FUNCTION) {
		iMessage->autoDelete = false;
	} else {
    //iMessage->setStartTime(ofGetElapsedTimef());
    iMessage->setStartTime(curTime);
  }
	iMessage->enableMessage(true);
	if(!hasMessage(iMessage))	//only add it if it's not already there
		messages.push_back(iMessage);
	
	return iMessage;
}

//Lets you know if this message already in messages
bool ofxObject::hasMessage(ofxMessage *iMessage)
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i] == iMessage)
			return true;
	}
	return false;
}

int ofxObject::isDescendant(ofxObject *iObject)
{
	for (unsigned int i = 0; i < children.size(); i++) {
		ofxObject *obj = children[i];
		if (obj == iObject) return(1);
		else if (obj->isDescendant(iObject)) return(1);
	}
  
	return(0);
  
}

// Convenience method for selection.
// Checks if an object's id is among a vector of IDs.
bool ofxObject::isObjectID(vector<GLuint> iIDs)
{
  for(int i=0; i < iIDs.size(); i++){
    if(iIDs[i] == id)
      return true;
  }
  return false;
}

void ofxObject::setDisplayList(GLuint iList)
{
  displayList = iList;
  //Tells render to use the list.
  displayListFlag = true;
}
