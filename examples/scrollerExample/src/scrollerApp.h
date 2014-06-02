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

class scrollerApp : public ofBaseApp{
  
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
  
  // Helper method to build instruction text
  void buildInstructionText();
  
  
public:
  
  // ofxSoso scene
  ofxScene  *scene;
  
  // Scroller objects
  ofxScroller *horizontalScroller;
  ofxScroller *verticalScroller;
  
	ofxSosoTrueTypeFont *font16;
  
  vector<ofxImageObject *> stars;
  vector<ofxTextObject*> instructions;
  ofxRectangleObject *textBackground;
  
  // Variables for calculating mouse drag velocity
  float time = 0;

  float prevMouseX = 0;
  float prevMouseY = 0;
  
  float prevDragTime = 0;
  
  float mouseYVelocity = 0.0f;
  float mouseXVelocity = 0.0f;
  
};

