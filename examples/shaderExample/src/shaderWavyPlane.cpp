//
//  wavyPlane.cpp
//  shaderExample
//
//  Created by Alex Olivier on 6/6/14.
//
//
// Based on openFrameworks shader tutorials

#include "shaderWavyPlane.h"

shaderWavyPlane::shaderWavyPlane(int iWidth, int iHeight, int iCols, int iRows){
  
  plane = new ofPlanePrimitive();
  plane->set(iWidth, iHeight, iCols, iRows, OF_PRIMITIVE_TRIANGLES);
  
}

// Destroy everything
shaderWavyPlane::~shaderWavyPlane(){
  
  delete plane;
  
}

// Implement render to draw plane
void shaderWavyPlane::render(){
  
  plane->drawWireframe();

}

// Extending class to override shader parameters
void shaderWavyPlane::setShaderParams(){
  
  // Create and pass a float values to shaders
  // called "time"
  shader->setUniform1f("time", ofGetElapsedTimef()*4);
  
}