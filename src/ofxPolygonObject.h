/*
 ofxPolygonObject
 
 ofxObject for creating and texturing arbitrary polygons. Also see ofxDynamicPolygonObject for
 an animatable version of ofxPolygon.
 
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

class ofxPolygonObject : public ofxObject
{
public:
	ofxPolygonObject(int iNumVertices);
	virtual ~ofxPolygonObject();
	
	void                  render();
  
	void                  setTexture(ofImage *iTex);
  void                  setTextureByReference(ofTexture &iTex);
	virtual void          setVertexPos(int iVertexNum, ofVec3f iPos);
	virtual void          setVertexTexCoords(int iVertexNum, float iU, float iV);
	virtual void          setVertexColor(int iVertexNum, float iR, float iG, float iB, float iA=255);
  
	ofVec3f               getVertexPos(int iVertexNum);
	ofVec2f               getVertexTexCoords(int iVertexNum);
	void                  setDrawMode(int iDrawMode);	//OF_FILLED, OF_OUTLINE
	void                  enableVertexColoring(bool iEnable);
	ofTexture*            getTexture();
  
protected:
	bool                  vertexColoringEnabled;
	int                   numVertices;
  float                 drawMode;
	float                 *vertices;
	float                 *texCoords;
	float                 *vertexColors;
	ofTexture             *texture;
  
};
