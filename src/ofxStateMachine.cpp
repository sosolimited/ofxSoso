//
//  ofxStateMachine.cpp
//  Schneider
//
//  Created by Eric Gunther on 12/17/13.
//
//

#include "ofxStateMachine.h"

ofxStateTransition::ofxStateTransition(ofxState *iStartState, ofxState *iEndState, float iDur)
{
  startState = iStartState;
  endState = iEndState;
  duration = iDur;
  
}


ofxStateMachine::ofxStateMachine()
{
  
}

ofxStateMachine::~ofxStateMachine(){}

void ofxStateMachine::update(float iTime)
{
  
}


ofxState* ofxStateMachine::addState(string iName)
{
  ofxState *state = getState(iName);
  
  if(state){
    return state;
  }else{
    state = new ofxState(iName);
    states.push_back(state);
    return state;
  }
}

ofxState* ofxStateMachine::getState(string iName)
{
  for(auto state : states){
    if(state->name.compare(iName)==0){
      return state;
    }
  }
  return NULL;
}


ofxStateTransition* ofxStateMachine::addTransition(ofxState *iStartState, ofxState *iEndState, float iDur)
{
  ofxStateTransition *t = new ofxStateTransition(iStartState, iEndState, iDur);
  transitions.push_back(t);
  
  return t;  
}

