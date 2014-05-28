// ofxScrollerObject
// Created May 8, 2014
//

//
//  scScrollList.cpp
//  Schneider
//
//  Created by Eric Gunther on 11/7/13.
//
//

#include "ofxScroller.h"

//----------------------------------------------------------------------------------
// Class ofxScrollTransform
ofxScrollTransform::ofxScrollTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, float iStartVal, float iEndVal)
{
  interpolation = iInterpolation;
  transform = iTransform;
  scrollRange[0] = iStartScroll;
  scrollRange[1] = iEndScroll;
  valueRange[0].set(iStartVal, 0, 0);
  valueRange[1].set(iEndVal, 0, 0);
}


ofxScrollTransform::ofxScrollTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, ofVec3f iStartVal, ofVec3f iEndVal)
{
  interpolation = iInterpolation;
  transform = iTransform;
  scrollRange[0] = iStartScroll;
  scrollRange[1] = iEndScroll;
  valueRange[0] = iStartVal;
  valueRange[1] = iEndVal;
}


//-----------------------------------------------------------------------------------
// Class ofxScrollObject
ofxScrollObject::ofxScrollObject(ofxObject *iObject) {
  object = iObject;
}


void ofxScrollObject::addTransform(ofxScrollTransform *iTransform, bool isVertical) {
  if (isVertical)
    verticalTransforms.push_back(iTransform);
  else
    horizontalTransforms.push_back(iTransform);
}


void ofxScrollObject::addTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, float iStartVal, float iEndVal, bool isVertical) {
  addTransform(new ofxScrollTransform(iTransform, iStartScroll, iEndScroll, iInterpolation, iStartVal, iEndVal), isVertical);
}


void ofxScrollObject::addTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, ofVec3f iStartVal, ofVec3f iEndVal, bool isVertical) {
  addTransform(new ofxScrollTransform(iTransform, iStartScroll, iEndScroll, iInterpolation, iStartVal, iEndVal), isVertical);
}


// Hack to make two transforms of the same type work.
// If there is a transform of the same type (eg OF_TRANSLATE) with a range that is closer
// to the current scrollPosition, then return true.
bool ofxScrollObject::isTrumped(ofxScrollTransform *iTransform, float iScrollPosition, bool isVertical) {
  // If we're in the range of the given transform, then nothing trumps it!
  if(ofInRange(iScrollPosition, iTransform->scrollRange[0], iTransform->scrollRange[1])){
    return false;
  }
  
  vector<ofxScrollTransform *> transforms;
  if (isVertical)
    transforms = verticalTransforms;
  else
    transforms = horizontalTransforms;
  
  for(auto t : transforms){
    // If it's not the one we're interested in.
    if(t != iTransform){
      // If they're the same transform type.
      if(t->transform == iTransform->transform){
        // If it's in the range.
        if(ofInRange(iScrollPosition, t->scrollRange[0], t->scrollRange[1])){
          return true;
        }else{ // If it's closer to the range
          if(fabs(iScrollPosition - t->scrollRange[0]) < fabs(iScrollPosition - iTransform->scrollRange[0])
             // majapw -> Commenting out this section fixes some issues with header transforms and does not seem to affect
             // anything else.  It would beneficial to have another pair of eyes here though as I can't adequately explain why.
             /* ||
              fabs(iScrollPosition - t->scrollRange[1]) < fabs(iScrollPosition - iTransform->scrollRange[0]) ||
              fabs(iScrollPosition - t->scrollRange[0]) < fabs(iScrollPosition - iTransform->scrollRange[1]) ||
              fabs(iScrollPosition - t->scrollRange[1]) < fabs(iScrollPosition - iTransform->scrollRange[1])*/){
                return true;
              }
        }
      }
    }
  }
  return false;
}


//-------------------------------------------------------------------------------------
// class scScrollList
scScrollList::scScrollList(float iHeight) {
  // Top of list.
  horizontalScrollPosition = 0;
  verticalScrollPosition = 0;
  // Max scroll position.
  scrollHeight = iHeight;
  
  minVerticalScroll = 0;
  maxVerticalScroll = iHeight;
  
  // Object that slides up and down.
  scrollRoot = new ofxObject();
  addChild(scrollRoot);
  
  verticalScrollTracker = new ofxObject();
  horizontalScrollTracker = new ofxObject();
  
  addChild(verticalScrollTracker);
  addChild(horizontalScrollTracker);
  
}


scScrollList::~scScrollList() {}


