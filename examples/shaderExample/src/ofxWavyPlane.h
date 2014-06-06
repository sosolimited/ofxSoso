
/*
 ofxWavyPlane
 
 */

#pragma once

#include "ofxObject.h"
#include "ofImage.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive

class ofxWavyPlane : public ofxObject{
	
public:
	
	ofxWavyPlane(int iWidth, int iHeight, int iCols, int iRows);
	~ofxWavyPlane();
	
	void						render();
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	void						clear();
  
  // Overrides ofxObject's virtual method
  void setShaderParams();
	
public:
  
	ofPlanePrimitive  *plane;
	bool  isCentered;
	float width,
  height;
  
};
