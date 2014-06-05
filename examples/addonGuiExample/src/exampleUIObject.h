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
  void        update(); // TODO: explain why not use idle();
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
  
  ofxUILabelToggle      *demoToggleBG;
  ofxUIMinimalSlider    *demoSliderR;
  ofxUIMinimalSlider    *demoSliderG;
  ofxUIMinimalSlider    *demoSliderB;
  ofxUILabelButton      *demoButtonInv;
  ofxUILabel            *demoLabel;
  
  float       sliderValR = 0;
  float       sliderValG = 0;
  float       sliderValB = 0;

private:
  
  bool        isVisible;
  
  int         windowBarHack;
  int         screenWidthHack;

};