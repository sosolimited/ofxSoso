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
  
  // Define some arbitrary sizing parameters
  int canvas_width = 300;
  int canvas_height = 600;

  int UIButtonWidth = 200;
  int UIButtonHeight = 20;
  int UIRadioWidth = 20;
 
  // Build the UI Canvas
  UI = new ofxUICanvas(0, 0, canvas_width, canvas_height);
    // Use zeros for translation here and setTrans on UIObject in App.

  // Disable app event callbacks because we do it from within render tree (ofxObject-style)
  UI->disableAppEventCallbacks();
  // TODO: REMOVE -- Disable mouse even callbacks because we have to flip the y-coords.
  UI->disableMouseEventCallbacks();

  // Add a lable to the top of the canvas
  UI->addLabel("exampleUIObject", OFX_UI_FONT_MEDIUM);
 
  // Define an arbitrary string vector for dummy radio button widget
  vector<string> radioNames;
  radioNames.push_back("radio_1");
  radioNames.push_back("radio_2");
  radioNames.push_back("radio_3");

  
  // Build and attach some dummy demostration objects
  UI->addSpacer();
  UI->addLabel("assorted dummy objects:");
  UI->addLabel("(not connected)", OFX_UI_FONT_SMALL);

  UI->addButton("button", false, UIButtonWidth, UIButtonHeight);
  UI->addToggle("toggle", false, UIButtonWidth, UIButtonHeight);
  UI->addIntSlider("int slider", 0, 10, 4, UIButtonWidth, UIButtonHeight);
  UI->addSlider("float slider", 0, 10, 6, UIButtonWidth, UIButtonHeight);
  UI->addRangeSlider("range slider", 0, 10, 2, 8, UIButtonWidth, UIButtonHeight);
  UI->addTextInput("text input", "text input", UIButtonWidth, UIButtonHeight);
  UI->addRadio("radio", radioNames, OFX_UI_ORIENTATION_HORIZONTAL);

  // Build some functional demostration objects
  UI->addSpacer();
  UI->addLabel("assorted functional objects:");
  UI->addLabel("(background color control)", OFX_UI_FONT_SMALL);
  
  demoToggleBG = new ofxUILabelToggle("toggle bg control", false, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM, true);
  UI->addWidgetDown(demoToggleBG);
  
  demoSliderR = new ofxUIMinimalSlider("red", 0, 255, sliderValR, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM);
  UI->addWidgetDown(demoSliderR);
  
  demoSliderG = new ofxUIMinimalSlider("green", 0, 255, sliderValG, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM);
  UI->addWidgetDown(demoSliderG);
  
  demoSliderB = new ofxUIMinimalSlider("blue", 0, 255, sliderValB, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM);
  UI->addWidgetDown(demoSliderB);
  
  demoButtonInv = new ofxUILabelButton("invert bg color", false, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_MEDIUM, true);
  UI->addWidgetDown(demoButtonInv);
  
  demoLabel = NULL;
  demoLabel = new ofxUILabel("updating label: ", OFX_UI_FONT_MEDIUM);
  UI->addWidgetDown(demoLabel);
  
}

void exampleUIObject::idle(float iTime)
{
//  UI->update(); // TODO: moved to update() for funsies
}

void exampleUIObject::update()
{
  
  float t = ofGetElapsedTimef();
  
  if(demoLabel)
    demoLabel->setLabel("updating label: "+ ofToString(t, 2));
  
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