/*
 *  ofxTextureObject.cpp
 *  openFrameworks
 *
 *  Created by Eric Gunther on 11/8/09.
 *  Copyright 2009 Sosolimited. All rights reserved.
 *
 */

#include "ofxTextureObject.h"
#include "ofGraphics.h"

ofxTextureObject::ofxTextureObject(int w, int h)
{

    texture.allocate(w, h, GL_RGBA);
    width = w;
	height = h;
	isCentered = false;
	
    renderDirty = true; //eg 070112
}

ofxTextureObject::ofxTextureObject(){

    texture.clear();

}

void ofxTextureObject::render()
{	
    //eg 070112 Added display lists.
    if(renderDirty){
        
        glDeleteLists(displayList, 1);
        glNewList(displayList, GL_COMPILE_AND_EXECUTE);

        //For when iLoadNow=false is used in constructor
        if(width==0 || height==0){
            width = texture.getWidth();
            height = texture.getHeight();
        }
        
        if(isCentered){
            ofPushMatrix();
            ofTranslate(-width/2, -height/2, 0);
        }
        
        glNormal3f(0,0,1); 
        texture.draw(0,0);
        if(isCentered){
            ofPopMatrix();
        }
        
        glEndList();
        renderDirty = false;
    }else{
		glCallList(displayList);
    }
}


void ofxTextureObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
    renderDirty = true;
}


void ofxTextureObject::clear()
{
	texture.clear();
    renderDirty = true;
}