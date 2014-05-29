#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "ofxSosoRenderer.h"
#include "ofxTextObject.h"
#include "ofxGridSystem.h"
#include "ofxCircleObject.h"
#include "ofxArcObject.h"
#include "ofxLineSegmentObject.h"
#include "ofxPolygonObject.h"

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
  ofxScene                  *scene;
  
  ofxSosoTrueTypeFont				*font16;
  ofxTextObject             *counterDisplay;
  int                       numObjectsToCreate;

  // counters for each type of object we wish to test destructor for.
  int                       numCreatedTextObjects;
  int                       numCreatedGridSystems;
  int                       numCreatedCircleObjects;
  int                       numCreatedArcObjects;
  int                       numCreatedLineSegments;
  int                       numCreatedPolygonObjects;
};

