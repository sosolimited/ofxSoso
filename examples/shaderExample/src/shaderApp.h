

#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "ofxSosoRenderer.h"
#include "ofxRectangleObject.h"
#include "ofxTextObject.h"
#include "ofxWavyPlane.h"
#include "ofxNoisyPlane.h"
#include "ofxColorRect.h"
#include "ofxImageMask.h"
#include "ofxImageBlur.h"




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
  
  
public:
  ofxScene						*scene;
  
  ofxWavyPlane  *wavyPlane;
  ofxNoisyPlane *noisyPlane;
  ofxColorRect  *rect;
  ofxImageMask *imageMerge;
  ofxImageBlur *imageBlur;
  
  
  //ofxMe
  ofxTextObject *instructions;
  
  bool shaderEnable = true;
 
};

