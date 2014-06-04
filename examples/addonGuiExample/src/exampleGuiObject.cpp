//
//  exampleGuiObject.cpp
//  ofxSoso addonExample
//
//  Copyright 2014 Sosolimited. All rights reserved.
//  ofxObject encapsulation of openFramworks addon (ofxGui), so you can just add it as an object
//

# include <iostream>
# include "exampleGuiObject.h"

exampleGuiObject::exampleGuiObject()
{
  ofLogNotice("exampleGuiObject::exampleGuiObject()");
  
  // Initialize visibility
  bool isVisible = true;
  
  // TODO: Are these necessary?
  windowBarHack = 0;
  screenWidthHack = 0;
  
  // TODO: REMOVE -- Reassign mouse callback to flip y-coords.
  ofAddListener(ofEvents().mouseReleased, this, & exampleGuiObject::onMouseReleased);
  ofAddListener(ofEvents().mousePressed, this, & exampleGuiObject::onMousePressed);
  ofAddListener(ofEvents().mouseMoved, this, & exampleGuiObject::onMouseMoved);
  ofAddListener(ofEvents().mouseDragged, this, & exampleGuiObject::onMouseDragged);
  
}

exampleGuiObject::~exampleGuiObject()
{
  delete gui; // TODO: wa -- is this correct?
}

void exampleGuiObject::setup()
{
  ofLogNotice("exampleGuiObject::setup()");
  
  // Build panel
  gui = new ofxPanel();
  string gui_title = "exampleGuiObject";
  string xml_path = "settings.xml";
  gui->setup(gui_title, xml_path, 0, 0);
    // Use zeros for translation here and setTrans on guiObject in App.
  
  // Build components
  button = new ofxButton();
  toggle = new ofxToggle();
  slider = new ofxSlider<int>();
  label = new ofxLabel();
  
  // Add components to panel
  gui->add(button->setup("button", false));
  gui->add(toggle->setup("toggle", false));
  gui->add(slider->setup("slider<int>", 5, 0, 10));
  gui->add(label->setup("label", "label_value"));
  
}

void exampleGuiObject::idle(float iTime)
{
  //  gui->update(); // TODO: wa -- ofxPanel has no update();
}

void exampleGuiObject::render()
{
  if (isVisible)
  {
    ofPushMatrix();
    
    // To draw correctly in soso universe, we gotta flip it.
    ofScale(1, -1, 1);
    
    gui->draw();
    
    ofPopMatrix();
  }
}

void exampleGuiObject::exit()
{
  //  gui->exit(); // TODO: wa -- ofxPanel has no exit();
}

// TODO: wa -- needs Y-flipping
void exampleGuiObject::onMouseReleased(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mouseReleased(iArgs);
  
}

void exampleGuiObject::onMousePressed(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mousePressed(iArgs);
  
}

void exampleGuiObject::onMouseMoved(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mouseMoved(iArgs);
  
}

void exampleGuiObject::onMouseDragged(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mouseDragged(iArgs);
  
}