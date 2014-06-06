//
//  ofxColorRect.h
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//
#pragma once

#include "ofxObject.h"
#include "ofxRectangleObject.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive

// extending classes for shaders
class ofxColorRect : public ofxRectangleObject {
  
public:
  
  ofxColorRect(float iW, float iH) : ofxRectangleObject(iW, iH){};
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
  void setMouseVals(float iMouseX, float iMouseY);
  
private:
  
  float mouseX = 0;
  float mouseY = 0;
  
  
};