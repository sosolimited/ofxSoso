/*
 *  ofxRectangleObject.cpp
 *  HBOSnowApp
 *
 *  Created by Eric Gunther on 11/17/09.
 *  Copyright 2009 Sosolimited. All rights reserved.
 *
 */

#include "ofxRectangleObject.h"

ofxRectangleObject::ofxRectangleObject(float iW, float iH)
{
	dimensions.set(iW, iH);
	isCentered = false;
	isFilled = true;
  
	strokeAlpha = 0.0;
	fillAlpha = 255.0;
  
}

ofxRectangleObject::~ofxRectangleObject(){}

void ofxRectangleObject::render()
{

	ofColor color = getColor();

	if(isCentered)  { 
	
		if (fillAlpha > 0.0) {
			ofFill();
			ofSetColor(color.r, color.g, color.b, fillAlpha * drawMaterial->color.a/255.0);
      ofDrawRectangle(-dimensions.x/2.0f, -dimensions.y/2.0f, dimensions.x, dimensions.y);

		}
    
		if (strokeAlpha > 0.0) {
			ofNoFill();
			ofSetColor(color.r, color.g, color.b, strokeAlpha * drawMaterial->color.a/255.0);
			ofDrawRectangle(-dimensions.x/2.0f, -dimensions.y/2.0f, dimensions.x, dimensions.y);
		}
    
	} else {
    
		if (fillAlpha > 0.0) {
			ofFill();
			ofSetColor(color.r, color.g, color.b, fillAlpha * drawMaterial->color.a/255.0);
			ofDrawRectangle(0, 0, dimensions.x, dimensions.y);
		}
		if (strokeAlpha > 0.0) {
			ofNoFill();
			ofSetColor(color.r, color.g, color.b, strokeAlpha * drawMaterial->color.a/255.0);
			ofDrawRectangle(0, 0, dimensions.x, dimensions.y);

		}
    
	}
  
}

void ofxRectangleObject::setCentered(bool iB)
{
	isCentered = iB;
}

