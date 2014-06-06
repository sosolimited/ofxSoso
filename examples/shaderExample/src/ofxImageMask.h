
/*
 ofxNoisyPlane
 
 */

#pragma once

#include "ofxObject.h"
#include "ofImage.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive
#include "ofxImageObject.h"

class ofxImageMask : public ofxObject{
	
public:
	
	ofxImageMask(string iFilename1, string iFilename2);
	~ofxImageMask();
	
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	void						clear();
  void  idle(float iTime);
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
	
private:
  
  
private:

	bool  isCentered;
	float width,
  height;
  
  ofxImageObject *image1, *image2;
  
  
};
