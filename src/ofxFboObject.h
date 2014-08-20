/*
ofxFboObject

Convenient ofxObject encapsulation of ofFbo. Lets you manipulate the fbo as an object in the scene.

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

#include "ofxObject.h"
#include "ofFbo.h"

class ofxFboObject : public ofxObject
{
public:
	ofxFboObject(int iWidth, int iHeight, int iInternalFormat=GL_RGBA, int iNumSamples = 0);
	~ofxFboObject();

	void					idle(float iTime);
	void					render();
    int                     getWidth() { return this->fbo->getWidth(); };
    int                     getHeight() { return this->fbo->getHeight(); };

public:
	ofFbo					*fbo;
protected:
    int                     width, height;

};