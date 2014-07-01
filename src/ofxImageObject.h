/*
 ofxImageObject
 
 Convenient ofxObject encapsulation of ofxImage. Lets you manipulate the image as an object in the scene.
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

#include "ofxObject.h"
#include "ofImage.h"

class ofxImageObject : public ofxObject{
	
public:
	ofxImageObject(string iFilename, bool iLoadNow=true);
	~ofxImageObject();
  
	void            loadImage(string iFilename);
	void						render();
	void						setCentered(bool iEnable);
  void            setDestroyPixels(bool iDestroyPixels);
	void						clear();
  ofTexture       getTexture();   //EG 021513

private:
  
  ofImage*        makeReferenceImage(string iFilename, bool iDestroyPixels);
  
public:
  
	bool            isCentered;
	float           width,
                  height;
  bool            loaded;
  bool            destroyPixels=true; // Flag for if we should destroy pixels after loading image, AO 053014
	string          filename;
  
  
private:
  
  // AO: Using ofTexture as of 6/26
  ofTexture       *tex = NULL;
};
