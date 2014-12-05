//
//  wavyPlane.cpp
//  shaderExample
//
//  Created by Alex Olivier on 6/6/14.
//
//
// Based on openFrameworks shader tutorials

#include "shaderNoisyPlane.h"

shaderNoisyPlane::shaderNoisyPlane(int iWidth, int iHeight, int iCols, int iRows){
  
  plane = new ofPlanePrimitive();
  plane->set(iWidth, iHeight, iCols, iRows, OF_PRIMITIVE_TRIANGLES);
  
  // If we want to give a shader knowledge of more than one
  // pixel at a time, we need to pass it a texture
  
  // Here, we're creating a grayscale ofImage to use as a noise map
  noiseRef = new ofImage();
  noiseRef->allocate(30, 30, OF_IMAGE_GRAYSCALE);
  
  // We set this image as our plane's texture
  plane->mapTexCoordsFromTexture(noiseRef->getTexture());

}

// Destroy everything
shaderNoisyPlane::~shaderNoisyPlane(){
  
  delete plane;
  delete noiseRef;
  
}

// This function generates noise values
// Noise is represented as a 0-255 grayscale value
// stored in as a pixel value
void shaderNoisyPlane::generateNoise(){
  
  // Scale noise based on mouse x value
  float noiseScale = ofMap(mouseX, 0, appWidth, 0, 0.1);

  // Set noise velocity based on time
  float noiseVel = ofGetElapsedTimef();
  
  // Go through noiseRef pixel array
  unsigned char * pixels = noiseRef->getPixels();
  int w = noiseRef->getWidth();
  int h = noiseRef->getHeight();
  for(int y=0; y<h; y++) {
    for(int x=0; x<w; x++) {
      int i = y * w + x;
      
      // Use ofNoise to generate 3-D Perlin noise
      float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
      pixels[i] = 255 * noiseValue;
    }
  }
  
  // Update our ofImage
  noiseRef->update();

}

void shaderNoisyPlane::idle(float iTime){

  // bind our texture. in our shader this will now be tex0 by default
  // so we can just go ahead and access it there.
  noiseRef->getTexture().bind();
  
  // We can also pass a texture by calling
  // shader->setUniformTexture(string varName, ofTexture tex, int texNumber);
  
  generateNoise();
  
}

// Implement render to draw plane's wireframe
void shaderNoisyPlane::render(){
  
  plane->drawWireframe();
  
}

// Set shader's parameters
void shaderNoisyPlane::setShaderParams(){
  
  //shader->setUniform1f("time", ofGetElapsedTimef()*3);
  //plane->mapTexCoordsFromTexture(noiseRef->image->getTextureReference());
  
}

void shaderNoisyPlane::setMouseVals(float iMouseX, float iMouseY){
  
  mouseX = iMouseX;
  mouseY = iMouseY;
}

void shaderNoisyPlane::setAppDimensions(float iW, float iH){
  
  appWidth = iW;
  appHeight = iH;
}