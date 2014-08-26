/*
ImageObject

Convenient Object encapsulation of Image. Lets you manipulate the image as an object in the scene.
Create with iLoadNow = false if you want to load it later with something like threaded image loading

Soso OpenFrameworks Addon
Copyright (C) 2012 Sosolimited

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#pragma once

#include "soso/Object.h"
#include "cinder/gl/Texture.h"

namespace soso
{

class ImageObject : public Object{

public:

	ImageObject(std::string iFilename, bool iLoadNow=true);
	~ImageObject();

	void						render();
	void						setCentered(bool iEnable);

	// DW: Unsure what enableTexture is about.
	DEPRECATED_ATTRIBUTE
	void						enableTexture(bool iB);
	void						clear();

	ci::gl::TextureRef                   getTexture();   //EG 021513

public:
	ci::gl::TextureRef	image;
	bool						isCentered;
	float						width,
								height;

	std::string						filename;

};

} // namespace soso