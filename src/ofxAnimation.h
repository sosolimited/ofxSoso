/*
 ofxAnimation
 
 ofxAnimation can be used to animate the position, scale, rotation, color, alpha, and shown/hidden status of ofxObjects.
 You animate by "tweening" ofxObjects. For each tween, specify an object, a start/end time, a start/end value, and the interpolation.
 Tweens can also be played once or looped. See tweenBez and tweenSpline for creating non-linear tween paths.
 
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
#include "ofxMessage.h"



class ofxTween
{
public:
	ofxTween(ofxObject *iObject, ofxMessage *iMessage);
	~ofxTween();
  
	void							start();
	void							stop();
  
public:
	ofxObject					*object;
	ofxMessage				*message;
};


class ofxAnimation
{
public:
	ofxAnimation();
	~ofxAnimation();
  
	void							start();
	void							stop();
	bool							isAnimating();
	float							getDuration();
  
  
	ofxTween*					tween(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
                            float iX0, float iX1,
                            int iPlayMode=OF_ONE_TIME_PLAY);
	ofxTween*					tween(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
                            float iX0, float iY0, float iZ0,
                            float iX1, float iY1, float iZ1,
                            int iPlayMode=OF_ONE_TIME_PLAY);
	ofxTween*					tween(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
                            float iX0, float iY0, float iZ0, float iW0,
                            float iX1, float iY1, float iZ1, float iW1,
                            int iPlayMode=OF_ONE_TIME_PLAY);
  
	ofxTween*					tween(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
                            ofVec3f iV0, ofVec3f iV1,
                            int iPlayMode=OF_ONE_TIME_PLAY);
	ofxTween*					tween(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
                            ofVec4f iV0, ofVec4f iV1,
                            int iPlayMode=OF_ONE_TIME_PLAY);
  
	ofxTween*					showObject(ofxObject *iObject, float iTime, int iPlayMode=OF_ONE_TIME_PLAY);
	ofxTween*					hideObject(ofxObject *iObject, float iTime, int iPlayMode=OF_ONE_TIME_PLAY);
  
	ofxTween*					tweenBezier(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp, vector<ofVec4f> iPath, int iPlayMode=OF_ONE_TIME_PLAY);
	ofxTween*					tweenSpline(ofxObject *iObject, int iID, float iStartTime, float iEndTime, int iInterp, vector<ofVec4f> iPath, int iPlayMode=OF_ONE_TIME_PLAY);
  
	ofxTween*					tweenFunc(ofxObject *iObject,float (*iFunction)(void *), void *iArgs, float iTime, int iPlayMode=OF_ONE_TIME_PLAY); //LM
  
public:
	vector<ofxTween *>				tweens;
  
};