// Updates all transforms based on current scrollPosition.
void scScrollList::update(float iTime, bool isVertical) {
  
  if (isVertical){
    
    if (verticalScrollTracker->isAnimating()){
      
      verticalScrollPosition = verticalScrollTracker->getScale().x;
    }
    
  }else{
    
    if (horizontalScrollTracker->isAnimating()){
      
      horizontalScrollPosition = horizontalScrollTracker->getScale().x;
      
    }
    
  }
  
  
  time = iTime;
  if (start_disable_time != -1){
    if (time > start_disable_time){
      start_disable_time = -1;
      if (disable_duration > -1) {
        disable(disable_duration);
      }
      disable_duration = -1;
    }
  }
  
  if (disable_time != -1) {
    if (time > disable_time) {
      disable_time = -1;
      enable();
    }
  }
  
  if (isEnabled) {
    for(auto obj : scrollObjects) {
      vector<ofxScrollTransform *> transforms;
      float scrollPosition;
      
      if (isVertical) {
        transforms = obj->verticalTransforms;
        scrollPosition = verticalScrollPosition;
      } else {
        transforms = obj->horizontalTransforms;
        scrollPosition = horizontalScrollPosition;
      }
      
      for(auto t : transforms){
        
        // OFX_SHOW transform is handled a bit differently:
        // If the scrollPosition is the in the object's scroll range, it is shown.
        // If it is not, it is automatically hidden.
        if(t->transform == OF_SHOW){
          if(ofInRange(scrollPosition, t->scrollRange[0], t->scrollRange[1])){
            obj->object->show();
          }else{
            obj->object->hide();
          }
        }
        
        // Check to see if this transform is overridden by another of the same type.
        if(!obj->isTrumped(t, scrollPosition)){
          // Check first to see if the scrollPosition is in the transform range.
          // EG: Can't do this because scroll refresh rate is too slow.
          //if(ofInRange(scrollPosition, t->scrollRange[0], t->scrollRange[1])){
          // Normalized position in range.
          
          float p;
          
          if (scrollPosition < minVerticalScroll){
            
            p = ofxMessage::interpolateTime(t->interpolation, (minVerticalScroll - t->scrollRange[0])/(t->scrollRange[1] - t->scrollRange[0]));
            
          }else if (scrollPosition > maxVerticalScroll){
            
            p = ofxMessage::interpolateTime(t->interpolation, (scrollPosition - t->scrollRange[0])/(t->scrollRange[1] - t->scrollRange[0]));
            
          }else{
            
            p = ofxMessage::interpolateTime(t->interpolation, (scrollPosition - t->scrollRange[0])/(t->scrollRange[1] - t->scrollRange[0]));
            
          }
          
          p = max(0.0f, p);
          p = min(1.0f, p);
          // Do the right thing for each type of transform.
          switch(t->transform){
            case OF_TRANSLATEX:
            {
              float startX = t->valueRange[0][0];
              float endX = t->valueRange[1][0];
              ofVec3f currTrans = obj->object->getTrans();
              
              obj->object->setTrans(startX + p*(endX - startX), currTrans.y, currTrans.z);
              break;
            }
            case OF_TRANSLATEY:
            {
              float startY = t->valueRange[0][0];
              float endY = t->valueRange[1][0];
              ofVec3f currTrans = obj->object->getTrans();
              
              obj->object->setTrans(currTrans.x, startY + p*(endY - startY), currTrans.z);
              break;
            }
            case OF_TRANSLATE:
              obj->object->setTrans(t->valueRange[0] + p*(t->valueRange[1] - t->valueRange[0]));
              break;
            case OF_ROTATE:
              obj->object->setRot(t->valueRange[0] + p*(t->valueRange[1] - t->valueRange[0]));
              break;
            case OF_SCALE:
              obj->object->setScale(t->valueRange[0][0] + p*(t->valueRange[1][0] - t->valueRange[0][0]));
              break;
            case OF_SCALE3:
              obj->object->setScale(t->valueRange[0] + p*(t->valueRange[1] - t->valueRange[0]));
              break;
            case OF_SETALPHA:
              //maybe do set color w/ new alpha?
              obj->object->setAlpha((t->valueRange[0][0] + p*(t->valueRange[1][0] - t->valueRange[0][0])));
              
              break;
            case OF_SETCOLOR:
              obj->object->setColor((t->valueRange[0] + p*(t->valueRange[1] - t->valueRange[0])).x,(t->valueRange[0] + p*(t->valueRange[1] - t->valueRange[0])).y,(t->valueRange[0] + p*(t->valueRange[1] - t->valueRange[0])).z);
              break;
            default:
              break;
          };
        }
      }
    }
  }
}


