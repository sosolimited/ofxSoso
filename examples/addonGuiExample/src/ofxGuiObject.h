//
//  ofxGuiObject.h
//  ofxSoso addonExample
//
//  Created by Wade Aaron on 5/30/14.
//  Copyright 2014 Sosolimited. All rights reserved.
//  ofxObject encapsulation of ofxGui, so you can just add it as an object
//

# pragma once

#include "ofxObject.h"
#include "ofxGui.h"

#include "ofAppRunner.h" // TODO: wa -- hacked in for now. should this come in from somewhere else? ofxGuiObject/ofxObject?

class ofxGuiObject : public ofxObject {
  
public:
//  ofxGuiObject(float iWidth, float iHeight);
  ofxGuiObject();
  ~ofxGuiObject();
  
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
  
  ofxPanel    *gui;
  
private:
  
  bool        isVisible;
  
  int         windowBarHack;
  int         screenWidthHack;
  
};