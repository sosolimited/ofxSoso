#include "ofxMessage.h"
#include "ofUtils.h"

//float ofxMessage::OF_RELATIVE_VAL = 9999.9f;

ofxMessage::ofxMessage(int iID, void *iVals, int iInterpolation, float iDuration, float iDelay, int iPlayMode)
{
	id = iID;
	baseEndVals = iVals;
	path = OF_LINEAR_PATH;
	playMode = iPlayMode;
	loopDirection = false;

	//startVals = NULL;
	//initialize all start values to OF_RELATIVE_VAL
	if((id==OF_SCALE) || (id==OF_SETALPHA)){
		float *val = (float *)malloc(sizeof(float));
		val[0] = 0;
		startVals = (void *)val;

		float *baseVal = (float *)malloc(sizeof(float));
		baseVal[0] = OF_RELATIVE_VAL;
		baseStartVals = (void *)baseVal;

		float *end = (float *)malloc(sizeof(float));
		end[0] = 0;
		endVals = (void *)end;		
		
	}else if((id==OF_TRANSLATE) || (id==OF_ROTATE) || (id==OF_SETCOLOR) || (id==OF_SCALE3)){
		ofVec3f *vals = (ofVec3f *)malloc(sizeof(ofVec3f));
		vals->set(0,0,0);
		startVals = (void *)vals;

		ofVec3f *baseVal = (ofVec3f *)malloc(sizeof(ofVec3f));
		baseVal->set(OF_RELATIVE_VAL, OF_RELATIVE_VAL, OF_RELATIVE_VAL);
		baseStartVals = (void *)baseVal;

		ofVec3f *end = (ofVec3f *)malloc(sizeof(ofVec3f));
		end->set(0,0,0);
		endVals = (void *)end;
	}else if(id==OF_SETCOLOR4){
		ofVec4f *vals = (ofVec4f *)malloc(sizeof(ofVec3f));
		vals->set(0,0,0,0);
		startVals = (void *)vals;
    
		ofVec4f *baseVal = (ofVec4f *)malloc(sizeof(ofVec3f));
		baseVal->set(OF_RELATIVE_VAL, OF_RELATIVE_VAL, OF_RELATIVE_VAL, OF_RELATIVE_VAL);
		baseStartVals = (void *)baseVal;
    
		ofVec4f *end = (ofVec4f *)malloc(sizeof(ofVec3f));
		end->set(0,0,0,0);
		endVals = (void *)end;
  }else{
		startVals = NULL;
		baseStartVals = NULL;
		endVals = NULL;
	}

	interpolation = iInterpolation;
	duration = iDuration;
	startDelay = iDelay;
	startTime = ofGetElapsedTimef();	//default to current time
	
	isEnabled = true;	
	isRunning = false;
	autoDelete = true;	
}

ofxMessage::ofxMessage(int iID, int iInterpolation, int iPath, vector<ofVec4f> iPathPoints, float iDuration, float iDelay, int iPlayMode)
{
	id = iID;	
	path = iPath;
	pathPoints = iPathPoints;
	playMode = iPlayMode;
	loopDirection = false;

	baseEndVals = NULL;
	startVals = NULL;
	baseStartVals = NULL;
	endVals = NULL;

	interpolation = iInterpolation;
	duration = iDuration;
	startDelay = iDelay;
	startTime = ofGetElapsedTimef();	//default to current time
    
	isEnabled = true;	
	isRunning = false;
	autoDelete = true;	
}

ofxMessage::ofxMessage(int iID, float (*iFunction)(void *), void *iArgs, float iDelay, int iPlayMode) //LM
{
	id = iID;
	path = OF_LINEAR_PATH;
	playMode = iPlayMode;
	loopDirection = false;
    
    functionPtr = iFunction;
    startVals = iArgs;
    baseStartVals = NULL;
    endVals = NULL;
    baseEndVals = NULL;
    
	interpolation = NULL;
	duration = 0;
	startDelay = iDelay;
	startTime = ofGetElapsedTimef();	//default to current time
	
	isEnabled = true;	
	isRunning = false;
	autoDelete = true;	
}

