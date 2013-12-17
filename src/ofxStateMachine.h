//
//  ofxStateMachine.h
//  Schneider
//
//  Created by Eric Gunther on 12/17/13.
//
//

#pragma once

#include "ofxObject.h"

class ofxState {

public:
  ofxState(string iName){ name = iName; }
  ~ofxState();
public:
  string name;

};


class ofxStateTransition {

public:
  ofxStateTransition(ofxState *iStartState, ofxState *iEndState, float iDur);
  ~ofxStateTransition();
public:
  ofxState *startState;
  ofxState *endState;
  float duration;

};


class ofxStateMachine {

public:
  ofxStateMachine();
  ~ofxStateMachine();
  
  void update(float iTime);
  
  ofxState* addState(string iName);
  ofxState* getState(string iName);
  ofxStateTransition* addTransition(ofxState *iStartState, ofxState *iEndState, float iDur);

private:
  vector<ofxState *> states;
  vector<ofxStateTransition *> transitions;
  
  


};