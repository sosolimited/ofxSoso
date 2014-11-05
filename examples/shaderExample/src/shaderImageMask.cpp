//
//  shaderImageMask.cpp
//  shaderExample
//
//  Created by Alex Olivier on 6/6/14.
//
//  This example shows how to pass multiple textures
//  to a shader.  It uses one texture as a mask.

// Based on openFrameworks shader tutorials

#include "shaderImageMask.h"


shaderImageMask::shaderImageMask(string iFilename1, string iFilename2){
  
  // Create 2 image objects
  image1 = new ofxImageObject(iFilename1);
  image2 = new ofxImageObject(iFilename2);

  addChild(image1);
  addChild(image2);
}

// Destroy objects
shaderImageMask::~shaderImageMask(){
  
  removeChild(image1);
  removeChild(image2);
  
  delete image1;
  delete image2;
  
}

void shaderImageMask::idle(float iTime){
  
  
}


//  Override default method to set shader parameters
//  Pass 2 textures to our shaders
void shaderImageMask::setShaderParams(){
  
  // Pass one texture in as "tex0"
  // Pass second texture in as "mask"
  shader->setUniformTexture("tex0", *image1->tex, 1);
  shader->setUniformTexture("mask", *image2->tex, 2);
  
}