// ofxScrollerObject
// Created May 8, 2014
//  Originally created by Eric Gunther on 11/7/13.
//
//
//  scScrollList uses the skrollr model (jQuery library we used for Recon2012)
//  of tweening transforms based on the scroll list position.
//

#pragma once
#include "ofxObject.h"
#include "ofxMessage.h" // Using the interpolate time function to interpolate scroller values
#include <map>

// extending ofxMessage
enum {
  OF_TRANSLATEX = -1,
  OF_TRANSLATEY = -2
};

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
  ~ofxScrollObject(){};
  
  void addTransform(ofxScrollTransform *iTransform, bool isVertical = true);
  void addTransform(int iTransform, float startScroll, float endScroll, int iInterpolation=OF_LINEAR, float startVal=0, float endVal=0, bool isVertical = true);
  void addTransform(int iTransform, float startScroll, float endScroll, int iInterpolation, ofVec3f startVal, ofVec3f endVal, bool isVertical = true);
  bool isTrumped(ofxScrollTransform *iTransform, float iScrollPosition, bool isVertical = true);
  void disableScrollObject(){ isEnabled = false; }
  void enableScrollObject() { isEnabled = true; }
  
  float getYOffset() { return yOffset; }
  void setYOffset(float iOffset){ yOffset = iOffset; }
  
  
public:
  ofxObject *object;
  
  // allows for two types of transforms in one scroll list
  vector<ofxScrollTransform *> verticalTransforms;
  vector<ofxScrollTransform *> horizontalTransforms;
  
  
  bool isEnabled = true;
  float yOffset = 0;
  
};


// ---------------------------------------------------------------
// Scroll list that puts it all together.
class scScrollList : public ofxObject
{
  
public:
  scScrollList(float iHeight);
  ~scScrollList();
  
  
  
  void update(float iTime, bool isVertical = true);
  
  ofxScrollObject* addObject(ofxObject *iObject, bool iAddAsChild=true);
  ofxScrollObject* getScrollObject(ofxObject *iObject);
  
  void setScroll(float iPosition, bool isVertical = true);
  void moveScroll(float iDistance, bool isVertical = true);
  float getScrollPosition(bool isVertical = true);//{ return scrollPosition;}
  void stopScroll();
  
  float getScrollValueAtY(float iPrevScroll, float iYPos, ofxScrollObject *iObj);
  
  void setScrollHeight(float iHeight);
  
  // Set flags to enable/disable scroll list
  void enable();
  void disable();
  void disable(float iDelay); //disable for a certain number of seconds
  void disable(float iDelay, float iDuration); //disable for a certain number of seconds
  bool isEnabled = true;
  
  vector<ofxScrollObject *> getScrollObjects(){ return scrollObjects;}
  
  float oldScrollPos = 0;
  float scrollHeight;
  
  void setMinVerticalScroll(float iMin);
  void setMaxVerticalScroll(float iMax);
  
  // for snap points
  int addSnapPoint(float iScrollPos, bool isHorizontal = false);
  int addSnapPoint(float iScrollPos, int iSnapInd, bool isHorizontal = false);
  void clearSnapPoints(bool isHorizontal=false);
  float gotoSnapPoint(int iIndex, float iVelocity, bool isHorizontal=false, int iBezier=OF_EASE_INOUT);
  void gotoClosestSnapPoint(float iVelocity, bool isHorizontal=false);
  void gotoNextSnapPoint(int iDirection, bool isHorizontal=false);
  
protected:
  float start_disable_time = -1;
  float disable_time = -1;
  float disable_duration = -1;
  float time;
  float tweenedPos = -1;
  
  
  float verticalScrollPosition;
  float horizontalScrollPosition;
  
  float minVerticalScroll;  // AO note:  we can have transforms in these ranges, but they will be ignored
  float maxVerticalScroll;
  
  // not using for now, only vert min/max implemented
  float minHorizScroll;  // AO note:  we can have transforms in these ranges, but they will be ignored
  float maxHorizScroll;
  
  ofxObject *scrollRoot;
  
  bool diagonalScrollEnabled = false;
  
  vector<ofxScrollObject *> scrollObjects;
  
  ofxObject *verticalScrollTracker; //used for going to snap points
  ofxObject *horizontalScrollTracker; //used for going to snap points
  map<int, float> verticalSnapPoints;
  map<int, float> horizontalSnapPoints;
  int lastVerticalSnapPoint = 0;
  int lastHorizontalSnapPoint = 0;
  
  
};