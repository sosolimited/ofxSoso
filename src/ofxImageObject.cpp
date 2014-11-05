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

  tex = new ofTexture();
	filename = iFilename;
	if(iLoadNow){
    
    loadImage(iFilename);
    
	}

	isCentered = false;
  renderDirty = true; //eg 070112
}

// Destructor.
ofxImageObject::~ofxImageObject(){

  tex->clear();
  
}


void ofxImageObject::loadImage(string iFilename){
  
  ofImage *image = new ofImage();
  bool imageLoaded = image->loadImage(iFilename);
  
  float w = image->getWidth();
  float h = image->getHeight();
  
  if (imageLoaded){

    image->mirror(true, false);
    
    if (image->getPixels() != NULL){
      
      if (image->type == OF_IMAGE_COLOR_ALPHA){
        
        tex->loadData(image->getPixels(), image->getWidth(), image->getHeight(), GL_RGBA);
        
      }else if (image->type == OF_IMAGE_COLOR){
        
        tex->loadData(image->getPixels(), image->getWidth(), image->getHeight(), GL_RGB);
        
      }else{
        
        tex->loadData(image->getPixels(), image->getWidth(), image->getHeight(), GL_RGB);
        
      }
    
      width = tex->getWidth();
      height = tex->getHeight();
      
      loaded = true;
      
    }
    

  
  }
  
  delete image;
    
}


void ofxImageObject::idle(float iTime){
  
  
  if (!loaded){
    
    if (ofGetElapsedTimef() > 15.0f){
      
      loadImage(filename);
    }
    
  }
  
  
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