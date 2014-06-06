//
//  wavyPlane.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "ofxWavyPlane.h"

ofxWavyPlane::ofxWavyPlane(int iWidth, int iHeight, int iCols, int iRows){
  
  plane = new ofPlanePrimitive();
  plane->set(iWidth, iHeight, iCols, iRows, OF_PRIMITIVE_TRIANGLES);
  
}

// Destroy everything
ofxWavyPlane::~ofxWavyPlane(){
  
  delete plane;
  
}

void ofxWavyPlane::render(){
  
  plane->drawWireframe();

}

// Set shader's parameters
void ofxWavyPlane::setShaderParams(){
  
  shader->setUniform1f("time", ofGetElapsedTimef()*3);
  
}