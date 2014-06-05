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
  curState = NULL;
  curTransition = NULL;
  
  isTransitionBlocking = false;
  isVerbose = false;
  transitionClock = 0;
  timeP = 0;  // Previous time.
  timeD = 0;  // Time difference.
}

ofxStateMachine::~ofxStateMachine(){
  delete curState;
  delete curTransition;
  for (auto state : states){
    delete state;
  }
  states.clear();
  for (auto transition : transitions){
    delete transition;
  }
  transitions.clear();
}

// If added as child object, it updates itself automatically.
void ofxStateMachine::idle(float iTime)
{
  update(iTime);
}

// If not added as child object, this must be called by owner.
void ofxStateMachine::update(float iTime)
{
  timeD = iTime - timeP;
  
  // Update state transitioning.
	if(curTransition){
		transitionClock += timeD;
		if(transitionClock >= curTransition->duration){
			transitionClock = 0;
      // Set state to end state of transition.
      curState = curTransition->endState;
      if(isVerbose) ofLogNotice("Current state is now "+curState->name+".");
      curTransition = NULL;
		}
	}
  
  timeP = iTime;
}

// ----------------------------------------------------------------
// State and transition manipulation methods.

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

ofxStateTransition* ofxStateMachine::getTransition(ofxState *iStartState, ofxState *iEndState)
{
  for(auto transition : transitions){
    if(transition->startState==iStartState && transition->endState==iEndState){
      return transition;
    }
  }
  return NULL;
}


ofxStateTransition* ofxStateMachine::addTransition(ofxState *iStartState, ofxState *iEndState, float iDur)
{
  ofxStateTransition *t = getTransition(iStartState, iEndState);
  
  if(t){
    // If it already exists, update duration.
    t->duration = iDur;
  }else{
    // If not, make a new one and add it.
    t = new ofxStateTransition(iStartState, iEndState, iDur);
    transitions.push_back(t);
  }
  
  return t;
}

ofxStateTransition* ofxStateMachine::addTransition(string iStartState, string iEndState, float iDur)
{
  // These return the state if it already exists, and add it if it doesn't yet.
  ofxState *start = addState(iStartState);
  ofxState *end  = addState(iEndState);
  
  return addTransition(start, end, iDur);
}

// For debugging, it will print out all state transitions when set to true.
void ofxStateMachine::setVerbose(bool iEnable)
{
  isVerbose = iEnable;
}

// ---------------------------------------------------------
// State control methods.

// Note: The current state is held until a transition to the next state is finished.
// In other words, during transitions, the current state is still the start state of the transition.
// Then, when the transition is complete, the current state becomes the end state of the transition.
bool ofxStateMachine::gotoState(ofxState *iState)
{
  // Init state for the first time.
  if(curState == NULL){
    curState = iState;
    if(isVerbose) ofLogNotice("State inited to "+curState->name+".");
  }else{
    // If transition blocking enabled, don't allow any transitions if already in transition.
    if((isTransitionBlocking && curTransition==NULL) || !isTransitionBlocking){
      // Look through transitions for matching start and end.
      for(auto t : transitions){
        if(t->startState==curState && t->endState==iState){
          if(isVerbose) ofLogNotice("Transitioning from "+t->startState->name+" to "+t->endState->name+"...");
          startTransition(t);
          return true;
        }
      }
      ofLogNotice("State transition from "+curState->name+" to "+iState->name+" does not exist.");
      return false;
    }else{
      ofLogNotice("State already in transition....blocking request.");
      return false;
    }
  }
}

bool ofxStateMachine::forceState(ofxState *iState)
{
  curState = iState;
  curTransition = NULL;
  transitionClock = 0;
}

bool ofxStateMachine::gotoState(string iName)
{
  ofxState *state = getState(iName);
  
  if(state){
    return gotoState(state);
  }else{
    ofLogNotice("State "+iName+" does not exist.");
    return false;
  }
}

void ofxStateMachine::forceState(string iName)
{
  ofxState *state = getState(iName);
  if(state) forceState(state);
}

string ofxStateMachine::getCurState()
{
  if(curState) return curState->name;
  else return "NULL";
}

// --------------------------------------------------------------
// Private method for starting transitions.
void ofxStateMachine::startTransition(ofxStateTransition *iTransition)
{
  curTransition = iTransition;
  transitionClock = 0;
}


// --------------------------------------------------------------
// Public access to transition variables.

// When transition blocking is enabled, no state transitions are allowed
// while machine is in transition.
void ofxStateMachine::enableTransitionBlocking(bool iEnable)
{
  isTransitionBlocking = iEnable;
}

float ofxStateMachine::getTransitionDuration()
{
  if (curTransition) return curTransition->duration;
  else return 0;
}

bool ofxStateMachine::isTransitioning()
{
  if(curTransition) return true;
  else return false;
}

float ofxStateMachine::getTransitionClock()
{
  return transitionClock;
}

// Returns normalized value of transition progress.
float ofxStateMachine::getTransitionProgress()
{
  if(curTransition)
		return transitionClock/curTransition->duration;
	else
		return 1.0;
  
}
