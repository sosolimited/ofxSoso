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

    loadImage(iFilename);
    
	}
  
  if (tex!=NULL){
    width = tex->getWidth();
    height = tex->getHeight();
    
  }else{
    
    width = 0;
    height = 0;
    
  }
  
	isCentered = false;
  renderDirty = true; //eg 070112
}

// Destructor.
ofxImageObject::~ofxImageObject(){
  if (tex){
    delete tex;
  }
  
  //TODO: memory leak here caused by ofImage!
}

ofImage* ofxImageObject::makeReferenceImage(string iFilename, bool iDestroyPixels){

  ofImage *imageRef = new ofImage();
  loaded = imageRef->loadImage(iFilename, iDestroyPixels);
  imageRef->getTextureReference().texData.bFlipTexture = true;  //Get images right side up in soso world
  
  return imageRef;
}


void ofxImageObject::loadImage(string iFilename){

  if (tex==NULL){
    
    ofImage *imageRef = makeReferenceImage(iFilename, destroyPixels);
    
    tex = new ofTexture();
    tex->loadData(imageRef->getPixelsRef());
  
    width = imageRef->getWidth();
    height = imageRef->getHeight();
    
    loaded = true;
    
    delete imageRef;
    
  }else{
    
    delete tex;
    tex = NULL;
    
    loadImage(iFilename);

  }
}

//EG 021513
ofTexture ofxImageObject::getTexture()
{
  return *tex;

}

//AO 053014
void ofxImageObject::setDestroyPixels(bool iDestroyPixels){
  
  //TODO: Implement this in ofTexture instead of ofImage
  destroyPixels = iDestroyPixels;
  
}

void ofxImageObject::render()
{
  //eg 070112 Added display lists.
  if(renderDirty){
    
    glDeleteLists(displayList, 1);
    glNewList(displayList, GL_COMPILE_AND_EXECUTE);
    
    //For when iLoadNow=false is used in constructor
    if(width==0 || height==0){
      width = tex->getWidth();
      height = tex->getHeight();
    }
    
    if(isCentered){
      ofPushMatrix();
      ofTranslate(-width/2, -height/2, 0);
    }
    
    glNormal3f(0,0,1);
    tex->draw(0,0);
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
    if (tex)
      tex->clear();
    loaded = false;
  }
  renderDirty = true;
}