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
  bool isVisible = true;

  // Initialize "hack" variables for window bar and screen width corrections in Soso universe
  windowBarHack = 0;
  screenWidthHack = 0;
  
  // Reassign mouse callback to flip y-coords
  ofAddListener(ofEvents().mouseReleased, this, &exampleUIObject::onMouseReleased);
  ofAddListener(ofEvents().mousePressed, this, &exampleUIObject::onMousePressed);
  ofAddListener(ofEvents().mouseMoved, this, &exampleUIObject::onMouseMoved);
  ofAddListener(ofEvents().mouseDragged, this, &exampleUIObject::onMouseDragged);
}

exampleUIObject::~exampleUIObject()
{
  delete UI;
  delete demoToggleBG;
  delete demoSliderR;
  delete demoSliderG;
  delete demoSliderB;
  delete demoButtonInv;
  delete demoLabel;
  
}

void exampleUIObject::setup()
{
  // Define some sizing parameters
  int canvas_width = 300;
  int canvas_height = 600;

  int UIButtonWidth = 200;
  int UIButtonHeight = 20;
  int UIRadioWidth = 20;
 
  // Define an arbitrary string vector for radio buttons
  vector<string> radioNames;
  radioNames.push_back("radio_1");
  radioNames.push_back("radio_2");
  radioNames.push_back("radio_3");

  // Build the UI Canvas
  UI = new ofxUICanvas(0, 0, canvas_width, canvas_height);
    // Use zeros for translation here and setTrans on exampleUIObject in App.

  // Disable app event callbacks because we do it from within render tree (ofxObject-style)
  UI->disableAppEventCallbacks();

  // Disable mouse event callbacks because we have to flip the y-coords.
  UI->disableMouseEventCallbacks();

  // Add a label to the top of the canvas
  UI->addLabel("exampleUIObject", OFX_UI_FONT_MEDIUM);
  
  // Build and attach some dummy demostration objects
  UI->addSpacer();
  //UI->addLabel("Assorted dummy objects");
  //UI->addLabel("(not connected to anything)", OFX_UI_FONT_SMALL);
  
  UI->addLabel("Position");
  demoPos = new ofxUI2DPad("position", ofPoint(0, ofGetWidth()), ofPoint(0, ofGetHeight()), ofPoint(ofGetWidth()/2,ofGetHeight()/2), 150, 150);
  UI->addWidgetDown(demoPos);
  
  UI->addLabel("Resolution");
  demoCircleRes = new ofxUIMinimalSlider("resolution", 3, 35, sliderValRes, UIButtonWidth, UIButtonHeight, 0, 0, OFX_UI_FONT_SMALL);
  UI->addWidgetDown(demoCircleRes);

//  UI->addButton("button", false, UIButtonHeight, UIButtonHeight);
//  UI->addToggle("toggle", false, UIButtonHeight, UIButtonHeight);
//  UI->addIntSlider("int slider", 0, 10, 4, UIButtonWidth, UIButtonHeight);
//  UI->addSlider("float slider", 0, 10, 6, UIButtonWidth, UIButtonHeight);
//  UI->addRangeSlider("range slider", 0, 10, 2, 8, UIButtonWidth, UIButtonHeight);
//  UI->addRadio("radio", radioNames, OFX_UI_ORIENTATION_HORIZONTAL, UIButtonHeight, UIButtonHeight, 0, 0, OFX_UI_FONT_SMALL);

  // Build some functional objects to connect to scene background color in the App
  UI->addSpacer();
  UI->addLabel("Assorted functional objects");
  UI->addLabel("(connected to background color)", OFX_UI_FONT_SMALL);
  
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
  
  // Build an updating label
  UI->addSpacer();
  demoLabel = NULL;
  demoLabel = new ofxUILabel("updating label", OFX_UI_FONT_MEDIUM);
  UI->addWidgetDown(demoLabel);
  
}

void exampleUIObject::idle(float iTime)
{

}

void exampleUIObject::update()
{
  
  // Do something to the updating label (so it updates)
  float t = ofGetElapsedTimef();
  if (demoLabel)
    demoLabel->setLabel("updating label: "+ ofToString(t, 2));
  
  // Update the UI (ofxUICanvas)
  UI->update();
  
}

void exampleUIObject::render()
{
  if (isVisible){
    ofPushMatrix();
    
    // Flip y-coords to draw correctly in Soso universe.
    ofScale(1, -1, 1);
    
    UI->draw();
    
    ofPopMatrix();
  }
}

void exampleUIObject::exit()
{
  UI->exit();
}

// Mouse callbacks that flip the y to bring interactions into soso universe.

void exampleUIObject::onMouseReleased(ofMouseEventArgs & iArgs)
{
  windowBarHack = (0 - ofGetHeight() )/2.0;
  screenWidthHack = (0 - ofGetWidth() )/2.0;
  
  UI->mouseReleased(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack, iArgs.button);

}

void exampleUIObject::onMousePressed(ofMouseEventArgs & iArgs)
{
  windowBarHack = (0 - ofGetHeight() )/2.0;
  screenWidthHack = (0 - ofGetWidth() )/2.0;

  UI->mousePressed(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack, iArgs.button);
}

void exampleUIObject::onMouseMoved(ofMouseEventArgs & iArgs)
{
  windowBarHack = (0 - ofGetHeight() )/2.0;
  screenWidthHack = (0 - ofGetWidth() )/2.0;

  UI->mouseMoved(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack);
}

void exampleUIObject::onMouseDragged(ofMouseEventArgs & iArgs)
{
  windowBarHack = (0 - ofGetHeight() )/2.0;
  screenWidthHack = (0 - ofGetWidth() )/2.0;

  UI->mouseDragged(iArgs.x - getTrans().x + screenWidthHack, iArgs.y+getTrans().y + windowBarHack, iArgs.button);
}