ofxMessage::~ofxMessage()
{
  if(startVals) free(startVals);
	if(baseStartVals) free(baseStartVals);
	if(endVals) free(endVals);
  
  // Since baseEndVals was created with 'new', we must cast them back to their original type (Message1f/float, Message3f/ofVec3f) in order to destroy them correctly based on type.
  if(baseEndVals) {
    if(id==OF_SETALPHA){
      delete[] (float *)baseEndVals;
            cout<<"* OF_SETALPHA * baseEndVals = "<< baseEndVals <<endl;
    } else if(id==OF_SCALE){
      delete[] (float *)baseEndVals;
            cout<<"* OF_SCALE * baseEndVals = "<< baseEndVals <<endl;
    } else if(id==OF_TRANSLATE){
      delete (ofVec3f *)baseEndVals;
            cout<<"* OF_TRANSLATE * baseEndVals = "<< baseEndVals <<endl;
    } else if(id==OF_ROTATE){
      delete (ofVec3f *)baseEndVals;
            cout<<"* OF_ROTATE * baseEndVals = "<< baseEndVals <<endl;
    } else if(id==OF_SCALE3){
      delete (ofVec3f *)baseEndVals;
            cout<<"* OF_SCALE3 * baseEndVals = "<< baseEndVals <<endl;
    } else if(id==OF_SETCOLOR){
      delete (ofVec3f *)baseEndVals;
            cout<<"* OF_SETCOLOR * baseEndVals = "<< baseEndVals <<endl;
    }
  }
  
  //DEV: should we delete functionPtr?
  
}

float ofxMessage::getFinishTime()
{
	return (startTime + startDelay + duration);
}

void ofxMessage::setStartTime(float iTime)
{
	startTime = iTime;
}

void ofxMessage::enableMessage(bool iEnable)
{
	isEnabled = iEnable;
}

//these three methods set the start values and allocate memory for them if they are still null 
void ofxMessage::setStartVals(float iX)
{
	if(startVals == NULL){	
		float *val = (float *)malloc(sizeof(float));
		val[0] = iX;
		startVals = (void *)val;
		//printf("ofxMessage::setStartVals(%f) - NULL\n", iX);
	}else{
		((float *)startVals)[0] = iX;
		//printf("ofxMessage::setStartVals(%f)\n", iX);
	}
}

void ofxMessage::setStartVals(float iX, float iY, float iZ)
{
	if(startVals == NULL){						
		ofVec3f *vals = (ofVec3f *)malloc(sizeof(ofVec3f));
		vals->set(iX, iY, iZ);
		startVals = (void *)vals;
		//printf("ofxMessage::setStartVals(%f, %f, %f) - NULL\n", ((ofVec3f *)startVals)->x, ((ofVec3f *)startVals)->y, ((ofVec3f *)startVals)->z);
	}else{
		((ofVec3f *)startVals)->set(iX, iY, iZ);		
		//printf("ofxMessage::setStartVals(%f, %f, %f) - NULL\n", ((ofVec3f *)startVals)->x, ((ofVec3f *)startVals)->y, ((ofVec3f *)startVals)->z);
	}
}

void ofxMessage::setStartVals(float iX, float iY, float iZ, float iW)
{
	if(startVals == NULL){						
		ofVec4f *vals = (ofVec4f *)malloc(sizeof(ofVec4f));
		vals->set(iX, iY, iZ, iW);
		startVals = (void *)vals;
	}else{
		((ofVec4f *)startVals)->set(iX, iY, iZ, iW);
	}
}

void ofxMessage::setBaseStartVals(float iX)
{
	if(baseStartVals)
		((float *)baseStartVals)[0] = iX;
}

void ofxMessage::setBaseStartVals(float iX, float iY, float iZ)
{
	if(baseStartVals)
		((ofVec3f *)baseStartVals)->set(iX, iY, iZ);
}

void ofxMessage::setBaseStartVals(float iX, float iY, float iZ, float iW)
{
	((ofVec4f *)baseStartVals)->set(iX, iY, iZ, iW);
}

void ofxMessage::setEndVals(float iX)
{
	if(endVals)
		((float *)endVals)[0] = iX;
}

void ofxMessage::setEndVals(float iX, float iY, float iZ)
{
	if(endVals)
		((ofVec3f *)endVals)->set(iX, iY, iZ);
}

void ofxMessage::setEndVals(float iX, float iY, float iZ, float iW)
{
	if(endVals)
		((ofVec4f *)endVals)->set(iX, iY, iZ, iW);
}


//static method for interpolating time 
float ofxMessage::interpolateTime(int iInterp, float iTime)
{	
	if (iTime >= 1.0f){
		//iTime = 1.0f;
		return 1.0f;
	}else if(iTime < 0){
		return 0.0f;
	}

	switch (iInterp) {
		case OF_LINEAR:		
			return (iTime);
			break;
		case OF_EASE_OUT:		
			return (1.0f-(iTime-1.0f)*(iTime-1.0f));		
			break;
		case OF_EASE_IN:
			return (iTime*iTime);
			break;
		case (OF_EASE_INOUT):	
			if (iTime <= 0.5f) {
				return((iTime*2.0f)*(iTime*2.0f)*0.5f);
			} 
			else if (iTime > 0.5f) {
				return((1.0f-((iTime-0.5f)*2.0f-1.0f)*((iTime-0.5f)*2.0f-1.0f))*0.5f + 0.5f);
			}
			break;	
	}

	return iTime;
}



