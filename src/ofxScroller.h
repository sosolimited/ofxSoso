// ofxScrollerObject
// Created May 8, 2014
//  Originally created by Eric Gunther on 11/7/13.
//
//
//  ofxScroller uses the skrollr model (jQuery library we used for Recon2012)
//  of tweening transforms based on the scroll list position.
//

#pragma once
#include "ofxObject.h"
#include "ofxMessage.h" // Using the interpolate time function to interpolate scroller values
#include <map>


enum scrollDirection{
  OF_SCROLL_EITHER_DIRECTION = 0,
  OF_SCROLL_FORWARD = 1,
  OF_SCROLL_BACKWARD = -1
  
  
};

// SUPPORTING CLASSES
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// Holds all data for a specific transform on an ofxScrollObject.
class ofxScrollTransform{
  

  public:
  
    ofxScrollTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, float iStartVal, float iEndVal);
    ofxScrollTransform(int iTransform, float iStartScroll, float iEndScroll, int iInterpolation, ofVec3f iStartVal, ofVec3f iEndVal);
    ~ofxScrollTransform(){};

  
  public:
  
    int transform;
    int interpolation;
    float scrollRange[2];
    ofVec3f valueRange[2];

};

// ---------------------------------------------------------------
// Holds object and list of transforms.
class ofxScrollObject{
  
  public:
  
    ofxScrollObject(ofxObject *iObject);
    ~ofxScrollObject();
    
    void addTransform(ofxScrollTransform *iTransform);
    void addTransform(int iTransform, float startScroll, float endScroll, int iInterpolation=OF_LINEAR, float startVal=0, float endVal=0);
    void addTransform(int iTransform, float startScroll, float endScroll, int iInterpolation, ofVec3f startVal, ofVec3f endVal);
  
    bool isTrumped(ofxScrollTransform *iTransform, float iScrollPosition);

  public:
    
    ofxObject *object;
    vector<ofxScrollTransform *> transforms;

  };

// END SUPPORTING CLASSES
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// Scroll list that puts it all together.
class ofxScroller : public ofxObject
{

// Public methods
public:
  
  ofxScroller(float iHeight);
  ~ofxScroller();
  
  void update(float iTime);
  
  // Set flags to enable/disable scroll list
  void enable();
  void disable();
  void disable(float iDelay); //disable for a certain number of seconds
  void disable(float iDelay, float iDuration); //disable for a certain number of seconds
  
  // For adding objects
  ofxScrollObject* addObject(ofxObject *iObject, bool iAddAsChild=true);
  ofxScrollObject* getScrollObject(ofxObject *iObject);
  
  // To get collection of scroll objects
  vector<ofxScrollObject *> getScrollObjects(){ return scrollObjects;}
  
  // Methods for adding new snap points
  int addSnapPoint(float iScrollPos);
  int addSnapPoint(float iScrollPos, int iSnapInd);
  void clearSnapPoints();
  
  // Methods for going to snap poitns
  float gotoSnapPoint(int iIndex, float iVelocity, int iBezier=OF_EASE_INOUT);
  int gotoClosestSnapPoint(float iVelocity); //just go to closest in either dir
  int gotoClosestSnapPoint(float iVelocity, scrollDirection iDirection);
  int gotoNextSnapPoint(float iVelocity, scrollDirection iDirection);
  
  // For updating scroll position
  float setScroll(float iPosition);
  float moveScroll(float iDistance);
  float setScrollVelocity(float iVelocity);
  float getScrollVelocity(){ return scrollVelocity; }
  float getScrollPosition(){ return scrollPosition; }
  
  // To set max scroll value
  void setScrollHeight(float iHeight);
  float getScrollHeight(){ return scrollHeight; }
  

// Private methods
private:
  
  int getClosestSnapPoint(scrollDirection iDirection, bool iGetNext);
  
  
// Private variables
private:
  
  float scrollHeight;   // Size of scroller
  float scrollPosition; // Current scroll position

  ofxObject *scrollRoot;  // Object root
  vector<ofxScrollObject *> scrollObjects;  // Collection of scroll objects

  // Collection of snap points
  map<int, float> snapPoints;
  
  // Scroll tracking object (for jumping to snap points)
  ofxObject *scrollTracker;
  bool isSnapping = false;
  
  bool isEnabled = true;  // Option for disabling scroller
  float start_disable_time = -1; // For timing scroll disable
  float disable_time = -1;
  float disable_duration = -1;
  float time;
  
  // For velocity
  bool isUsingVelocity;
  float scrollVelocity;

};