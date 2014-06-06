#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "ofxSosoRenderer.h"

#include "ofxGridSystem.h"
#include "myGrid.h"
#include "ofxTextObject.h"
#include "ofxImageObject.h"
#include "ofxRectangleObject.h"

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
  
  void buildLayout(ofxGridSystem* grid, ofxObject* iRoot);
  
public:
  ofxScene						*scene;
  ofxObject           *root;
  
  // Specifying the grid as an object lets us use some nice
  // shorthand conventions when requesting layout values from the grid.
  myGrid              grid;
 
};

