
/*
 shaderNoisyPlane
 
 */

#pragma once

#include "ofxObject.h"
#include "ofImage.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive
#include "ofxImageObject.h"
#include "ofxFboObject.h"

class shaderImageBlur : public ofxObject{
	
public:
	
	shaderImageBlur(string iFilename);
	~shaderImageBlur();
	
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	void						clear();
  void  idle(float iTime);
  void render();
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
  void setMouseVals(float iMouseX, float iMouseY);
  
  void doBlur();
  
  void preDraw();
	
private:
  
  
private:
  
	bool  isCentered;
	float width,
  height;
  
  float mouseX = 0;
  float mouseY = 0;
  
  ofImage*image;
  
  ofShader *shaderX;
  ofShader *shaderY;
  
  ofFbo *fboBlurOnePass;
  ofFbo *fboBlurTwoPass;
  
  ofxFboObject *fbo1;
  ofxFboObject *fbo2;
  
  float blur = 5.0f;
  
};
