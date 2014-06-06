//
//  wavyPlane.cpp
//  shaderExample
//
//  Created by Sosolimited on 6/6/14.
//
//

#include "ofxNoisyPlane.h"

ofxNoisyPlane::ofxNoisyPlane(int iWidth, int iHeight, int iCols, int iRows){
  
  plane = new ofPlanePrimitive();
  plane->set(iWidth, iHeight, iCols, iRows, OF_PRIMITIVE_TRIANGLES);
  
  noiseRef = new ofImage();
  //noiseRef = new ofxImageObject("white_square.png");
  //noiseRef->image->setImageType(OF_IMAGE_GRAYSCALE);
  noiseRef->allocate(30, 30, OF_IMAGE_GRAYSCALE);
  plane->mapTexCoordsFromTexture(noiseRef->getTextureReference());
  //noiseRef->setCentered(true);
  //addChild(noiseRef);
}

// Destroy everything
ofxNoisyPlane::~ofxNoisyPlane(){
  
  delete plane;
  delete noiseRef;
  
}

void ofxNoisyPlane::generateNoise(){
  
  noiseRef->getTextureReference().bind();
  
  //float noiseScale = ofMap(mouseX, 0, ofGetWidth(), 0, 0.1);
  float noiseScale = 0.05;
  float noiseVel = ofGetElapsedTimef();
  
  unsigned char * pixels = noiseRef->getPixels();
  int w = noiseRef->getWidth();
  int h = noiseRef->getHeight();
  for(int y=0; y<h; y++) {
    for(int x=0; x<w; x++) {
      int i = y * w + x;
      float noiseVelue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
      pixels[i] = 255 * noiseVelue;
    }
  }
  noiseRef->update();

}

void ofxNoisyPlane::idle(float iTime){
  
  generateNoise();
  
}

void ofxNoisyPlane::render(){
  
  
  plane->drawWireframe();
  
}

// Set shader's parameters
void ofxNoisyPlane::setShaderParams(){
  
  //shader->setUniform1f("time", ofGetElapsedTimef()*3);
  //plane->mapTexCoordsFromTexture(noiseRef->image->getTextureReference());
  
}