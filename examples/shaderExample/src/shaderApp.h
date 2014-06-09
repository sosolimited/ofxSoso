

#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "ofxSosoRenderer.h"
#include "ofxRectangleObject.h"
#include "ofxTextObject.h"

// Custom shader classes
#include "shaderWavyPlane.h"
#include "shaderNoisyPlane.h"
#include "shaderColorRect.h"
#include "shaderImageMask.h"
#include "shaderImageBlur.h"




class shaderApp : public ofBaseApp{
  
public:
  
  void setup();
  void update();
  void draw();
  
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  
  // helper method
  void buildInstructionText();
  
public:
  
  ofxSosoTrueTypeFont *font16;  // For instruction text
  vector<ofxTextObject*> instructions;
  
  ofxScene						*scene;
  
  shaderWavyPlane  *wavyPlane;
  shaderNoisyPlane *noisyPlane;
  shaderColorRect  *colorRect;
  shaderImageMask *imageMask;
  shaderImageBlur *imageBlur;
  
  vector<ofxObject *> shaderObjects;
  
  bool shaderEnable = true;
  
  ofShader shaderBlurX;
  ofShader shaderBlurY;
  
  ofFbo fboBlurOnePass;
  ofFbo fboBlurTwoPass;
  
  ofImage image;
 
};

