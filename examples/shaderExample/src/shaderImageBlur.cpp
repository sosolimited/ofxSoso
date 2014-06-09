//
//  shaderImageBlur.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "shaderImageBlur.h"

shaderImageBlur::shaderImageBlur(string iFilename){
  
  image = new ofImage();
  image->loadImage(iFilename);
  image->getTextureReference().texData.bFlipTexture = true;
  
  fboBlurOnePass = new ofFbo();
  fboBlurTwoPass = new ofFbo();
  
  ofLogNotice("Image width is " + ofToString(image->getWidth()));
  ofLogNotice("Image height is " + ofToString(image->getHeight()));
  
  fboBlurOnePass->allocate(image->getWidth(), image->getHeight());
  fboBlurTwoPass->allocate(image->getWidth(), image->getHeight());
  
  fbo1 = new ofxFboObject(image->getWidth(), image->getHeight());
  fbo2 = new ofxFboObject(image->getWidth(), image->getHeight());
  
  //addChild(fbo1);
  addChild(fbo2);
  
  shaderX = new ofShader();
  shaderY = new ofShader();

  
  shaderX->load("shaders/image_blur/shaderBlurX");
  shaderY->load("shaders/image_blur/shaderBlurY");
  

  //setEnableShaders(false);
  
}

shaderImageBlur::~shaderImageBlur(){
  
  delete image;
  delete shaderX;
  delete shaderY;
  delete fboBlurOnePass;
  delete fboBlurTwoPass;
  
  
}

void shaderImageBlur::preDraw(){
  
    doBlur();
}

void shaderImageBlur::doBlur(){
  
  if (shaderEnabled)
    blur = ofMap(mouseX, 0, 2048, 0, 10, true);
  else
    blur = 0;
  
  //ofLogNotice(ofToString("blur"));

  fbo1->fbo->begin();
  ofClear(255,255,255, 0);
  shaderX->begin();
  shaderX->setUniform1f("blurAmnt", blur);
  
  image->draw(0,0);
  shaderX->end();
  fbo1->fbo->end();
  
  fbo2->fbo->begin();
  ofClear(255,255,255, 0);
  shaderY->begin();
  shaderY->setUniform1f("blurAmnt", blur);
  fbo1->render();
  shaderY->end();
  fbo2->fbo->end();
  
}

void shaderImageBlur::idle(float iTime){
  

 
}

void shaderImageBlur::render(){
  
  

  doBlur();
  
  //ofLogNotice("blur is " + ofToString(blur));
  


  
  
  
  
  //float blur = ofMap(mouseX, 0, 2048, 0, 10, true);
//  float blur = 5.0f;
//  
//  //----------------------------------------------------------
//  fboBlurOnePass->begin();
//  
//  //begin first shader
//  //shaderX->begin();
//  //shaderX->setUniform1f("blurAmnt", blur);
//  
//  image->draw(0,0);
//  
//  //shaderX->end();
//  fboBlurOnePass->end();
//  
//  fboBlurOnePass->draw(0,0); //test code
/*
  //----------------------------------------------------------
  fboBlurTwoPass->begin();
  
  shaderY->begin();
  shaderY->setUniform1f("blurAmnt", blur);
  
  fboBlurOnePass->draw(0,0);
  
  shaderY->end();
  fboBlurTwoPass->end();
  
  //----------------------------------------------------------
 
  //ofSetColor(ofColor::white);
  fboBlurTwoPass->draw(0,0);*/

}


// Set shader's parameters
void shaderImageBlur::setShaderParams(){
  
  //shader->setUniform1f("time", ofGetElapsedTimef()*3);
  //plane->mapTexCoordsFromTexture(noiseRef->image->getTextureReference());
  //shader->setUniformTexture("tex0", *image->image, 1);
  
}

void shaderImageBlur::setMouseVals(float iMouseX, float iMouseY){

  mouseX = iMouseX;
  mouseY = iMouseY;
}