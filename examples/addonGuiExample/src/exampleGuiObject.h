//
//  exampleGuiObject.h
//  ofxSoso addonExample
//
//  Copyright 2014 Sosolimited. All rights reserved.
//  ofxObject encapsulation of openFramworks addon (ofxGui), so you can just add it as an object

//

# pragma once

#include "ofxObject.h"
#include "ofxGui.h"

#include "ofAppRunner.h" // TODO: wa -- hacked in for now. should this come in from somewhere else? ofxGuiObject/ofxObject?

class exampleGuiObject : public ofxObject {
  
public:
  exampleGuiObject();
  ~exampleGuiObject();
  
  void        setup();
  void        idle(float iTime);
  void        render();
  
  void        exit();
  
  void        onMouseReleased(ofMouseEventArgs & iArgs);
  void        onMousePressed(ofMouseEventArgs & iArgs);
  void        onMouseMoved(ofMouseEventArgs & iArgs);
  void        onMouseDragged(ofMouseEventArgs & iArgs);
  
  void        setIntendedWindowSize(int iW, int iH) { intendedWindowWidth = iW, intendedWindowHeight = iH; }
  
  void        toggleVisible() { isVisible = !isVisible; }
  void        setVisible(bool iIsVisible) { isVisible = iIsVisible; }
  
public:
  
  int         intendedWindowWidth;
  int         intendedWindowHeight;
  
  // TODO: Parts Comment
  ofxPanel        *gui;
  ofxButton       *button;
  ofxToggle       *toggle;
  ofxSlider<int>  *slider;
  ofxLabel        *label;
  
private:
  
  bool        isVisible;
  
  int          windowBarHack;
  int          screenWidthHack;
  
};