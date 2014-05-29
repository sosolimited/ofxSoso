
//
//  ofxScroller.cpp
//
//  Created by Eric Gunther on 11/7/13.  Ported to ofxSoso on May 29, 2014 by Alex Olivier.
//
//

#include "ofxScroller.h"

//----------------------------------------------------------------------------------
// Class ofxScrollTransform
// TODO: Potentially use ofxMessages
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

// Destructor
ofxScrollObject::~ofxScrollObject(){
  
  // Delete transforms
  for (auto transform : transforms){
    delete transform;
  }
  transforms.clear();
}

// Add a scroll transform to the list of transforms
void ofxScrollObject::addTransform(ofxScrollTransform *iTransform) {
  transforms.push_back(iTransform);
}

// Create and add a scroll transform to the list of transforms
void ofxScrollObject::addTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, float iStartVal, float iEndVal) {
  addTransform(new ofxScrollTransform(iTransform, iStartScroll, iEndScroll, iInterpolation, iStartVal, iEndVal));
}


// Create and add a scroll transform to the list of transforms
void ofxScrollObject::addTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, ofVec3f iStartVal, ofVec3f iEndVal) {
  addTransform(new ofxScrollTransform(iTransform, iStartScroll, iEndScroll, iInterpolation, iStartVal, iEndVal));
}

