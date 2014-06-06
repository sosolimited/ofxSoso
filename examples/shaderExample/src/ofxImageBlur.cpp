//
//  ofxImageBlur.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "ofxImageBlur.h"

ofxImageBlur::ofxImageBlur(string iFilename){
  
  image = new ofImage();
  image->loadImage(iFilename);
  
  fboBlurOnePass = new ofFbo();
  fboBlurTwoPass = new ofFbo();
  
  fboBlurOnePass->allocate(image->getWidth(), image->getHeight());
  fboBlurTwoPass->allocate(image->getWidth(), image->getHeight());
  
  shaderX = new ofShader();
  shaderY = new ofShader();
  
  shaderX->load("shaders/shader5/shaderBlurX");
  shaderY->load("shaders/shader5/shaderBlurY");
  
}

ofxImageBlur::~ofxImageBlur(){
  
  delete image;
  delete shaderX;
  delete shaderY;
  delete fboBlurOnePass;
  delete fboBlurTwoPass;
  
  
}

void ofxImageBlur::idle(float iTime){
  
  
}

void ofxImageBlur::render(){
  
  float blur = 5;
  
  // Begin fbo one
  fboBlurOnePass->begin(false);
  
  //begin first shader
  shaderX->begin();
  shaderX->setUniform1f("blurAmnt", blur);
  
  image->draw(0,0);
  
  shaderX->end();
  
  //start fbo 2
  fboBlurTwoPass->begin(false);
  
  shaderY->begin();
  shaderY->setUniform1f("blurAmnt", blur);
  
  fboBlurOnePass->draw(0,0);
  
  shaderY->end();
  
  fboBlurTwoPass->end();
  
  ofSetColor(ofColor::white);
  fboBlurTwoPass->draw(0,0);
  
  
  
  
}


// Set shader's parameters
void ofxImageBlur::setShaderParams(){
  
  //shader->setUniform1f("time", ofGetElapsedTimef()*3);
  //plane->mapTexCoordsFromTexture(noiseRef->image->getTextureReference());
  //shader->setUniformTexture("tex0", *image->image, 1);
  
}

void ofxImageBlur::setMouseVals(float iMouseX, float iMouseY){
  
  mouseX = iMouseX;
  mouseY = iMouseY;
}