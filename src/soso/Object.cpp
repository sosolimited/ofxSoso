#include "soso/Object.h"

#include "cinder/app/App.h"
#include "cinder/CinderMath.h"
#include "cinder/Camera.h"

using namespace std;
using namespace cinder;

//class ObjectMaterial _____________________________________________________________________________
ObjectMaterial::ObjectMaterial()
{
	color.set(255.0f, 255.0f, 255.0f, 255.0f);
	inheritAlphaFlag = true;
}

ObjectMaterial::~ObjectMaterial(){}







//class Object _____________________________________________________________________________

int Object::numObjects = 0;
bool Object::alwaysMatrixDirty = false;
bool Object::prevLit = true;
float Object::curTime = 0;	//Updated by Scene

//----------------------------------------------------------
Object::Object(){

	id = numObjects++;

	//transformation matrix
	matrix = Matrix44f::identity();
	//matrixTmp = (float*)malloc(sizeof(float)*16);
	localMatrix = Matrix44f::identity();

	material = new ObjectMaterial();
	drawMaterial = new ObjectMaterial();
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

	timePrev = app::getElapsedSeconds();	//ofGetSystemTime()/1000.0f;
	timeElapsed = 0;
}


//----------------------------------------------------------
Object::~Object()
{
  // Ensure that this object is removed from the render tree before deletion.
	for( auto &parent : parents ) {
			parent->removeChild( this );
	}

	// Get rid of parent references to this object in all its children.
	auto self = this;
	for( auto &child : children ) {
		child->parents.erase( remove_if( child->parents.begin(), child->parents.end(),
																		[self] (Object *p) {
																			return p == self;
																		} ), child->parents.end() );
	}

	//kill all messages
	for(unsigned int i=0; i < messages.size(); i++){
		delete(messages[i]);
		messages.erase(messages.begin() + i);
		i--;
	}

	delete(material);
	delete(drawMaterial);

	//if (matrix != NULL) free(matrix);
	//if (matrixTmp != NULL) free(matrixTmp);
	//if (localMatrix != NULL) free(localMatrix);

	//if (rotationMatrix != NULL) free(rotationMatrix);
	//if (rotationMatrixTmp != NULL) free(rotationMatrixTmp);;
}

int Object::addChild(ObjectRef child)
{
	//LM 071312 return if already has child
	for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			return (1);
		}
	}

	children.push_back(child);
	child->parents.push_back( this );

	// v2.33 - need to set the child object's matrix to dirty - the parent may very well have moved!
	//child->matrixDirty = true;

	return (1);
}

void Object::removeChildSafe(ObjectRef child)
{
    children_to_remove.push_back(child);
}

void Object::removeChild(Object *child)
{
	children.erase( remove_if( children.begin(), children.end(),
														[child] (const ObjectRef &c) {
																return c.get() == child;
														} ), children.end() );

	for (unsigned int i = 0; i < children.size(); i++) {
		if (children[i].get() == child) {
			children.erase(children.begin() + i);
			break;
		}
	}

	auto self = this;
	child->parents.erase( remove_if( child->parents.begin(), child->parents.end(),
																	[self] (Object *p) {
																		return self == p;
																	}), child->parents.end() );
}

