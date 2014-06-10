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
  // ofxUI.h addon is dependent on ofxXmlSettings.h addon
  #include "ofxXmlSettings.h"

class exampleUIObject : public ofxObject {
  
public:
  exampleUIObject();
  ~exampleUIObject();
  
  void        setup();
  void        idle(float iTime);
  void        update();
  
  void        render();
  
  void        exit();
  
  void        onMouseReleased(ofMouseEventArgs & iArgs);
  void        onMousePressed(ofMouseEventArgs & iArgs);
  void        onMouseMoved(ofMouseEventArgs & iArgs);
  void        onMouseDragged(ofMouseEventArgs & iArgs);
    
  void        toggleVisible() { isVisible = !isVisible; }
  void        setVisible(bool iIsVisible) { isVisible = iIsVisible; }
  
public:
    
  ofxUICanvas   *UI;
  
  ofxUILabelToggle      *demoToggleBG;
  ofxUIMinimalSlider    *demoSliderR;
  ofxUIMinimalSlider    *demoSliderG;
  ofxUIMinimalSlider    *demoSliderB;
  ofxUIMinimalSlider    *demoSliderH;
  ofxUIMinimalSlider    *demoSliderS;
  ofxUIMinimalSlider    *demoSliderL;
  ofxUILabelButton      *demoButtonInv;
  ofxUILabel            *demoLabel;
  ofxUI2DPad            *demoPos;
  ofxUIMinimalSlider    *demoCircleRes;
  ofxUIMinimalSlider    *demoCircleScale;
  ofxUIToggle           *demoToggle1;
  
private:
  
  bool        isVisible;
  
  int         windowBarHack;
  int         screenWidthHack;

  //Slider Values and default positions
  float       sliderValR = 100;
  float       sliderValG = 150;
  float       sliderValB = 200;
  int         sliderValRes = 5;
  float       sliderValScale = 100;

};