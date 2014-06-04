//
//  exampleUIObject.cpp
//  ofxSoso addonExample
//
//  Copyright 2014 Sosolimited. All rights reserved.
//  ofxObject encapsulation of openFramworks addon (ofxUI), so you can just add it as an object
//

# include <iostream>
# include "exampleUIObject.h"

exampleUIObject::exampleUIObject()
{
  ofLogNotice("exampleUIObject::exampleUIObject()");
  
  // Initialize visibility
  bool isVisible = true;
  
  // TODO: Are these necessary?
  windowBarHack = 0;
  screenWidthHack = 0;
  
  // TODO: REMOVE -- Reassign mouse callback to flip y-coords.
  ofAddListener(ofEvents().mouseReleased, this, &exampleUIObject::onMouseReleased);
  ofAddListener(ofEvents().mousePressed, this, &exampleUIObject::onMousePressed);
  ofAddListener(ofEvents().mouseMoved, this, &exampleUIObject::onMouseMoved);
  ofAddListener(ofEvents().mouseDragged, this, &exampleUIObject::onMouseDragged);
  
}

exampleUIObject::~exampleUIObject()
{

}

void exampleUIObject::setup()
{
  ofLogNotice("exampleUIObject::setup()");
  
  int canvas_width = ofGetWindowWidth()/2;
  int canvas_height = ofGetWindowHeight();
  
  // Build the UI Canvas
  UI = new ofxUICanvas(0, 0, canvas_width, canvas_height);
    // Use zeros for translation here and setTrans on UIObject in App.

  // Disable app event callbacks because we do it from within render tree (ofxObject-style)
  UI->disableAppEventCallbacks();
  // TODO: REMOVE -- Disable mouse even callbacks because we have to flip the y-coords.
  UI->disableMouseEventCallbacks();

  int UIButtonWidth = 200;
  int UIButtonHeight = 20;
  int UIRadioWidth = 20;
  
  UISliderVal = 5;
  
  string UI_title = "exampleUIObject";
  
  UI->addWidgetDown(new ofxUILabel(UI_title, OFX_UI_FONT_MEDIUM));
  UI->addSpacer();
  UI->addWidgetDown(new ofxUILabel("label", OFX_UI_FONT_MEDIUM));
  UI->addTextInput("text input", "text input", UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM);
  UI->addWidgetDown(new ofxUIButton("button", false, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM));
  UI->addWidgetDown(new ofxUIToggle("toggle", false, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM));
  UI->addIntSlider("int slider", 0, 10, UISliderVal, UIButtonWidth, UIButtonHeight);
  UI->addSlider("float slider", 0, 10, UISliderVal, UIButtonWidth, UIButtonHeight);
  UI->addRangeSlider("range slider", 0, 10, 2, 7, UIButtonWidth, UIButtonHeight);
  UI->addSpacer();
  UI->addWidgetDown(new ofxUILabelButton("labelButton", false, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM));
  UI->addWidgetDown(new ofxUILabelToggle("labelToggle", false, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM));
  UI->addMinimalSlider("minimal slider", 0, 10, UISliderVal, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM);
  
  vector<string> modeNames;
  modeNames.push_back("eric");
  modeNames.push_back("john");
  modeNames.push_back("justin");
  
  UI->addWidgetDown(new ofxUIRadio("radio", modeNames, OFX_UI_ORIENTATION_HORIZONTAL, UIRadioWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM));
  
}

void exampleUIObject::idle(float iTime)
{
  UI->update();
}

void exampleUIObject::render()
{
  if (isVisible){
    ofPushMatrix();
    
    // TODO: REMOVE -- Flip y-coords to draw correctly in soso universe.
    ofScale(1, -1, 1);
    
    UI->draw();
    
    ofPopMatrix();
  }
}

void exampleUIObject::exit()
{
  UI->exit();
}

void exampleUIObject::onMouseReleased(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  UI->mouseReleased(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack, iArgs.button);
}

void exampleUIObject::onMousePressed(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  UI->mousePressed(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack, iArgs.button);
}

void exampleUIObject::onMouseMoved(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  UI->mouseMoved(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack);
}

void exampleUIObject::onMouseDragged(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  UI->mouseDragged(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack, iArgs.button);
}