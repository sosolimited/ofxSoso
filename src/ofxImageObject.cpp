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
    image = new ofImage();
		loaded = image->loadImage(iFilename);
    image->getTextureReference().texData.bFlipTexture = true;  //Get images right side up in soso world
	}else{
    
    image = NULL;
    
  }
  
  if (image){
    width = image->getWidth();
    height = image->getHeight();
    
  }else{
    
    width = 0;
    height = 0;
    
  }
  
	isCentered = false;
	
  renderDirty = true; //eg 070112
}

// Destructor.
ofxImageObject::~ofxImageObject(){
  if (image){
    delete image;
  }
}


void ofxImageObject::loadImage(string iFilename){
  
  if (image==NULL){
    
    image = new ofImage();
    
  }
  
  if (loaded){
    
    delete image;
    
    //remake
    image = new ofImage();
    
  }
  
  loaded = image->loadImage(iFilename);
  image->getTextureReference().texData.bFlipTexture = true;  //Get images right side up in soso world
  
  width = image->getWidth();
  height = image->getHeight();
}

void ofxImageObject::enableTexture(bool iB)
{
  if (image){
    image->setUseTexture(iB);
    renderDirty = true;
  }
}

//EG 021513
ofTexture ofxImageObject::getTexture()
{
  
  return image->getTextureReference();
}

void ofxImageObject::render()
{
  //eg 070112 Added display lists.
  if(renderDirty){
    
    glDeleteLists(displayList, 1);
    glNewList(displayList, GL_COMPILE_AND_EXECUTE);
    
    //For when iLoadNow=false is used in constructor
    if(width==0 || height==0){
      
      if (image){
        width = image->getWidth();
        height = image->getHeight();
      }else{
        
        width = 0;
        height = 0;
        
      }
    }
    
    if(isCentered){
      ofPushMatrix();
      ofTranslate(-width/2, -height/2, 0);
    }
    
    glNormal3f(0,0,1);
    
    if (image)
      image->draw(0,0);
    
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
    if (image)
      image->clear();
    loaded = false;
  }
  renderDirty = true;
}