// EG: Hack to make two transforms of the same type work.
// If there is a transform of the same type (eg OF_TRANSLATE) with a range that is closer
// to the current scrollPosition, then return true.  Otherwise, returns false.
bool ofxScrollObject::isTrumped(ofxScrollTransform *iTransform, float iScrollPosition) {
  
  // If we're in the range of the given transform, then nothing trumps it!
  if(ofInRange(iScrollPosition, iTransform->scrollRange[0], iTransform->scrollRange[1])){
    return false;
  }
  
  //Go through all transforms
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
// class ofxScroller
ofxScroller::ofxScroller(float iHeight) {
  
  // Top of scroller
  scrollPosition = 0;
  
  // Max scroll position.
  scrollHeight = iHeight;
  
  // Object that slides up and down.
  scrollRoot = new ofxObject();
  addChild(scrollRoot);
  
  // Tracking object for jumping to snap points
  scrollTracker = new ofxObject();
  addChild(scrollTracker);

}

// Destructor
ofxScroller::~ofxScroller() {

  delete scrollTracker;
  
  //delete all scroll objects
  for (auto scroll_obj : scrollObjects){
    delete scroll_obj;
  }
  
  scrollObjects.clear();
  snapPoints.clear();
}


// Updates all transforms based on current scrollPosition.
void ofxScroller::update(float iTime) {
  
  // If we are in the middle of jumping to a snap point...
  if (scrollTracker->isAnimating()){
    scrollPosition = scrollTracker->getScale().x;
  }else if (isSnapping){
    
    // This is a slight hack because the scrollTracker animation is one frame behind
    // TODO: Fix hack
    scrollPosition = scrollTracker->getScale().x;
    isSnapping = false;
  }else{
    
    scrollTracker->setScale(scrollPosition);
  }
  
  // Check all disable timers
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
      for(auto t : obj->transforms){
        
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
          if (scrollPosition < 0){
            p = ofxMessage::interpolateTime(t->interpolation, (0 - t->scrollRange[0])/(t->scrollRange[1] - t->scrollRange[0]));
            
          }else if (scrollPosition > (scrollHeight)){
            p = ofxMessage::interpolateTime(t->interpolation, (scrollPosition - t->scrollRange[0])/(t->scrollRange[1] - t->scrollRange[0]));
            
          }else{
            p = ofxMessage::interpolateTime(t->interpolation, (scrollPosition - t->scrollRange[0])/(t->scrollRange[1] - t->scrollRange[0]));
          }
          
          p = max(0.0f, p);
          p = min(1.0f, p);

          // Do the right thing for each type of transform.
          switch(t->transform){
            case OF_TRANSLATE:
            {
              
              // FIRST check if we are using any OF_RELATIVE_VAL values...
              // If so, we should grab the object's current position
              float x,y,z;
              ofVec3f startVals;
              ofVec3f endVals;
              
              ofVec3f pos = obj->object->getTrans();
              ofVec3f vec = t->valueRange[0];
              
              // Check if using relative values for start vals
              if(vec.x == OF_RELATIVE_VAL) x = pos.x;
              else x = vec.x;
              if(vec.y == OF_RELATIVE_VAL) y = pos.y;
              else y = vec.y;
              if(vec.z == OF_RELATIVE_VAL) z = pos.z;
              else z = vec.z;
                
                startVals = ofVec3f(x, y, z);
              
              ofVec3f endVec = t->valueRange[1];
              
              // Check if using relative vals for end vals
              if(endVec.x == OF_RELATIVE_VAL) x = pos.x;
              else x = endVec.x;
              if(endVec.y == OF_RELATIVE_VAL) y = pos.y;
              else y = endVec.y;
              if(endVec.z == OF_RELATIVE_VAL) z = pos.z;
              else z = endVec.z;
              
              endVals = ofVec3f(x, y, z);
              
              // Set trans
              obj->object->setTrans(startVals + p*(endVals - startVals));
              break;
            }
  
            case OF_ROTATE:
            {
              float x,y,z;
              ofVec3f startVals;
              ofVec3f endVals;
              
              ofVec3f rot = obj->object->getRot();
              ofVec3f vec = t->valueRange[0];
              
              // FIRST check if we are using any OF_RELATIVE_VAL values...
              // If so, we should grab the object's current rotation
              
              // Check for relative start vals
              if(vec.x == OF_RELATIVE_VAL) x = rot.x;
              else x = vec.x;
              if(vec.y == OF_RELATIVE_VAL) y = rot.y;
              else y = vec.y;
              if(vec.z == OF_RELATIVE_VAL) z = rot.z;
              else z = vec.z;
              
              startVals = ofVec3f(x, y, z);
              
              ofVec3f endVec = t->valueRange[1];
              
              // Check for relative end vals
              if(endVec.x == OF_RELATIVE_VAL) x = rot.x;
              else x = endVec.x;
              if(endVec.y == OF_RELATIVE_VAL) y = rot.y;
              else y = endVec.y;
              if(endVec.z == OF_RELATIVE_VAL) z = rot.z;
              else z = endVec.z;
              
              endVals = ofVec3f(x, y, z);
              
              obj->object->setRot(startVals + p*(endVals - startVals));
              break;
            }
             
            case OF_SCALE:
            {
              
              float val0 = t->valueRange[0][0];
              float val1 = t->valueRange[1][0];
              float startVal, endVal;
              float scale = obj->object->getScale().x;
        
              // FIRST check if we are using any OF_RELATIVE_VAL values...
              // If so, we should grab the object's current scale
              
              // Check for relative start vals
              if (val0 == OF_RELATIVE_VAL)
                startVal = scale;
              else startVal = val0;
              
              // Check for relative end vals
              if (val1 == OF_RELATIVE_VAL)
                endVal = scale;
              else endVal = val1;

              obj->object->setScale(startVal + p*(endVal - startVal));
              break;
            }
              
            case OF_SCALE3:
            {
              float x,y,z;
              ofVec3f startVals;
              ofVec3f endVals;
              
              ofVec3f scale = obj->object->getScale();
              ofVec3f vec = t->valueRange[0];
              
              // FIRST check if we are using any OF_RELATIVE_VAL values...
              // If so, we should grab the object's current scale
              
              // Check for relative start vals
              if(vec.x == OF_RELATIVE_VAL) x = scale.x;
              else x = vec.x;
              if(vec.y == OF_RELATIVE_VAL) y = scale.y;
              else y = vec.y;
              if(vec.z == OF_RELATIVE_VAL) z = scale.z;
              else z = vec.z;
              
              startVals = ofVec3f(x, y, z);

              ofVec3f endVec = t->valueRange[1];
              
              // Check for relative end vals
              if(endVec.x == OF_RELATIVE_VAL) x = scale.x;
              else x = endVec.x;
              if(endVec.y == OF_RELATIVE_VAL) y = scale.y;
              else y = endVec.y;
              if(endVec.z == OF_RELATIVE_VAL) z = scale.z;
              else z = endVec.z;
              
              endVals = ofVec3f(x, y, z);
              
              obj->object->setScale(startVals + p*(endVals - startVals));
              break;
            }
              
            case OF_SETALPHA:
            {
              float val0 = t->valueRange[0][0];
              float val1 = t->valueRange[1][0];
              float startVal, endVal;
              float alpha = obj->object->getAlpha();
              
              
              // FIRST check if we are using any OF_RELATIVE_VAL values...
              // If so, we should grab the object's current alpha
              
              // Check for relative start vals
              if (val0 == OF_RELATIVE_VAL)
                startVal = alpha;
              else startVal = val0;
              
              // Check for relative end vals
              if (val1 == OF_RELATIVE_VAL)
                endVal = alpha;
              else endVal = val1;
              
              
              //maybe do set color w/ new alpha?
              obj->object->setAlpha((startVal + p*(endVal - startVal)));
              
              break;
            }
              
            case OF_SETCOLOR:
            {
              float r,g,b;
              ofVec3f startVals;
              ofVec3f endVals;
              
              ofVec4f color = obj->object->getColor();
              ofVec3f vec = t->valueRange[0];
              
              // FIRST check if we are using any OF_RELATIVE_VAL values...
              // If so, we should grab the object's current alpha
              
              // Check for relative start vals
              if(vec.x == OF_RELATIVE_VAL) r = color.x;
              else r = vec.x;
              if(vec.y == OF_RELATIVE_VAL) g = color.y;
              else g = vec.y;
              if(vec.z == OF_RELATIVE_VAL) b = color.z;
              else b = vec.z;
              
              startVals = ofVec3f(r, g, b);
              
              ofVec3f endVec = t->valueRange[1];
              
              // Check for relative end vals
              if(endVec.x == OF_RELATIVE_VAL) r = color.x;
              else r = endVec.x;
              if(endVec.y == OF_RELATIVE_VAL) g = color.y;
              else g = endVec.y;
              if(endVec.z == OF_RELATIVE_VAL) b = color.z;
              else b = endVec.z;
              
              endVals = ofVec3f(r, g, b);
              
              obj->object->setColor((startVals + p*(endVals - startVals)).x,(startVals + p*(endVals - startVals)).y,(startVals + p*(endVals - startVals)).z);
              
              break;
            
            }

            default:
              break;
          };
        }
      }
    }
  }
}