//iM goes from 0 - 1.0
ofVec4f ofxMessage::bezier(float iM, std::vector<ofVec4f> iPath) 
{
	ofVec4f r;
	
	//if (iM > 1.0) iM = 1.0;
	if (iM > 0.999) iM = 0.999;

	int n = iPath.size()-1;
	int k,kn,nn,nkn;
	double blend,muk,munk;
	float bx = 0;
	float by = 0;
	float bz = 0;
	float bw = 0;

	muk = 1;
	munk = pow(1.0 - (double)iM, (double)n);

	for (k = 0; k <= n; k++) {
		nn = n;
		kn = k;
		nkn = n - k;
		blend = muk * munk;
		muk *= iM;
		munk /= (1-iM);
		while (nn >= 1) {
			blend *= nn;
			nn--;
			if (kn > 1) {
				blend /= (double)kn;
				kn--;
			}
			if (nkn > 1) {
				blend /= (double)nkn;
				nkn--;
			}
		}    
		bx += iPath[k].x * blend;
		by += iPath[k].y * blend;
		bz += iPath[k].z * blend;
		bw += iPath[k].w * blend;
	}

	r.set(bx, by, bz, bw);

	return(r);
}


ofVec4f ofxMessage::spline(float iM, std::vector<ofVec4f> iPath) 
{
	ofVec4f r;

	float p0, p1, p2, p3;
	float xx, yy, zz, ww;
	int nPoint;
	float t;

	// first figure out which two points you are between on the entire spline /////////
	nPoint = (int)(iM * (float)(iPath.size() - 1));	
	
	// then figure out how far along you are between the adjacent control points //////
	float step = 1.0f/(float)(iPath.size() - 1);
	t = fmod((float)iM, step)/step;
	
	// check to see if you're at the boundaries ////////////////////////////////////////
	int nPointPrev = nPoint - 1;
	int nPointNext = nPoint + 1;
	int nPointNextNext = nPoint + 2;

	if(nPointPrev < 0){
		nPointPrev = 0;
	}	
	else if(nPointNext > ((int)iPath.size() - 1)){
		nPointNext = (int)iPath.size() - 1;
		nPointNextNext = nPointNext;
	}
	else if(nPointNextNext > ((int)iPath.size() - 1)){
		nPointNextNext = (int)iPath.size() - 1;
	}

	// do the spline calculations /////////////////////////////////////////////////////
	// First calculate the X coefficients
	p0 = iPath[nPointPrev][0]; 
	p1 = iPath[nPoint][0];
	p2 = iPath[nPointNext][0];
	p3 = iPath[nPointNextNext][0];

	xx = 0.5*((2.0*p1) + (-p0 + p2)*t + (2.0*p0 - 5.0*p1 + 4*p2 - p3)*t*t + (-p0 + 3*p1 - 3.0*p2 + p3)*t*t*t);
	
	// Next, the Y coefficients
	p0 = iPath[nPointPrev][1]; 
	p1 = iPath[nPoint][1];
	p2 = iPath[nPointNext][1]; 
	p3 = iPath[nPointNextNext][1];

	yy = 0.5*((2.0*p1) + (-p0 + p2)*t + (2.0*p0 - 5.0*p1 + 4*p2 - p3)*t*t + (-p0 + 3*p1 - 3.0*p2 + p3)*t*t*t);

	// Finally, the Z coefficients
	p0 = iPath[nPointPrev][2]; 
	p1 = iPath[nPoint][2]; 
	p2 = iPath[nPointNext][2]; 
	p3 = iPath[nPointNextNext][2]; 

	zz = 0.5*((2.0*p1) + (-p0 + p2)*t + (2.0*p0 - 5.0*p1 + 4*p2 - p3)*t*t + (-p0 + 3*p1 - 3.0*p2 + p3)*t*t*t);

	// Finally, the W coefficients 
	p0 = iPath[nPointPrev][3]; 
	p1 = iPath[nPoint][3]; 
	p2 = iPath[nPointNext][3]; 
	p3 = iPath[nPointNextNext][3]; 

	ww = 0.5*((2.0*p1) + (-p0 + p2)*t + (2.0*p0 - 5.0*p1 + 4*p2 - p3)*t*t + (-p0 + 3*p1 - 3.0*p2 + p3)*t*t*t);

	r.set(xx,yy,zz,ww);

	return(r);
}