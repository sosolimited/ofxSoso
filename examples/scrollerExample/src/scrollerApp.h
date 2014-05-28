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
  
  
public:
  ofxScene						*scene;
  
	ofxSosoTrueTypeFont				*font16,
  *font64;
  
	ofxTextObject				    *text,
  *circlesLabel,
  *lineLabel,
  *polygonLabel;
	
	ofxLetterTextObject				*letterText;
  
  ofxImageObject					*image;
	ofImage							*polyTex;
	ofxDynamicPolygonObject			*dynamicPolygon;
  
  ofxObject						*circleRoot,
  *lineRoot;
	vector<ofxCircleObject *>		circles;
	vector<ofxLineSegmentObject *>	lines;
  
	ofxAnimation					*animation;
  
	ofxVideoPlayerObject			*movie;
	ofVideoPlayer					player;
  
  
};

