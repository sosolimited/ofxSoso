#pragma once

#include "ofMain.h"
#include "ofxScene.h"
#include "ofxSosoRenderer.h"

#include "exampleUIObject.h"

class App : public ofBaseApp{
  
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
  
  void UIEvent(ofxUIEventArgs &e);
  
public:
  ofxScene						*scene;
  
  exampleUIObject     *UIObject;
  
  // Default Background Color and Related Values
  int defaultR = 100;
  int defaultG = 100;
  int defaultB = 100;
  
  int sliderR = 0;
  int sliderG = 0;
  int sliderB = 0;
  
  bool bgControl = 0;
  bool bgInvert = 0;
 
};

