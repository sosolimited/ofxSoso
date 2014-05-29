/*
 ofxLetterTextObject
 
 ofxLetterTextObject inherits from ofxTextObject. It turns each character in the text into an ofxObject,
 which lets you animate the letters independently. Each ofxLetterTextObjectLetter has a home position
 that you can reference in your animations.
 
 Note: ofxLetterTextObject does not yet support the fancy render-time Unicode and named entity
 character substitutions supported by ofxTextObject.
 
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
#include "ofxTextObject.h"

class ofxLetterTextObjectLetter : public ofxObject
{
public:
	//ofxLetterTextObjectLetter(ofxSosoTrueTypeFont *iFont, char iChar, float iX, float iY, float iScaleFactor);
	ofxLetterTextObjectLetter(ofxSosoTrueTypeFont *iFont, char *iChar, float iX, float iY, float iScaleFactor);  //eg 070411
  ~ofxLetterTextObjectLetter();
  
	void                    render();
  
public:
	char                    character;
  ofVec3f                 home;
  float                   scaleFactor;
	char                    *charPointer;
	ofxSosoTrueTypeFont			*font;
};


class ofxLetterTextObject : public ofxTextObject
{
public:
	ofxLetterTextObject(ofxSosoTrueTypeFont *iFont, char *iString=NULL);
	~ofxLetterTextObject();
  
	//void                  predraw();
	//void                  postdraw();
	void                    render();
  
	void                    setString(char *iString);
  void                    setString(string iString);
	void                    setFont(ofxSosoTrueTypeFont *iFont);
	void                    setColumnWidth(float iWidth);
	void                    setPointSize(float iPointSize);
	void                    setLeading(float iLeading);
	void                    setSpaceWidth(float iWidth);
	void                    setAlignment(TextObjectAlignment iAlignment);
	void                    setColor(float iR, float iG, float iB, float iA=255);
	void                    setColor(ofVec4f iColor);
	void                    setColor(ofColor iColor);
	//void                  normalizeScale();
	void                    setWordColor(int iIndex, float iR, float iG, float iB, float iA=255.0);
  void                    setWordColor(string iWord, float iR, float iG, float iB, float iA=255.0, bool iExactMatch = true);
  
protected:
	void                    cleanupLetters();
	void                    rebuildLetters();
  
public:
	vector<ofxLetterTextObjectLetter*>	letters;
	
};
