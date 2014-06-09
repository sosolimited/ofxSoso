//
//  shaderImageBlur.cpp
//  shaderExample
//
//  Created by Alex Olivier on 6/6/14.
//
//
// Based on openFrameworks shader tutorials

// Blur an image using shaders
// This is a more complex example that uses multiple shader passes
// and FBOs to store the intermediate results

// The first shader pass will blur the image in the x direction
// The second shader pass will blur the image in the y direction

#include "shaderImageBlur.h"

shaderImageBlur::shaderImageBlur(string iFilename){
  
  // Create an ofImage (image to be blurred)
  image = new ofImage();
  image->loadImage(iFilename);
  image->getTextureReference().texData.bFlipTexture = true; // Flip it to fit soso universe
  
  // Create two FBO objects
  // The first FBO will store intermediate results (blur pass 1)
  // Final blur will be stored in the second fbo (blur pass 2)
  fboBlurOnePass = new ofxFboObject(image->getWidth(), image->getHeight());
  fboBlurTwoPass = new ofxFboObject(image->getWidth(), image->getHeight());
  
  // Add the final fbo as a child
  addChild(fboBlurTwoPass);
  
  // Create two shaders, one to blur in x direction
  // and the other to blur in y direction
  shaderBlurX = new ofShader();
  shaderBlurY = new ofShader();

  // Load x and y blur shaders
  shaderBlurX->load("shaders/image_blur/shaderBlurX");
  shaderBlurY->load("shaders/image_blur/shaderBlurY");

}

// Destroy everything
shaderImageBlur::~shaderImageBlur(){
  
  delete image;
  delete shaderBlurX;
  delete shaderBlurY;
  delete fboBlurOnePass;
  delete fboBlurTwoPass;
  
}


// We'll do our shaders in the render method
void shaderImageBlur::render(){

  // Calculate blur amount based on mouse x value
  blur = ofMap(mouseX, 0, appWidth, 0, 10, true);
  
  
  if (shaderEnabled){
    
    // SHADER PASS 1 ----------------------------------------------------
    // We'll store the results of our first shader pass in fboBlurOnePass
    
    // Begin the fbo
    fboBlurOnePass->fbo->begin();
    
    // Clear out old texture memory
    ofClear(255,255,255, 0);
    
    // Begin shader to blur in the x direction
    shaderBlurX->begin();
    
    // Pass in blur variable to shader
    shaderBlurX->setUniform1f("blurAmnt", blur);
    
    // Draw the image
    image->draw(0,0);
    
    // End Shader and FBO
    shaderBlurX->end();
    fboBlurOnePass->fbo->end();
    
    
    // SHADER PASS 2 ----------------------------------------------------
    // We'll store the results of our first shader pass in fboBlurTwoPass
    
    // Begin our second fbo
    fboBlurTwoPass->fbo->begin();
    
    // Clear out old texture memory
    ofClear(255,255,255, 0);
    
    // Start the second shader
    shaderBlurY->begin();
    
    // Pass in blur variable to shader
    shaderBlurY->setUniform1f("blurAmnt", blur);
    
    // Draw the results from our first pass
    fboBlurOnePass->render();
    
    // End shader and fbo
    shaderBlurY->end();
    fboBlurTwoPass->fbo->end();
    
    // Draw the final result
    fboBlurTwoPass->render();
    
  }else{
    
    // If shader is disable, just draw the image
    image->draw(0,0);
    
  }
}


// Pass in mouse x and y values
void shaderImageBlur::setMouseVals(float iMouseX, float iMouseY){

  mouseX = iMouseX;
  mouseY = iMouseY;
}

// Pass in app dimensions
void shaderImageBlur::setAppDimensions(float iW, float iH){

  appWidth = iW;
  appHeight = iH;
}