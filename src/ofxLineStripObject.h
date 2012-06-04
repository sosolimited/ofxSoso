/*
ofxLineStripObject

Simple ofxObject for drawing and manipulating line strips. 

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

class ofxLineStripObject : public ofxObject
{
public:
	ofxLineStripObject(int iNumVerts=2);
	ofxLineStripObject();

	void								render();
	
	void								setLineWidth(float iWeight);
	void								setVertexPos(int iVertNum, float iX, float iY, float iZ);
	void								setVertexColor(int iVertNum, float iR, float iG, float iB, float iA);
	
	void								addVertex(float iX, float iY, float iZ);
	void								addVertexColor(float iR, float iG, float iB, float iA);

public:
	vector<ofVec3f>						vertices;
	vector<ofVec4f>						vertColors;
	float								lineWidth;
};