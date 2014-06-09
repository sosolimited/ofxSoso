//
//  shaderColorRect.h
//  shaderExample
//
//  Created by Alex Olivier on 6/6/14.
//
//

#pragma once

#include "ofxObject.h"
#include "ofxRectangleObject.h"

// Extending class to override shader parameters
class shaderColorRect : public ofxRectangleObject {
  
public:
  
  shaderColorRect(float iW, float iH) : ofxRectangleObject(iW, iH){};
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
  
  // Pass mouse values from app
  void setMouseVals(float iMouseX, float iMouseY);
  
  // Pass app width
  void setAppDimensions(float iW, float iH);
  
private:
  
  float mouseX = 0;
  float mouseY = 0;
  float appWidth = 0;
  float appHeight = 0;
  
};