/*
ofxTextObject

ofxTextObject is an ofxObject that renders text. Parts of the code are adapted from Luke Malcom's Text Suite ofAddon 
It does word wrapping. It uses an extended version of ofTrueTypeFont. 
It uses mipmapping to give the letters a smoother look at multiple scales, independent of resolution. 
Its DPI is set to 72 so that when text loaded at a resolution of, say, 16 it actually displays at 16 pixels. 
It loads kerning pairs from the font and allows you to do manual kerning as well. 
It parses and translates named entities as well as Unicode byte sequences.

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
#include "ofxSosoTrueTypeFont.h"

//Helper class to store fonts for easy static access by name.
class ofxTextFont{
public:
	ofxTextFont(string iFontName, ofxSosoTrueTypeFont *iFont);
	ofxTextFont(string iFontName, string iFilename, int iFontsize, bool iAntiAliased=true, bool iFullCharacterSet=true, bool iMakeContours = false, bool iMakeMipMaps = true);
	~ofxTextFont();
    
public:
	ofxSosoTrueTypeFont				*font;
	char							*fontName;
    
};


class wordBlock {
public:
    string				rawWord;
    
    ofVec2f             pos;	
    vector<ofVec2f>     charPositions;	
    
    float				width;
    float				height;
    ofColor				color;
    
    bool				isNewLine; 
    
    ofxSosoTrueTypeFont	*font;		
    
};


class lineBlock {
public:
    vector<int>			wordsID;
    
    ofVec2f				pos;	
    float				width;
    float				height;
    
};


enum TextObjectAlignment{ 
	OF_TEXT_ALIGN_LEFT, 
	OF_TEXT_ALIGN_RIGHT, 
	OF_TEXT_ALIGN_JUSTIFIED, 
	OF_TEXT_ALIGN_CENTER 
};

class ofxTextObject : public ofxObject
{
public:
	ofxTextObject(ofxSosoTrueTypeFont *iFont, char *iString=NULL);	
	ofxTextObject(ofxSosoTrueTypeFont *iFont, string iString=NULL);	//LM 063012
	~ofxTextObject();

	void							render();

    void                            drawText(float x=0, float y=0, bool drawFlag=true);                   
    void                            drawLeft(float x=0, float y=0, bool drawFlag=true);		
    void                            drawRight(float x=0, float y=0, bool drawFlag=true);	
    void                            drawCenter(float x=0, float y=0, bool drawFlag=true);	
    void                            drawJustified(float x=0, float y=0, bool drawFlag=true); 

	virtual void					setColor(float iR, float iG, float iB, float iA=255.0);
	virtual void					setColor(ofVec4f color);
    virtual void                    setColor(ofColor color);
	void							setWordColor(int iIndex, float iR, float iG, float iB, float iA=255.0);
    void                            setWordColor(string iWord, float iR, float iG, float iB, float iA=255.0,
                                                 bool iExactMatch = true);
    void                            setAllWordsColor(float iR,  float iG, float iB, float iA=255.0);
    void							setAlpha(float iA);	

    void                            enableWordColoring(bool iFlag); //eg
	void							enableDrawAsShapes(bool iFlag);

    virtual void					setString(char *iString);
	virtual void					setString(string iString); //LM 063012
    char*							getString();

	virtual void					setColumnWidth(float iWidth);
	virtual void					setColumnBox(float iWidth, float iHeight);
	virtual void					setAlignment(TextObjectAlignment iAlignment);

	float							getWidth();
    float                           getHeight();
	float							getColumnWidth();
    
    ofVec2f                         getCharPosition(int iIndex); //LM 031213
	    
	virtual void					setFont(ofxSosoTrueTypeFont *iFont);
    ofxSosoTrueTypeFont*			getFont();
    
    virtual void                    setPointSize(float iPointSize);
    float							getPointSize();
    float                           getScaleFactor();
    
    virtual void					setLeading(float iLeading);
    float							getLeading();
    
    virtual void					setSpaceWidth(float iWidth);
    
    int								getNumLines();
	
	void							enableDisplayList(bool iEnable);
     
    
    
    static void						addFont(ofxTextFont *iFont);
	static ofxTextFont*				addFont(char *iName, ofxSosoTrueTypeFont *iFont);
	static ofxSosoTrueTypeFont*		getFont(char *iFontName);
    
protected:
    void                            init(ofxSosoTrueTypeFont *iFont);
    
    int                             wrapTextX(float lineWidth);
    void                            wrapTextArea(float rWidth, float rHeight);
    bool					    	wrapTextForceLines(int linesN);

    void                            updatePositions(float width);
            
    void                            _loadWords();
    void                            _trimLineSpaces();
    float                           _getWidthOfWords();
    int                             _getLinedWords();
        
    void                            _checkString(string & inputString, char *checkBuf, string checkString);
    
protected:	
    string                          rawText;
	ofxSosoTrueTypeFont				*font;  

    float                           pointSize,
                                    leading;
    float                           scaleFactor;
    
    wordBlock                       blankSpaceWord;
    wordBlock                       newLineWord; 
    
    int                             alignment;	    
    	    
    bool                            drawAsShapes,	
									drawWordColor; 
    
	float							columnWidth,
									columnHeight;

	bool							isDisplayListEnabled;
    
    vector<wordBlock>               words;
    vector<lineBlock>               lines;

        
    static vector<ofxTextFont *>	allFonts;				//Static repository of fonts for easy access.
};
