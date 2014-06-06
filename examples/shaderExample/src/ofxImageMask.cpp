//
//  ofxImageMask.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "ofxImageMask.h"


ofxImageMask::ofxImageMask(string iFilename1, string iFilename2){
  
  image1 = new ofxImageObject(iFilename1);
  image2 = new ofxImageObject(iFilename2);
  

  addChild(image1);
  addChild(image2);
}

ofxImageMask::~ofxImageMask(){
  
  removeChild(image1);
  removeChild(image2);
  
}

void ofxImageMask::idle(float iTime){
  
  
}


// Set shader's parameters
void ofxImageMask::setShaderParams(){
  
  //shader->setUniform1f("time", ofGetElapsedTimef()*3);
  //plane->mapTexCoordsFromTexture(noiseRef->image->getTextureReference());
  shader->setUniformTexture("tex0", *image1->image, 1);
  shader->setUniformTexture("mask", *image2->image, 2);
  
}