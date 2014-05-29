#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "ofxRectangleObject.h"
#include "ofxSosoRenderer.h"
#include "ofxImageObject.h"
#include "ofxTextObject.h"
#include "ofxLetterTextObject.h"
#include "ofxCircleObject.h"
#include "ofxAnimation.h"
#include "ofxVideoPlayerObject.h"
#include "ofxLineSegmentObject.h"
#include "ofxDynamicPolygonObject.h"

#include "ofxScroller.h"

class testApp : public ofBaseApp{
  
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
  
  void buildInstructionText();
  
  
public:
  ofxScene						*scene;
  
  ofxScroller *horizontalScroller;
  ofxScroller *verticalScroller;
  
	ofxSosoTrueTypeFont				*font16,
  *font64;
  
  vector<ofxImageObject *> stars;
  vector<ofxTextObject*> instructions;
  ofxRectangleObject *textBackground;

  
	scrollDirection horizontalDragDirection = OF_SCROLL_FORWARD;
  scrollDirection verticalDragDirection = OF_SCROLL_FORWARD;

  
  float time = 0;
  
  float dragVelocity = 0.2f;
  
  float prevMouseX = 0;
  float prevMouseY = 0;
  
  
};

