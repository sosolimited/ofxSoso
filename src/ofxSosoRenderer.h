/*
 ofxSosoRenderer
 
 ofxSosoRenderer inherits from ofGLRenderer. It overrides setupScreen() and setupScreenPerspective() in order to
 set up the GL context to work with ofxScene. Use it by calling:
 ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
 
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

#include "ofGLRenderer.h"

class ofxSosoRenderer : public ofGLRenderer
{
public:
  ofxSosoRenderer(float iWidth, float iHeight, bool iOrthographic=false, bool iVFlip=false, float iFov=60, float iNearDist = 0, float iFarDist = 0);
  ~ofxSosoRenderer();
  
  void        setScreenParams(bool iOrthographic, bool iVFlip=false, float iFov=60, float iNearDist=0, float iFarDist=0);
  
	void        setupScreen();
  
  ofMatrix4x4 getModelViewMatrix() { return modelViewMatrix; }
  ofMatrix4x4 getProjectionMatrix() { return projectionMatrix; }

private:
  bool            orthographic;
  
  bool            vFlip;
  
  float           width;
  float           height;
  
  float           fov;
  float           nearDist,
                  farDist;
  
  ofMatrix4x4     projectionMatrix;
  ofMatrix4x4     modelViewMatrix;
  
  
  
  
};