// Disable the scroller - it will not update
void ofxScroller::disable() {
  
  isEnabled = false;
  start_disable_time = -1;
  disable_time = -1; //overwrite timer if necessary
  disable_duration = -1;
}


/*void ofxScroller::disable(float iDuration) {
 isEnabled = false;
 disable_time = time + iDuration;
 
 //start timer.
 }*/

// Disable the scroller after a delay - it will not update
void ofxScroller::disable(float iDelay) {
  if (iDelay > 0) {
    start_disable_time = time + iDelay;
    disable_duration = 0;
  } else
    disable();
}

// Disable the scroller for a certain duration, after a delay
void ofxScroller::disable(float iDelay, float iDuration) {
  
  start_disable_time = time + iDelay;
  disable_duration = iDuration;

}

// Enable the scroller
void ofxScroller::enable() {
  isEnabled = true;
  
  start_disable_time = -1;
  disable_duration = -1;
  disable_time = -1;
  
}

// Add a new snap point - we can jump to this scroll position
int ofxScroller::addSnapPoint(float iScrollPos){
  
  int newSnap = snapPoints.size();
  snapPoints[newSnap] = iScrollPos;
  
  return newSnap;
  
}

// Add a scroll point at a certain index
int ofxScroller::addSnapPoint(float iScrollPos, int iSnapInd){

  snapPoints[iSnapInd] = iScrollPos;
  return iSnapInd;
  
}

