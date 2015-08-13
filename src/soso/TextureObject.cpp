/*
 *  TextureObject.cpp
 *  openFrameworks
 *
 *  Created by Eric Gunther on 11/8/09.
 *  Copyright 2009 Sosolimited. All rights reserved.
 *
 */

#include "soso/TextureObject.h"

using namespace cinder;
using namespace soso;

TextureObject::TextureObject(int w, int h)
{
	gl::Texture::Format format;
	format.setInternalFormat( GL_RGBA );
	texture = gl::Texture::create( w, h, format );
	texture->setTopDown();

	width = w;
	height = h;
	isCentered = false;

	renderDirty = true; //eg 070112
}

TextureObject::TextureObject( const gl::TextureRef &texture ):
	texture( texture )
{
	width = texture->getWidth();
	height = texture->getHeight();
	texture->setTopDown();
	isCentered = false;
	renderDirty = true;
}

void TextureObject::render()
{
	//For when iLoadNow=false is used in constructor
	if(width==0 || height==0){
			width = texture->getWidth();
			height = texture->getHeight();
	}

	if(isCentered){
		gl::pushModelView();
		gl::translate(-width/2, -height/2, 0);
	}

	auto normal = gl::context()->getGlslProg()->getAttribSemanticLocation(ci::geom::NORMAL);
	gl::vertexAttrib3f(normal, 0, 0, 1);
	gl::draw( texture );
	if(isCentered){
		gl::popModelView();
	}
}


void TextureObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
    renderDirty = true;
}


void TextureObject::clear()
{
	texture.reset();
  renderDirty = true;
}