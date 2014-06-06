//
//  ofxColorRect.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "ofxColorRect.h"

void ofxColorRect::setShaderParams(){

  float percentX = mouseX / 2048;
  percentX = ofClamp(percentX, 0, 1);
  
  float percentY = abs(mouseY / 1024);
  percentY = ofClamp(percentY, 0, 1);
  
  shader->setUniform1f("percentX", percentX);
  shader->setUniform1f("percentY", percentY);
  
}


void ofxColorRect::setMouseVals(float iMouseX, float iMouseY){
  
  mouseX = iMouseX;
  mouseY = iMouseY;
}