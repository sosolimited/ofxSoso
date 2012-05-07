/*
 *  ofxImageObject.cpp
 *  openFrameworks
 *
 *  Created by Eric Gunther on 11/8/09.
 *  Copyright 2009 Sosolimited. All rights reserved.
 *
 */

#include "ofxImageObject.h"
#include "ofGraphics.h"

ofxImageObject::ofxImageObject(string iFilename, bool iLoadNow)
{
	filename = iFilename;
	if(iLoadNow){
		image.loadImage(iFilename);	
        image.getTextureReference().texData.bFlipTexture = true;  //Get images right side up in soso world     
	}
	width = image.getWidth();
	height = image.getHeight();
	isCentered = false;
	
}

ofxImageObject::~ofxImageObject(){}

void ofxImageObject::enableTexture(bool iB)
{
	image.setUseTexture(iB);
}

void ofxImageObject::render()
{	
	//for when iLoadNow=false is used in constructor
	if(width==0 || height==0){
		width = image.getWidth();
		height = image.getHeight();
	}

	if(isCentered){
		ofPushMatrix();
		ofTranslate(-width/2, -height/2, 0);
	}
	
	glNormal3f(0,0,1); 
	image.draw(0,0);
	
	if(isCentered){
		ofPopMatrix();
	}
}


void ofxImageObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
}


void ofxImageObject::clear()
{
	image.clear();
}