void scScrollList::disable() {
  isEnabled = false;
  
  start_disable_time = -1;
  disable_time = -1; //overwrite timer if necessary
  disable_duration = -1;
}


/*void scScrollList::disable(float iDuration) {
 isEnabled = false;
 disable_time = time + iDuration;
 
 //start timer.
 }*/

void scScrollList::disable(float iDelay) {
  if (iDelay > 0) {
    start_disable_time = time + iDelay;
    disable_duration = 0;
  } else
    disable();
}

void scScrollList::disable(float iDelay, float iDuration) {
  
  start_disable_time = time + iDelay;
  disable_duration = iDuration;
  
  //start timer.
}


void scScrollList::enable() {
  isEnabled = true;
  
  start_disable_time = -1;
  disable_duration = -1;
  disable_time = -1;
  
}

void scScrollList::setMinVerticalScroll(float iMin){
  
  if (iMin >= 0)
    minVerticalScroll = iMin;
}

void scScrollList::setMaxVerticalScroll(float iMax){
  
  if (iMax < scrollHeight)
    maxVerticalScroll = iMax;
  
}


int scScrollList::addSnapPoint(float iScrollPos, bool isHorizontal){
  
  int newSnap;
  
  if (isHorizontal){
    
    newSnap = horizontalSnapPoints.size();
    horizontalSnapPoints[newSnap] = iScrollPos;
    
  }else {
    
    newSnap = verticalSnapPoints.size();
    verticalSnapPoints[newSnap] = iScrollPos;
    
  }
  
  return newSnap;
  
}


int scScrollList::addSnapPoint(float iScrollPos, int iSnapInd, bool isHorizontal){
  
  
  if (isHorizontal){
    
    horizontalSnapPoints[iSnapInd] = iScrollPos;
    
  }else {
    
    verticalSnapPoints[iSnapInd] = iScrollPos;
    
  }
  
  return iSnapInd;
  
}


void scScrollList::clearSnapPoints(bool isHorizontal){
  
  if (isHorizontal){
    
    horizontalSnapPoints.clear();
    
  }else{
    
    verticalSnapPoints.clear();
  }
  
}

float scScrollList::gotoSnapPoint(int iIndex, float iVelocity, bool isHorizontal, int iBezier){
  
  if (isHorizontal){
    
    if (horizontalSnapPoints.count(iIndex) > 0){
      
      float duration = abs(horizontalScrollPosition-horizontalSnapPoints[iIndex])/iVelocity;
      
      if (duration > 0){
        //do a message with the scroll tracker
        horizontalScrollTracker->stopMessages();
        horizontalScrollTracker->doMessage1f(OF_SCALE, 0, duration, iBezier, horizontalSnapPoints[iIndex]);
      }
      
      return duration;
      
    }
    
  }else{
    
    
    if (verticalSnapPoints.count(iIndex) > 0){
      
      float duration = abs(verticalScrollPosition-verticalSnapPoints[iIndex])/iVelocity;
      
      if (duration > 0){
        //do a message with the scroll tracker
        verticalScrollTracker->stopMessages();
        verticalScrollTracker->doMessage1f(OF_SCALE, 0, duration, iBezier, verticalSnapPoints[iIndex]);
      }
      
      return duration;
      
      
    }else{
      
      ofLogNotice("Vertical snap point not found");
    }
    
  }
  
  
  
  
  return false;
}

