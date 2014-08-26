/*
LetterTextObject

LetterTextObject inherits from TextObject. It turns each character in the text into an Object,
which lets you animate the letters independently. Each LetterTextObjectLetter has a home position
that you can reference in your animations.

Note: LetterTextObject does not yet support the fancy render-time Unicode and named entity
character substitutions supported by TextObject.

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
#include "soso/TextObject.h"

namespace soso
{

class LetterTextObjectLetter : public Object
{
public:
	//LetterTextObjectLetter(SosoTrueTypeFont *iFont, char iChar, float iX, float iY, float iScaleFactor);
	LetterTextObjectLetter(SosoTrueTypeFont *iFont, char *iChar, float iX, float iY, float iScaleFactor);  //eg 070411
    ~LetterTextObjectLetter();

	void							render();

public:
	char							character;
	char							*charPointer;
	ci::Vec3f                         home;
	SosoTrueTypeFont				*font;
	float							scaleFactor;
};


class LetterTextObject : public TextObject
{
public:
	LetterTextObject(SosoTrueTypeFont *iFont, char *iString=NULL);
	~LetterTextObject();

	//void							predraw();
	//void							postdraw();
	void							render();

	void							setString(char *iString);
    void                            setString(string iString);
	void							setFont(SosoTrueTypeFont *iFont);

	void							setColumnWidth(float iWidth);
	void                            setPointSize(float iPointSize);
	void							setLeading(float iLeading);
	void							setSpaceWidth(float iWidth);
	void							setAlignment(TextObjectAlignment iAlignment);

	void							setColor(float iR, float iG, float iB, float iA=255);
	void							setColor(ci::Vec4f iColor);
	void                            setColor(ci::ColorA8u iColor);
	//void							normalizeScale();
	void							setWordColor(int iIndex, float iR, float iG, float iB, float iA=255.0);
    void                            setWordColor(string iWord, float iR, float iG, float iB, float iA=255.0,
                                                 bool iExactMatch = true);
protected:
	void							cleanupLetters();
	void							rebuildLetters();

public:
	vector<LetterTextObjectLetter*>	letters;

};

} // namespace soso
