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
  


  
public:
  
  ofxScroller(float iHeight);
  ~ofxScroller();

  void update(float iTime);
  
  // For adding objects
  ofxScrollObject* addObject(ofxObject *iObject, bool iAddAsChild=true);
  ofxScrollObject* getScrollObject(ofxObject *iObject);
  
  // To get collection of scroll objects
  vector<ofxScrollObject *> getScrollObjects(){ return scrollObjects;}
  
  // For updating scroll position
  float setScroll(float iPosition);
  float moveScroll(float iDistance);
  float getScrollPosition();
  
  // To set max scroll value
  void setScrollHeight(float iHeight);
  
  // for snap points
  int addSnapPoint(float iScrollPos);
  int addSnapPoint(float iScrollPos, int iSnapInd);
  void clearSnapPoints();
  float gotoSnapPoint(int iIndex, float iVelocity, int iBezier=OF_EASE_INOUT);
  int gotoClosestSnapPoint(float iVelocity);
  int gotoNextSnapPoint(scrollDirection iDirection);
  
  // Set flags to enable/disable scroll list
  void enable();
  void disable();
  void disable(float iDelay); //disable for a certain number of seconds
  void disable(float iDelay, float iDuration); //disable for a certain number of seconds

  
private:
  
  // Size of scroller
  float scrollHeight;
  
  // Current scroll position
  float scrollPosition;
  
  // For timing scroll disable
  float start_disable_time = -1;
  float disable_time = -1;
  float disable_duration = -1;
  float time;
  
  // Option for disabling scroller
  bool isEnabled = true;
  
  // Object root
  ofxObject *scrollRoot;

  // Collection of scroll objects
  vector<ofxScrollObject *> scrollObjects;
  
  // Scroll tracking object (for jumping to snap points)
  ofxObject *scrollTracker;
  bool isSnapping = false;

  // Collection of snap points
  map<int, float> snapPoints;

};