//Goes to the closest snap point following the present scroll location
void scScrollList::gotoNextSnapPoint(int iDirection, bool isHorizontal){
  
  float minDist = 999999.0f;
  int minIndex = 0;
  float scrollDist = 0;
  
  if (isHorizontal){
    
    for (auto mapkey : horizontalSnapPoints){
      
      float scrollPos = horizontalScrollPosition; //not really
      float diff = mapkey.second - scrollPos;
      
      if (iDirection == -1){
        
        
        if ((diff <= 0)&&(abs(diff) < abs(minDist))){
          minIndex = mapkey.first;
          minDist = diff;
        }
        
        
      }else{
        
        if ((diff >= 0)&&(diff < minDist)){
          minIndex = mapkey.first;
          minDist = diff;
        }
        
      }
    }
    
  }else{
    
    for (auto mapkey : verticalSnapPoints){
      
      float scrollPos = verticalScrollPosition; //not really
      float diff = mapkey.second - scrollPos;
      
      //We're scrolling backwards
      if (iDirection == -1){
        
        if ((diff <= 0)&&(abs(diff) < abs(minDist))){
          minIndex = mapkey.first;
          minDist = diff;
        }
        
        
        // We're scrolling forwards
      }else{
        
        if ((diff >= 0)&&(diff < minDist)){
          minIndex = mapkey.first;
          minDist = diff;
        }
        
      }
      
    }
  }
  
  scrollDist = minDist;
  
  //get the scroll position of the snap point and use it to calculate velocity
  
  float velocity = abs(minDist/2.0f); // 2 seconds to next
  float minVelocity = 250.0f; // same as magnetCheck? no -- but related. adjust together.
  
  velocity = max(velocity, minVelocity);
  gotoSnapPoint(minIndex, velocity, false, OF_EASE_OUT);
  
}


// TODO: CONFIRM NOT USED
//void scScrollList::gotoClosestSnapPoint(float iVelocity, bool isHorizontal){
//
//  float minDist = 999999.0f;
//  int minIndex = 0;
//
//  if (isHorizontal){
//
//    for (auto mapkey : horizontalSnapPoints){
//
//      float scrollPos = horizontalScrollPosition; //not really
//      float diff = abs(scrollPos - mapkey.second);
//
//
//      if (diff < minDist){
//
//        minIndex = mapkey.first;
//        minDist = diff;
//      }
//    }
//
//  }else{
//
//    for (auto mapkey : verticalSnapPoints){
//
//      float scrollPos = verticalScrollPosition; //not really
//      float diff = abs(scrollPos - mapkey.second);
//
//      if (diff < minDist){
//
//        minIndex = mapkey.first;
//        minDist = diff;
//      }
//    }
//  }
//
////  gotoSnapPoint(minIndex, iVelocity);
//  gotoSnapPoint(minIndex, iVelocity, false, OF_EASE_OUT);
//
//}




ofxScrollObject* scScrollList::addObject(ofxObject *iObject, bool iAddAsChild)
{
  // Check if it's already been added. If so, return associated ofxScrollObject.
  for(auto obj : scrollObjects){
    if(obj->object == iObject){
      return obj;
    }
  }
  // If not, add it and return new ofxScrollObject.
  ofxScrollObject *sO = new ofxScrollObject(iObject);
  scrollObjects.push_back(sO);
  if(iAddAsChild == true) {
    scrollRoot->addChild(iObject);
  }
  return sO;
}


ofxScrollObject* scScrollList::getScrollObject(ofxObject *iObject)
{
  // Check if it's already been added. If so, return associated ofxScrollObject.
  for(auto obj : scrollObjects){
    if(obj->object == iObject){
      return obj;
    }
  }
  // Otherwise return NULL.
  return NULL;
}


// Set the max scroll position.
void scScrollList::setScrollHeight(float iHeight) {
  scrollHeight = iHeight;
}


// Set the scroll position of the list. iPosition is in pixels and 0 moves the list to the top.
void scScrollList::setScroll(float iPosition, bool isVertical) {
  if (isEnabled) {
    if (isVertical) {
      
      if (!verticalScrollTracker->isAnimating()){
        verticalScrollTracker->setScale(iPosition);
        verticalScrollPosition = iPosition;
        verticalScrollPosition = max(minVerticalScroll, verticalScrollPosition);
        verticalScrollPosition = min(maxVerticalScroll, verticalScrollPosition);
      }
    } else {
      
      if (!horizontalScrollTracker->isAnimating()){
        horizontalScrollTracker->setScale(iPosition);
        horizontalScrollPosition = iPosition;
        horizontalScrollPosition = max(0.0f, horizontalScrollPosition);
        horizontalScrollPosition = min(scrollHeight, horizontalScrollPosition);
      }
    }
  }
}


void scScrollList::moveScroll(float iDistance, bool isVertical) {
  setScroll(getScrollPosition(isVertical) + iDistance, isVertical);
}


float scScrollList::getScrollPosition(bool isVertical) {
  
  if (isVertical) {
    return verticalScrollPosition;
  } else {
    return horizontalScrollPosition;
  }
}

void scScrollList::stopScroll() {
  horizontalScrollTracker->stopMessages();
  verticalScrollTracker->stopMessages();
}