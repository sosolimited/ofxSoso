/*
 ofxSosoTrueTypeFont
 
 ofxSosoTrueTypeFont inherits from ofTrueTypeFont. It adds several features and is used by ofxTextObject.
 It loads kerning information. It allows for mipmapping of letter textures. It also enables parsing of
 named entities and Unicode byte sequences. It allows you to conveniently obtain the positions of all the letters
 in a formatted string.
 
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

#include <vector>
#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofConstants.h"
#include "ofPath.h"
#include "ofTexture.h"
#include "ofMesh.h"

//had to add these in for FT_UInt
#include "ft2build.h"
#include "freetype2/freetype.h"


//--------------------------------------------------
typedef struct {
	int character;
	int height;
	int width;
	int setWidth;
	int topExtent;
	int leftExtent;
	float tW,tH;
	float x1,x2,y1,y2;
	float t1,t2,v1,v2;
} sosoCharProps;

typedef ofPath ofTTFCharacter;

//--------------------------------------------------
#define NUM_CHARACTER_TO_START		33		// 0 - 32 are control characters, no graphics needed.

///// \todo?
//const static string OF_TTF_SANS = "sans-serif";
//const static string OF_TTF_SERIF = "serif";
//const static string OF_TTF_MONO = "monospace";

//#include "ofTrueTypeFont.h"

#define FONT_NUM_CHARS (255 - NUM_CHARACTER_TO_START)

//Helper class for mapping higher Unicode characters down into the 0-255 range.
class ofxSosoMappedChar
{
public:
	ofxSosoMappedChar(unsigned char iMapToIndex, int iUnicodeIndex,  string iNamedEntity, char iUTFByte0, char iUTFByte1=0, char iUTFByte2=0, char iUTFByte3=0, char iUTFByte4=0, char iUTFByte5=0);
	~ofxSosoMappedChar();
  
public:
	unsigned char		mapToIndex;
	FT_ULong        unicodeIndex;
	string          utf8Sequence;
	string          namedEntity;
};


class ofxSosoTrueTypeFont
{
public:
  ofxSosoTrueTypeFont();
	~ofxSosoTrueTypeFont();
	
	// set the default dpi for all typefaces.
	/// \todo
	static void setGlobalDpi(int newDpi);
	
	bool isLoaded();
	bool isAntiAliased();
	bool hasFullCharacterSet();
	int getSize();
	
	float getLineHeight();
	void setLineHeight(float height);
	
	float getLetterSpacing();
	void setLetterSpacing(float spacing);

	float getSpaceSize();
	void setSpaceSize(float size);
	
	int	getNumCharacters();
	
	/// \todo
	ofTTFCharacter getCharacterAsPoints(int character, bool vflip=ofIsVFlipped());
	vector<ofTTFCharacter> getStringAsPoints(string str, bool vflip=ofIsVFlipped());
	ofMesh & getStringMesh(string s, float x, float y);
	ofTexture & getFontTexture();
	
	/// \todo
	void bind();
	void unbind();
	ofTextEncoding getEncoding() const;
	void setEncoding(ofTextEncoding encoding);
  
  
	//overridden methods from ofTrueTypeFont
  bool                    loadFont(string filename, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=true, bool makeContours=false, bool makeMipMaps=true, float simplifyAmt=0.3, int dpi=72);
  
  void                    drawString(string c, float x, float y);
  void                    drawChar(int c, float x, float y);
	void                    drawStringAsShapes(string s, float x, float y);
  
	//overridden for special Unicode character replacements
	float                   stringWidth(string s);
  float                   stringHeight(string s);
	ofRectangle             getStringBoundingBox(string c, float x, float y);
  
	//new methods
  vector <ofVec2f>        getCharPositions(string s, float x=0, float y=0);
	FT_UInt                 getFTCharIndex(FT_Face iFace, unsigned char iIndex);

	
	float                   getKerningAdjustment(int c1, int c2);
	void                    setKerningPair(unsigned char iChar0, unsigned char iChar1, int iUnits);
	void                    enableKerning(bool iEnable);
	void                    printNonzeroKerningPairs();

	static void             removeCharacters(string &iString, string iCharsToRemove);
  
  static int              getMappedChar(string iString, int &iIndex);
  char*                   getMappedCharSequence(string iString, int &iIndex);
  
  static string           convertStringTo255(string iString);
  
private:
  void                    buildMappedChars();
  
	static vector<ofxSosoMappedChar *>		mappedChars;

	static bool             areMappedCharsBuilt;
	
protected:
  //these are private in ofTrueTypeFont, so we override them to get access
  void                    unloadTextures();
	void                    reloadTextures();
  
protected:
	
	bool bLoadedOk;
	bool bAntiAliased;
	bool bFullCharacterSet;
	int nCharacters;
	
	vector <ofTTFCharacter> charOutlines;
	vector <ofTTFCharacter> charOutlinesNonVFlipped;
	
	float lineHeight;
	float letterSpacing;
	float spaceSize;
	
	vector<sosoCharProps> cps; // properties for each character
	
	int fontSize;
	bool bMakeContours;
	float simplifyAmt;
	int dpi;
	
	void drawCharAsShape(int c, float x, float y);
	void createStringMesh(string s, float x, float y);
	
	int border;
	string filename;
	
	ofTexture texAtlas;
	bool binded;
	ofMesh stringQuads;
	
	bool                    isKerningEnabled;
	int                     kerningPairs[FONT_NUM_CHARS][FONT_NUM_CHARS];
	
private:
#if defined(TARGET_ANDROID) || defined(TARGET_OF_IOS)
	friend void ofUnloadAllFontTextures();
	friend void ofReloadAllFontTextures();
#endif
	
	GLint blend_src, blend_dst;
	GLboolean blend_enabled;
	GLboolean texture_2d_enabled;
	
	ofTextEncoding encoding;
	static bool	initLibraries();
	static void finishLibraries();
	
	friend void ofExitCallback();
  

};
