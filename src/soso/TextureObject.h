/*
TextureObject

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

class TextureObject : public Object{

public:

	TextureObject(int w, int h);
  TextureObject( const ci::gl::TextureRef &texture );

	void						render();
	void						setCentered(bool iEnable);
	void						enableTexture(bool iB);
	DEPRECATED_ATTRIBUTE
	void						clear();

public:
	ci::gl::TextureRef			texture;
	bool						isCentered;
	float						width,
								height;


};

} // namespace soso
