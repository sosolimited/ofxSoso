/*
ofxDynamicPolygonObject

Extension of ofxPolygonObject for conveniently and independently animating the vertex positions, texture coordinates, and colors 
of an arbitrary polygon. It uses dummy ofxObjects, which can be easily animated, that control the parameters. 

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

#include "ofxPolygonObject.h"


class ofxDynamicPolygonObject : public ofxPolygonObject
{
public:
	ofxDynamicPolygonObject(int iNumVertices);
	~ofxDynamicPolygonObject();

	void								idle(float iTime);

	void								setVertexPos(int iVertexNum, ofVec3f iPos);
	void								setVertexTexCoords(int iVertexNum, float iU, float iV);
	void								setVertexColor(int iVertexNum, float iR, float iG, float iB, float iA=255);

	void								gotoVertexPos(int iVertexNum, ofVec3f iPos, float iDur=-1.0, float iDelay=0, int iInterp=OF_LINEAR);
	void								gotoVertexTexCoords(int iVertexNum, float iU, float iV, float iDur=-1.0, float iDelay=0, int iInterp=OF_LINEAR);
	void								gotoVertexColor(int iVertexNum, float iR, float iG, float iB, float iA=255, float iDur=-1.0, float iDelay=0, int iInterp=OF_LINEAR);
		

public:
	struct ofxDynamicPolygonVert{	//PEND maybe make one tracker object and just use trans for pos, scale for tex, and color for color
		ofxObject			*posTracker,
							*texTracker,
							*colorTracker;
	};
	vector<ofxDynamicPolygonVert *>		dynamicVerts;



};