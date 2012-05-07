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

	ofVec4f color = getColor();

	if(isCentered)  { 
	
		if (fillAlpha > 0.0) {
			ofFill();
			ofSetColor(color.x, color.y, color.z, fillAlpha * drawMaterial->color.w/255.0);
			ofRect(-dimensions.x/2.0f, -dimensions.y/2.0f, dimensions.x, dimensions.y);	
		}

		if (strokeAlpha > 0.0) {
			ofNoFill();
			ofSetColor(color.x, color.y, color.z, strokeAlpha * drawMaterial->color.w/255.0);
			ofRect(-dimensions.x/2.0f, -dimensions.y/2.0f, dimensions.x, dimensions.y);	
		}

	} else {

		if (fillAlpha > 0.0) {
			ofFill();
			ofSetColor(color.x, color.y, color.z, fillAlpha * drawMaterial->color.w/255.0);
			ofRect(0, 0, dimensions.x, dimensions.y);	
		}
		if (strokeAlpha > 0.0) {
			ofNoFill();
			ofSetColor(color.x, color.y, color.z, strokeAlpha * drawMaterial->color.w/255.0);
			ofRect(0, 0, dimensions.x, dimensions.y);	

		}

	}

}

void ofxRectangleObject::setCentered(bool iB)
{
	isCentered = iB;
}

