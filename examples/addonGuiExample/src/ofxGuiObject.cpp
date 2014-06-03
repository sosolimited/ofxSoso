//
//  ofxGuiObject.cpp
//  ofxSoso addonExample
//
//  Created by Wade Aaron on 5/30/14.
//  Copyright 2014 Sosolimited. All rights reserved.
//  ofxObject encapsulation of ofxGui, so you can just add it as an object
//

#include "ofxGuiObject.h"

# include <iostream>
# include "ofxGuiObject.h"

//ofxGuiObject::ofxGuiObject(float iWidth, float iHeight)
ofxGuiObject::ofxGuiObject()
// TODO: wa -- improve this appropriately
{
  ofLogNotice("ofxGuiObject::ofxGuiObject()");
  
  bool isVisible = true;
  
  gui = new ofxPanel();
  
  ofAddListener(ofEvents().mouseReleased, this, & ofxGuiObject::onMouseReleased);
  ofAddListener(ofEvents().mousePressed, this, & ofxGuiObject::onMousePressed);
  ofAddListener(ofEvents().mouseMoved, this, & ofxGuiObject::onMouseMoved);
  ofAddListener(ofEvents().mouseDragged, this, & ofxGuiObject::onMouseDragged);
  
  windowBarHack = 0;
  screenWidthHack = 0;
  
}

ofxGuiObject::~ofxGuiObject()
{
  delete gui; // TODO: wa -- is this correct?
}

void ofxGuiObject::idle(float iTime)
{
  //  gui->update(); // TODO: wa -- ofxPanel has no update();
}

void ofxGuiObject::render()
{
  if (isVisible)
  {
    ofPushMatrix(); // TODO: wa -- what's this?
    
    // To draw correctly in soso universe, we gotta flip it.
    ofScale(1, -1, 1);
    
    gui->draw();
  }
}

void ofxGuiObject::exit()
{
  //  gui->exit(); // TODO: wa -- ofxPanel has no exit();
}

// TODO: wa -- needs Y-flipping
void ofxGuiObject::onMouseReleased(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mouseReleased(iArgs);
  
}

void ofxGuiObject::onMousePressed(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mousePressed(iArgs);
  
}

void ofxGuiObject::onMouseMoved(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mouseMoved(iArgs);
  
}

void ofxGuiObject::onMouseDragged(ofMouseEventArgs & iArgs)
{
  if (ofGetHeight() != intendedWindowHeight)
    windowBarHack = (intendedWindowHeight - ofGetHeight() )/2.0;
  
  if (ofGetWidth() != intendedWindowWidth)
    screenWidthHack = (intendedWindowWidth - ofGetWidth() )/2.0;
  
  gui->mouseDragged(iArgs);
  
}