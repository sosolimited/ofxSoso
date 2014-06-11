
/*
 shaderWavyPlane
 
 */

#pragma once

#include "ofxObject.h"
#include "ofImage.h"
#include "of3dPrimitives.h" // So we can use ofPlanePrimitive

class shaderWavyPlane : public ofxObject{
	
public:
	
	shaderWavyPlane(int iWidth, int iHeight, int iCols, int iRows);
	~shaderWavyPlane();
	
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
