
/*
 shaderNoisyPlane
 
 */

#pragma once

#include "ofxObject.h"
#include "ofImage.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive
#include "ofxImageObject.h"

class shaderNoisyPlane : public ofxObject{
	
public:
	
	shaderNoisyPlane(int iWidth, int iHeight, int iCols, int iRows);
	~shaderNoisyPlane();
	
	void						render();
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	void						clear();
  void  idle(float iTime);
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
  
  // Pass mouse values from app
  void setMouseVals(float iMouseX, float iMouseY);
  
  // Pass app width
  void setAppDimensions(float iW, float iH);
	
private:
  
  void generateNoise();
  
private:
  
	ofPlanePrimitive  *plane;
	bool  isCentered;
	float width,
  height;
  
  //ofxImageObject  *noiseRef; // An image that will be used for generating noise
  ofImage *noiseRef;
  
  
  float mouseX = 0;
  float mouseY = 0;
  float appWidth = 0;
  float appHeight = 0;
  
};
