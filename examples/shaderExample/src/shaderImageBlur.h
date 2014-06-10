
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
	
  void  render();

  // Pass mouse vals
  void setMouseVals(float iMouseX, float iMouseY);
  
  // Pass app width
  void setAppDimensions(float iW, float iH);
  

private:
  
  float mouseX = 0;
  float mouseY = 0;
  float appWidth = 0;
  float appHeight = 0;
  
  ofImage*  image;
  
  ofShader  *shaderBlurX;
  ofShader  *shaderBlurY;
  
  ofxFboObject  *fboBlurOnePass;
  ofxFboObject  *fboBlurTwoPass;
  
  float blur = 5.0f;
  
};
