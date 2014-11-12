/*
 *  ofxImageObject.cpp
 *  openFrameworks
 *
 *  Created by Eric Gunther on 11/8/09.
 *  Copyright 2009 Sosolimited. All rights reserved.
 *
 */

#include "ofxImageObject.h"


ofxImageObject::ofxImageObject(string iFilename, bool iLoadNow, bool iDestroyPixels)
{

  tex = new ofTexture();
	filename = iFilename;
	if(iLoadNow){
    
    loadImage(iFilename, iDestroyPixels);
    
	}

	isCentered = false;
  renderDirty = true; //eg 070112
}

// Destructor.
ofxImageObject::~ofxImageObject(){

  tex->clear();
  
}


void ofxImageObject::loadImage(string iFilename, bool iDestroyPixels){
  
  // Texture loading code borrowed from this function
  // loaded = ofLoadImage(*tex, iFilename);
  // We broke it out so that we can destroy pixel buffer, if desired
  
  ofPixels pixels;
	loaded = ofLoadImage(pixels,iFilename);
	if(loaded){
		tex->allocate(pixels.getWidth(), pixels.getHeight(), ofGetGlInternalFormat(pixels));
		tex->loadData(pixels);
	}

  // If destroy pixel buffer flag, clear pixel buffer after loading image
  if (iDestroyPixels){
    if (pixels.isAllocated()){

      pixels.clear();
      
    }
  }
  
  
  // Flip it for Soso universe
  tex->getTextureData().bFlipTexture = true;
  
}


//EG 021513
ofTexture* ofxImageObject::getTexture()
{
  return tex;
}


void ofxImageObject::render()
{
   
    if (loaded){
      
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
}


void ofxImageObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
  renderDirty = true;
}


void ofxImageObject::clear()
{
  tex->clear();
  renderDirty = true;
}