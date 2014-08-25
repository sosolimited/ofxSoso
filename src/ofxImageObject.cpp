/*
 *  ofxImageObject.cpp
 *  openFrameworks
 *
 *  Created by Eric Gunther on 11/8/09.
 *  Copyright 2009 Sosolimited. All rights reserved.
 *
 */

#include "ofxImageObject.h"

using namespace std;
using namespace cinder;

ofxImageObject::ofxImageObject(string iFilename, bool iLoadNow)
{
	filename = iFilename;
	if(iLoadNow){
		if( fs::is_regular_file( iFilename ) ) {
			image = gl::Texture::create( loadImage( iFilename ) );
		}
	}
	width = image->getWidth();
	height = image->getHeight();
	isCentered = false;
	
    renderDirty = true; //eg 070112
}

ofxImageObject::~ofxImageObject(){
}

void ofxImageObject::enableTexture(bool iB)
{
    renderDirty = true;
}

//EG 021513
gl::TextureRef ofxImageObject::getTexture()
{
    return image;
}

void ofxImageObject::render()
{	
    //eg 070112 Added display lists.
    if(renderDirty){
        
        glDeleteLists(displayList, 1);
        glNewList(displayList, GL_COMPILE_AND_EXECUTE);

        //For when iLoadNow=false is used in constructor
        if(width==0 || height==0){
            width = image->getWidth();
            height = image->getHeight();
        }
        
        if(isCentered){
						gl::pushModelView();
						gl::translate(-width/2, -height/2, 0);
        }
        
        glNormal3f(0,0,1);
			gl::color( Color::white() );
				gl::draw( image );
        
        if(isCentered){
						gl::popModelView();
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
	if( image ) {
		image.reset();
	}
    renderDirty = true;
}