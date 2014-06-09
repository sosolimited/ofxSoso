#pragma once

#include "ofxObject.h"

class ofxArcObject : public ofxObject{
  
public:
  
	
  ofxArcObject(float iOuterRadius, float iInnerRadius, float iStartAngle, float iEndAngle);
	~ofxArcObject();
  
	void  render();
	void  setResolution(int iRes);
  
	void  setStartAngle(float iAngle);
	void  setEndAngle(float iAngle);
  float getStartAngle(); //LM 062012
  float getEndAngle(); //LM 062012
	void  setColors(ofColor iColor1, ofColor iColor2);
	
  
public:
  
	int resolution;
	float outerRadius,
  innerRadius,
  startAngle,
  endAngle;
  
  ofColor color1, color2;
	float height;
  
  
};