void Object::updateLocalMatrix()
{
	static float cX, sX, cY, sY, cZ, sZ;	//for xyz rotation

	//calculate cos + sin for rotations ONCE
	cX = (float)cos( toRadians( xyzRot[0] ) );
	sX = (float)sin( toRadians( xyzRot[0] ) );
	cY = (float)cos( toRadians( xyzRot[1] ) );
	sY = (float)sin( toRadians( xyzRot[1] ) );
	cZ = (float)cos( toRadians( xyzRot[2] ) );
	sZ = (float)sin( toRadians( xyzRot[2] ) );

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




void Object::updateMatrices( const Matrix44f &iParentMatrix )
{
//	static float *mat = NULL;
	auto mat = iParentMatrix;

	auto matrix2 = updateMatrix(mat);

	for (unsigned int i = 0; i < children.size(); i++) {
		children[i]->updateMatrices(matrix2);
	}
}


const ci::Matrix44f& Object::updateMatrix( const ci::Matrix44f &iParentMatrix )
{
	// if the object has multiple parents, the hierarchy tree matrix needs to be set to dirty, using mMatrixDirty.
	if (parents.size() > 1) matrixDirty = true;

	if (matrixDirty  ||  localMatrixDirty  || alwaysMatrixDirty) {
		if (localMatrixDirty) {
			updateLocalMatrix();
		}

		//matrix multiplication
		matrix = iParentMatrix * localMatrix;

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
ObjectMaterial* Object::updateMaterial(ObjectMaterial *iMat)
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
void Object::enableAlphaInheritance(bool iEnable)
{
	material->inheritAlphaFlag = iEnable;
}


void Object::idleBase(float iTime)
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
    for( ObjectRef child : children_to_remove ){
			removeChild( child );
    }
    children_to_remove.clear();

    timePrev = iTime;   //eg
}

//----------------------------------------------------------
//void Object::draw(float *_matrix){
void Object::draw(ObjectMaterial *iMaterial, float *iMatrix, int iSelect, bool iDrawAlone)
{
	//if(id == 1) printf("i am a circle %f - %f, %f, %f\n", ofGetElapsedTimef(), color.x, color.y, color.z);

	//moved to idleBase
	//timeElapsed = ofGetElapsedTimef() - timePrev; //ofGetSystemTime()/1000.0f - timePrev;
	//timePrev = ofGetElapsedTimef();	//ofGetSystemTime()/1000.0f;

	//if(id==1) printf("system time = %f\n", ofGetSystemTime());

	//cleanupMessages();	//replaced by deleteMessage()
	//updateMessages();		//moved to idleBase

	//call idle whether or not the object is shown
	//PEND: this might have to move down, so it doesn't get called multiple times
	//idle(ofGetElapsedTimef());


	if(shown) {

		//printf("Object::draw()\n");
		if(!iDrawAlone){
			auto mat = updateMatrix(iMatrix);
			ObjectMaterial *m = updateMaterial(iMaterial);	//v4.0

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





void Object::predraw()
{
	glPushName(id);

	//ofSetColor(color.x, color.y, color.z, color.w);
	gl::color( drawMaterial->color );

	//update lighting
	if (isLit != prevLit) {
		if (isLit) glEnable(GL_LIGHTING);
		else glDisable(GL_LIGHTING);
		prevLit = isLit;
	}

	gl::pushModelView();
	gl::multModelView( matrix );

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

void Object::render()
{
    if(displayListFlag)
        glCallList(displayList);
}

void Object::postdraw()
{
	gl::popModelView();
	//ofPopMatrix();
	glPopName();
}

int Object::collectNodes(int iSelect, Object *iNodes[], int iNumber, int iMax)
{
	int curNode = iNumber;

	if (iNumber >= iMax) {
		printf("Object::collectNodes() cannot render more than %d objects.\n", iMax);
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
			iNodes[iNumber] = (Object *)this;
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


ci::Vec3f Object::getWindowCoords()
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
	auto curMat = getMatrix();
	for (int i=0; i < 16; i++) mM[i] = (double)curMat[i];

	Vec3f eyeCoord = gl::getModelView().transformPointAffine( Vec3f::zero() );
	Vec3f ndc = gl::getProjection().transformPoint( eyeCoord );
	auto viewport = gl::getViewport();
	float screenWidth = viewport.getWidth();
	float screenHeight = viewport.getHeight();

	return Vec3f( ( ndc.x + 1.0f ) / 2.0f * screenWidth, ( 1.0f - ( ndc.y + 1.0f ) / 2.0f ) * screenHeight, ndc.z );
}

const ci::Matrix44f& Object::getMatrix()
{
	return matrix;
}

const ci::Matrix44f& Object::getLocalMatrix()
{
	return localMatrix;

}



void Object::setLighting(bool iOnOff)
{
	isLit = iOnOff;
}

void Object::setColor(float r, float g, float b, float a)
{
	//color.set(r, g, b, a);
	material->color.set(r, g, b, a);			//v4.0
}

void Object::setAlpha(float iA)
{
	//color.set(color.x, color.y, color.z, iA);
	material->color.a = iA;
}

ci::ColorA8u Object::getColor()
{
	//return color;
	return material->color;	//v4.0
}

float Object::getAlpha()
{
	//return color.w;
	return material->color.a;	//v4.0
}

bool Object::hasTransparency()
{
	//if(hasSpecialTransparency || (color[3] < 255.0))
	if(hasSpecialTransparency || (drawMaterial->color[3] < 255.0))	//v4.0
		return true;
	else
		return false;
}

void Object::setSpecialTransparency(bool iFlag)
{
	hasSpecialTransparency = iFlag;
}

void Object::setRot(float x, float y, float z)
{
	xyzRot.set(x, y, z);
	localMatrixDirty = true;
}

void Object::setColor( Vec4f c )
{
	material->color = ColorA8u( c.x, c.y, c.z, c.w );
}

void Object::setColor( const ci::ColorA8u &c )
{
	//color = c;
	material->color = c;	 //v4.0
}


void Object::setRot(ci::Vec3f r)
{
	xyzRot = r;
	localMatrixDirty = true;
}


ci::Vec3f Object::getRot()
{
	return xyzRot;
}

ci::Vec3f Object::getTrans()
{
	return xyz;
}


void Object::setTrans(float x, float y, float z)
{
	xyz.set(x, y, z);

	localMatrix[12] = xyz[0];
	localMatrix[13] = xyz[1];
	localMatrix[14] = xyz[2];
	matrixDirty = true;
}


void Object::setTrans(ci::Vec3f vec)
{
	xyz = vec;

	localMatrix[12] = xyz[0];
	localMatrix[13] = xyz[1];
	localMatrix[14] = xyz[2];
	matrixDirty = true;
}


ci::Vec3f Object::getScale()
{
	return (scale);
}


void Object::setScale(float s)
{
	scale.set(s,s,s);
	localMatrixDirty = true;
}


void Object::setScale(float x, float y, float z)
{
	scale.set(x,y,z);
	localMatrixDirty = true;
}


void Object::setScale(ci::Vec3f vec)
{
	scale = vec;
	localMatrixDirty = true;
}

void Object::hide()
{
	shown = false;
}


void Object::show()
{
	shown = true;
}


bool Object::isShown()
{
	return shown;
}

int Object::getID()
{
	return id;
}



void Object::Mul(float *source1, float *source2, float *_dest)
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


void Object::Transpose(float *source, float *dest)
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

void Object::LoadIdentity(float *dest)
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



void Object::updateMessages()
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i]->isEnabled){

			//grab commonly needed values
			float startTime = messages[i]->startTime + messages[i]->startDelay;
			float endTime = messages[i]->getFinishTime();
			float duration = messages[i]->duration;
			float time = Message::interpolateTime(messages[i]->interpolation, (curTime - startTime)/duration);
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
						ci::Vec3f *vec = (ci::Vec3f *)messages[i]->baseStartVals;
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
						ci::Vec3f *vecEnd = (ci::Vec3f *)messages[i]->baseEndVals;
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
								((ci::Vec3f *)messages[i]->vals)->x,
								((ci::Vec3f *)messages[i]->vals)->y,
								((ci::Vec3f *)messages[i]->vals)->z);
						*/

						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setTrans((1-t)*((ci::Vec3f *)messages[i]->startVals)->x + t*((ci::Vec3f *)messages[i]->endVals)->x,
								 (1-t)*((ci::Vec3f *)messages[i]->startVals)->y + t*((ci::Vec3f *)messages[i]->endVals)->y,
								 (1-t)*((ci::Vec3f *)messages[i]->startVals)->z + t*((ci::Vec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ci::Vec4f trans = Message::bezier(t, messages[i]->pathPoints);
						setTrans(trans.x, trans.y, trans.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ci::Vec4f trans = Message::spline(t, messages[i]->pathPoints);
						setTrans(trans.x, trans.y, trans.z);
					}

				}
			}
			//rotation_____________________________________________________________
			else if(messages[i]->id == OF_ROTATE){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						ci::Vec3f *vec = (ci::Vec3f *)messages[i]->baseStartVals;
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
						ci::Vec3f *vecEnd = (ci::Vec3f *)messages[i]->baseEndVals;
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
						setRot((1-t)*((ci::Vec3f *)messages[i]->startVals)->x + t*((ci::Vec3f *)messages[i]->endVals)->x,
							   (1-t)*((ci::Vec3f *)messages[i]->startVals)->y + t*((ci::Vec3f *)messages[i]->endVals)->y,
							   (1-t)*((ci::Vec3f *)messages[i]->startVals)->z + t*((ci::Vec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ci::Vec4f rot = Message::bezier(t, messages[i]->pathPoints);
						setRot(rot.x, rot.y, rot.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ci::Vec4f rot = Message::spline(t, messages[i]->pathPoints);
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
						ci::Vec4f s = Message::bezier(t, messages[i]->pathPoints);
						setScale(s.x);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ci::Vec4f s = Message::spline(t, messages[i]->pathPoints);
						setScale(s.x);
					}
				}
			}
			//scaling in 3 dimensions differently ___________________________________
			else if(messages[i]->id == OF_SCALE3){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start avlues once
						ci::Vec3f *vec = (ci::Vec3f *)messages[i]->baseStartVals;
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
						ci::Vec3f *vecEnd = (ci::Vec3f *)messages[i]->baseEndVals;
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
						setScale((1-t)*((ci::Vec3f *)messages[i]->startVals)->x + t*((ci::Vec3f *)messages[i]->endVals)->x,
								 (1-t)*((ci::Vec3f *)messages[i]->startVals)->y + t*((ci::Vec3f *)messages[i]->endVals)->y,
								 (1-t)*((ci::Vec3f *)messages[i]->startVals)->z + t*((ci::Vec3f *)messages[i]->endVals)->z);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ci::Vec4f s = Message::bezier(t, messages[i]->pathPoints);
						setScale(s.x, s.y, s.z);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ci::Vec4f s = Message::spline(t, messages[i]->pathPoints);
						setScale(s.x, s.y, s.z);
					}
				}
			}
			//color _________________________________________________________________
			else if(messages[i]->id == OF_SETCOLOR){
				if(curTime >= startTime){
					if(!messages[i]->isRunning){
						//set start values once
						ci::Vec3f *vec = (ci::Vec3f *)messages[i]->baseStartVals;
						if(vec){
							if(vec->x == OF_RELATIVE_VAL) x = material->color.r;
							else x = vec->x;
							if(vec->y == OF_RELATIVE_VAL) y = material->color.g;
							else y = vec->y;
							if(vec->z == OF_RELATIVE_VAL) z = material->color.b;
							else z = vec->z;

							messages[i]->setStartVals(x, y, z);
						}

						//set end values once
						ci::Vec3f *vecEnd = (ci::Vec3f *)messages[i]->baseEndVals;
						if(vecEnd){
							if(vecEnd->x == OF_RELATIVE_VAL) x = material->color.r;
							else x = vecEnd->x;
							if(vecEnd->y == OF_RELATIVE_VAL) y = material->color.g;
							else y = vecEnd->y;
							if(vecEnd->z == OF_RELATIVE_VAL) z = material->color.b;
							else z = vecEnd->z;

							messages[i]->setEndVals(x,y,z);
						}

						//printf("color startvals = %f, %f , %f\n", x,y,z);
						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setColor((1-t)*((ci::Vec3f *)messages[i]->startVals)->x + t*((ci::Vec3f *)messages[i]->endVals)->x,
								 (1-t)*((ci::Vec3f *)messages[i]->startVals)->y + t*((ci::Vec3f *)messages[i]->endVals)->y,
								 (1-t)*((ci::Vec3f *)messages[i]->startVals)->z + t*((ci::Vec3f *)messages[i]->endVals)->z,
								 material->color.a);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ci::Vec4f c = Message::bezier(t, messages[i]->pathPoints);
						setColor(c.x, c.y, c.z, material->color.a);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ci::Vec4f c = Message::spline(t, messages[i]->pathPoints);
						setColor(c.x, c.y, c.z, material->color.a);
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
							if(v[0] == OF_RELATIVE_VAL) x = material->color.a;
							else x = v[0];

							messages[i]->setStartVals(x);
						}

						//set end values once
						float *vEnd = (float *)messages[i]->baseEndVals;
						if(vEnd){
							if(vEnd[0] == OF_RELATIVE_VAL) x = material->color.a;
							else x = vEnd[0];

							messages[i]->setEndVals(x);
						}

						messages[i]->isRunning = true;
					}
					//update value
					if(messages[i]->path == OF_LINEAR_PATH){
						setAlpha((1-t)*((float *)messages[i]->startVals)[0] + t*((float *)messages[i]->endVals)[0]);
					}else if(messages[i]->path == OF_BEZIER_PATH){
						ci::Vec4f s = Message::bezier(t, messages[i]->pathPoints);
						setAlpha(s.x);
					}else if(messages[i]->path == OF_SPLINE_PATH){
						ci::Vec4f s = Message::spline(t, messages[i]->pathPoints);
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

bool Object::isAnimating()
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i]->isRunning)
			return true;
	}
	return false;
}

/*	//deprecated - we now clean messages up right in updateMessage() on a one-by-one basis using deleteMessage()
void Object::cleanupMessages()
{
	float curTime = ofGetElapsedTimef();
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i]->autoDelete){
			if(curTime > messages[i]->getFinishTime()){
				//printf("message %d deleted!\n", i);
				delete(messages[i]);
				messages.erase(messages.begin() + i);
				i--;

			}
		}
	}
}
*/

void Object::deleteMessage(Message *iMessage)
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(iMessage == messages[i]){
			delete(iMessage);
			messages.erase(messages.begin() + i);
			//printf("Object::deleteMessage()\n");

			return;
		}
	}
}

//Remove it, but don't delete it.
bool Object::removeMessage(Message *iMessage)
{
	//printf("Object::removeMessage()\n");
	for(unsigned int i=0; i < messages.size(); i++){
		if(iMessage == messages[i]){
			messages.erase(messages.begin() + i);
			//printf("succeeded\n");
			return true;
		}
	}
	return false;
}


void Object::stopMessages(int iMessageType)
{
	bool deleteFlag = true;
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


Message* Object::doMessage0f(int iID, float iDelay, float iDuration, int iInterp)
{
	Message *message =new Message(iID, NULL, iInterp, iDuration, iDelay);
	messages.push_back(message);

	return message;
}

Message* Object::doMessage1f(int iID, float iDelay, float iDuration, int iInterp, float iVal)
{
	float *args = new float[1];
	args[0] = iVal;

	Message *message = new Message(iID, (void *)args, iInterp, iDuration, iDelay);
    message->setStartTime(curTime);
	messages.push_back(message);

	return message;
}

Message* Object::doMessage3f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2)
{
	ci::Vec3f *args = new ci::Vec3f();
	args->set(iVal0, iVal1, iVal2);

	Message *message = new Message(iID, (void *)args, iInterp, iDuration, iDelay);
    message->setStartTime(curTime);
	messages.push_back(message);

	return message;
}

Message* Object::doMessage4f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2, float iVal3)
{
	ci::Vec4f *args = new ci::Vec4f();
	args->set(iVal0, iVal1, iVal2, iVal3);

	Message *message =new Message(iID, (void *)args, iInterp, iDuration, iDelay);
    message->setStartTime(curTime);
	messages.push_back(message);

	return message;
}

Message* Object::doMessageNf(int iID, float iDelay, float iDuration, int iInterp, int iPath, vector<ci::Vec4f> iPathPoints)
{
	Message *message = new Message(iID, iInterp, iPath, iPathPoints, iDuration, iDelay);
    message->setStartTime(curTime);
	messages.push_back(message);

	return message;
}

//Sse a message that has already been made
//sets startTime of iMessage to current time
Message* Object::doMessage(Message *iMessage)
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
bool Object::hasMessage(Message *iMessage)
{
	for(unsigned int i=0; i < messages.size(); i++){
		if(messages[i] == iMessage)
			return true;
	}
	return false;
}

int Object::isDescendant(ObjectRef iObject)
{
	for (unsigned int i = 0; i < children.size(); i++) {
		ObjectRef obj = children[i];
		if (obj == iObject) return(1);
		else if (obj->isDescendant(iObject)) return(1);
	}

	return(0);

}

void Object::setDisplayList(GLuint iList)
{
    displayList = iList;
    //Tells render to use the list.
    displayListFlag = true;
}
