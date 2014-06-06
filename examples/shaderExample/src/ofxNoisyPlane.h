
/*
 ofxNoisyPlane
 
 */

#pragma once

#include "ofxObject.h"
#include "ofImage.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive
#include "ofxImageObject.h"

class ofxNoisyPlane : public ofxObject{
	
public:
	
	ofxNoisyPlane(int iWidth, int iHeight, int iCols, int iRows);
	~ofxNoisyPlane();
	
	void						render();
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	void						clear();
  void  idle(float iTime);
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
	
private:
  
  void generateNoise();
  
private:
  
	ofPlanePrimitive  *plane;
	bool  isCentered;
	float width,
  height;
  
  //ofxImageObject  *noiseRef; // An image that will be used for generating noise
  ofImage *noiseRef;
  
};