// Delete all snap points
void ofxScroller::clearSnapPoints(){
  snapPoints.clear();
}

// Go to a saved snap point with a given velocity and interpolation
float ofxScroller::gotoSnapPoint(int iIndex, float iVelocity, int iBezier){
  
  // If we have this snap point...
  if (snapPoints.count(iIndex) > 0){
    
    // Given the velocity, calculate the snap duration
    float duration = abs(scrollPosition-snapPoints[iIndex])/iVelocity;
    
    // If we're not already at this position...
    if (duration >= 0){
        
      // Start animating the scroll tracker (using scale to keep track of scrollPosition)
      scrollTracker->stopMessages();
      scrollTracker->setScale(scrollPosition); // set scale to current scroll position
      scrollTracker->doMessage1f(OF_SCALE, 0.0f, duration, iBezier, snapPoints[iIndex]);
      isSnapping = true;  // Set this flag - needed for hack to ensure we finish snap animation
      }
      return duration;
    }
  return 0;
}

// Goes to the closest snap point in a given direction (forward or backward)
int ofxScroller::gotoNextSnapPoint(scrollDirection iDirection){
  
  float minDist = 999999.0f;
  int minIndex = -1;
  float scrollDist = 0;
    
  for (auto mapkey : snapPoints){
      
    float scrollPos = scrollPosition;
    float diff = mapkey.second - scrollPos;
    
      // Snapping backwards
      if (iDirection == OF_SCROLL_BACKWARD){
        
        if ((diff <= 0)&&(abs(diff) < abs(minDist))){
          minIndex = mapkey.first;
          minDist = diff;
        }

      // Snapping forwards
      }else{
        
        if ((diff >= 0)&&(diff < minDist)){
          minIndex = mapkey.first;
          minDist = diff;
        }
      }
    }
    
  scrollDist = minDist;
  
  //get the scroll position of the snap point and use it to calculate velocity
  
  float velocity = abs(minDist/2.0f); // 2 seconds to next
  float minVelocity = 250.0f; // same as magnetCheck? no -- but related. adjust together.
  
  velocity = max(velocity, minVelocity);

  gotoSnapPoint(minIndex, velocity,  OF_EASE_OUT);
  
  return minIndex;
  
}


// Goes to absolute closest snap point in either direction
int ofxScroller::gotoClosestSnapPoint(float iVelocity){

  float minDist = 999999.0f;
  int minIndex = -1;

    for (auto mapkey : snapPoints){

      float scrollPos = scrollPosition; //not really
      float diff = abs(scrollPos - mapkey.second);

      if (diff < minDist){

       minIndex = mapkey.first;
        minDist = diff;
      }
    }

  return gotoSnapPoint(minIndex, iVelocity,  OF_EASE_OUT);

}

// Add a new scroll object.
// ATTENTION: By default, this adds the objects as a child of the scroller
// Make sure you're not adding the object as a child elsewhere!
ofxScrollObject* ofxScroller::addObject(ofxObject *iObject, bool iAddAsChild)
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


ofxScrollObject* ofxScroller::getScrollObject(ofxObject *iObject)
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
void ofxScroller::setScrollHeight(float iHeight) {
  scrollHeight = iHeight;
}


// Set the scroll position of the list. iPosition is in pixels and 0 moves the list to the top.
float ofxScroller::setScroll(float iPosition) {

  if (isEnabled) {
    if (!isSnapping){
      
      scrollTracker->setScale(iPosition);
      scrollPosition = iPosition;
      scrollPosition = max(0.0f, scrollPosition);
      scrollPosition = min(scrollHeight+1, scrollPosition);  //TODO:
      
    }
  }
  return scrollPosition;
}


// Move scroll position by a certain amount
float ofxScroller::moveScroll(float iDistance) {
  return setScroll(getScrollPosition() + iDistance);
}


// Get current scroll position
float ofxScroller::getScrollPosition() {
  return scrollPosition;
}
