//
//  shaderColorRect.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "shaderColorRect.h"

void shaderColorRect::setShaderParams(){

  float percentX = mouseX / 1500;
  percentX = ofClamp(percentX, 0, 1);
  
  float percentY = abs(mouseY / 1024);
  percentY = ofClamp(percentY, 0, 1);
  
  shader->setUniform1f("percentX", percentX);
  shader->setUniform1f("percentY", percentY);
  
}


void shaderColorRect::setMouseVals(float iMouseX, float iMouseY){
  
  mouseX = iMouseX;
  mouseY = iMouseY;
}