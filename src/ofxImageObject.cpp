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
		loaded = image.loadImage(iFilename);
        image.getTextureReference().texData.bFlipTexture = true;  //Get images right side up in soso world     
	}
	width = image.getWidth();
	height = image.getHeight();
	isCentered = false;
	
    renderDirty = true; //eg 070112
}

ofxImageObject::~ofxImageObject(){
    if (loaded)
        image.clear();
}

void ofxImageObject::enableTexture(bool iB)
{
	image.setUseTexture(iB);
    renderDirty = true;
}

//EG 021513
ofTexture ofxImageObject::getTexture()
{
    return image.getTextureReference();
}

void ofxImageObject::render()
{	
    //eg 070112 Added display lists.
    if(renderDirty){
        
        glDeleteLists(displayList, 1);
        glNewList(displayList, GL_COMPILE_AND_EXECUTE);

        //For when iLoadNow=false is used in constructor
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
        
        glEndList();
        renderDirty = false;
    }else{
		glCallList(displayList);
    }
}


void ofxImageObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
    renderDirty = true;
}


void ofxImageObject::clear()
{
    if (loaded) {
        image.clear();
        loaded = false;
    }
    renderDirty = true;
}