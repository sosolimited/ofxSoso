//
//  exampleUIObject.h
//  ofxSoso addonExample
//
//  Copyright 2014 Sosolimited. All rights reserved.
//  ofxObject encapsulation of openFramworks addon (ofxUI), so you can just add it as an object
//

# pragma once

#include "ofxObject.h"
#include "ofxUI.h"
  // Note: ofxUI.h is dependent on ofxXmlSettings.h
  #include "ofxXmlSettings.h"

class exampleUIObject : public ofxObject {
  
public:
  exampleUIObject();
  ~exampleUIObject();
  
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
  
  // TODO: Add parts and comment
  ofxUICanvas   *UI;
  
  float         UISliderVal;
  
private:
  
  bool        isVisible;
  
  int         windowBarHack;
  int         screenWidthHack;
  
};