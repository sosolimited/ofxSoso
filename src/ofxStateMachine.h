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


class ofxStateMachine : public ofxObject{

public:
  ofxStateMachine();
  ~ofxStateMachine();
  
  void idle(float iTime);
  void update(float iTime);
  
  ofxState* addState(string iName);
  ofxState* getState(string iName);
  ofxStateTransition* addTransition(ofxState *iStartState, ofxState *iEndState, float iDur);
  ofxStateTransition* getTransition(ofxState *iStartState, ofxState *iEndState);
  
  bool gotoState(ofxState *iState);
  bool forceState(ofxState *iState);
  bool gotoState(string iName);
  void forceState(string iName);
  
  string getCurStateName();
  
  float getTransitionDuration();
  bool isTransitioning();
  float getTransitionClock();
  float getTransitionProgress();
  
  void enableTransitionBlocking(bool iEnable);
  
private:
  void startTransition(ofxStateTransition *iTransition);
  

private:
  vector<ofxState *> states;
  vector<ofxStateTransition *> transitions;
  
  ofxState *curState;
  ofxStateTransition *curTransition;
  
  float transitionClock;
  
  float timeP;
  float timeD;
  
  bool isTransitionBlocking;
  

  


};