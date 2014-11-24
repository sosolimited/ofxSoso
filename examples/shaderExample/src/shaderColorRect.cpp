//
//  shaderColorRect.cpp
//  shaderExample
//
//  Created by Alex Olivier on 6/6/14.
//
//
// Based on openFrameworks shader tutorials

#include "shaderColorRect.h"


void shaderColorRect::setShaderParams(){

  // Calculate mouse x as percentage
  // of total app width
  float percentX = mouseX / appWidth;
  percentX = ofClamp(percentX, 0, 1);
  
  // Calculate mouse y as percentage
  // of total app height
  float percentY = abs(mouseY / appHeight);
  percentY = ofClamp(percentY, 0, 1);
  
  
  // Create and pass 2 float values to shaders
  // called "percentX" and "percentY"
  shader->setUniform1f("percentX", percentX);
  shader->setUniform1f("percentY", percentY);
  
}

// Passes mouse values from app
void shaderColorRect::setMouseVals(float iMouseX, float iMouseY){
  
  mouseX = iMouseX;
  mouseY = iMouseY;

}

// Passes mouse values from app
// Normally, we might just grab an instance of the app
// but we run into circular references in this case
void shaderColorRect::setAppDimensions(float iW, float iH){
  
  appWidth = iW;
  appHeight = iH;
  
}