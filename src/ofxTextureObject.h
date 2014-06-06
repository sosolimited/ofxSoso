/*
 ofxWavyPlane

 */

#pragma once

#include "ofxObject.h"
#include "ofTexture.h"
#include "ofImage.h"

class ofxTextureObject : public ofxObject{
	
public:
	
	ofxTextureObject(int w, int h);
	~ofxTextureObject();
	
	void						render();
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	void						clear();
	
public:
	ofTexture				texture;
	bool						isCentered;
	float						width,
                  height;
  
};
