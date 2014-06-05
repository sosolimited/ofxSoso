#pragma once

#include "ofMain.h"
#include "ofxScene.h"

#include "ofxImageObject.h"
#include "ofxFboObject.h"


class emptyApp : public ofBaseApp{
  
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
  ofxImageObject      *image;
  ofImage             ofIm;
 
  
  ofxScene            *fboScene;
  ofxFboObject        *fbo;
};

