//
//  wavyPlane.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "shaderWavyPlane.h"

shaderWavyPlane::shaderWavyPlane(int iWidth, int iHeight, int iCols, int iRows){
  
  plane = new ofPlanePrimitive();
  plane->set(iWidth, iHeight, iCols, iRows, OF_PRIMITIVE_TRIANGLES);
  
}

// Destroy everything
shaderWavyPlane::~shaderWavyPlane(){
  
  delete plane;
  
}

void shaderWavyPlane::render(){
  
  plane->drawWireframe();

}

// Set shader's parameters
void shaderWavyPlane::setShaderParams(){
  
  shader->setUniform1f("time", ofGetElapsedTimef()*3);
  
}