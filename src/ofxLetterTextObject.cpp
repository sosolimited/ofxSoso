#include "ofxLetterTextObject.h"

//class ofxLetterTextObjectLetter
ofxLetterTextObjectLetter::ofxLetterTextObjectLetter(ofxSosoTrueTypeFont *iFont, string iString, float iX, float iY, float iScaleFactor)
{
	letter = iString;
	scaleFactor = iScaleFactor;
  
	home.set(iX, iY, 0);
	font = iFont;
	
	hasSpecialTransparency = true;
  
}

// Destructor.
ofxLetterTextObjectLetter::~ofxLetterTextObjectLetter()
{

}

void ofxLetterTextObjectLetter::render()
{
	glPushMatrix();
  glScalef(scaleFactor, scaleFactor, 1.0);
	font->drawString(letter,0,0);
	glPopMatrix();
}

//--------------------------------------------------------------------------------------------------------

//class ofxLetterTextObject
ofxLetterTextObject::ofxLetterTextObject(ofxSosoTrueTypeFont *iFont, string iString):ofxTextObject(iFont, iString)
{
	//Reconstruct the letter objects based on the updated formatting.
	rebuildLetters();
}

// Destructor.
ofxLetterTextObject::~ofxLetterTextObject()
{
  cleanupLetters();
}

void ofxLetterTextObject::cleanupLetters()
{
	for(unsigned int i=0; i < letters.size(); i++){
		removeChild(letters[i]);
		delete(letters[i]);
	}
	letters.clear();
}

void ofxLetterTextObject::rebuildLetters()
{
	int     currentWordID;
  
	cleanupLetters();
  
	if (words.size() > 0) {
    
		for(int a=0; a < lines.size(); a++)
		{
			for(int w=0; w < lines[a].wordsID.size(); w++)
			{
				currentWordID = lines[a].wordsID[w];
				
				//Make letters.
				for(int p=0; p < words[currentWordID].charPositions.size(); p++){
					//Add word position to char position.
					ofVec3f pos((words[currentWordID].pos.x + words[currentWordID].charPositions[p].x) * scaleFactor,
                      (words[currentWordID].pos.y + words[currentWordID].charPositions[p].y) * scaleFactor,
                      0);
          
          //grab the letter from the converted string
          string c = words[currentWordID].convertedWord.substr(p, 1);
          
					//Create letter text letter and pass position and char to it.
          ofxLetterTextObjectLetter *letter = new ofxLetterTextObjectLetter(font, c, pos.x, pos.y, scaleFactor);       //eg 070412
					letter->setTrans(pos.x, pos.y, pos.z);
					letter->setColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);	//Gotta grab word color or else it gets reset to white.
					letters.push_back(letter);
					
					addChild(letter);
				}
			}
		}
	}
}


void ofxLetterTextObject::setWordColor(int iIndex, float iR, float iG, float iB, float iA)
{
  ofxTextObject::setWordColor(iIndex, iR, iG, iB, iA);
  rebuildLetters();
}

void ofxLetterTextObject::setWordColor(string iWord, float iR, float iG, float iB, float iA, bool iExactMatch)
{
  ofxTextObject::setWordColor(iWord, iR, iG, iB, iA, iExactMatch);
  rebuildLetters();
  
}

//Empty render() because the letter objects handle all of the drawing.
void ofxLetterTextObject::render(){

//	ofLogNotice("drawing letter obj");
//	ofSetColor(0, 0, 255, 255);
//	ofRect(0,0,10,10);
}

void ofxLetterTextObject::setString(string iString) //JM 080513
{
  ofxTextObject::setString(iString);
  rebuildLetters();
}

void ofxLetterTextObject::setColumnWidth(float iWidth)
{
	ofxTextObject::setColumnWidth(iWidth);
	rebuildLetters();
}

void ofxLetterTextObject::setPointSize(float iPointSize)
{
	ofxTextObject::setPointSize(iPointSize);
	rebuildLetters();
}

void ofxLetterTextObject::setLeading(float iLeading)
{
	ofxTextObject::setLeading(iLeading);
	rebuildLetters();
}

void ofxLetterTextObject::setSpaceWidth(float iWidth)
{
	ofxTextObject::setSpaceWidth(iWidth);
	rebuildLetters();
}

void ofxLetterTextObject::setAlignment(TextObjectAlignment iAlignment)
{
	ofxTextObject::setAlignment(iAlignment);
	rebuildLetters();
}

void ofxLetterTextObject::setFont(ofxSosoTrueTypeFont *iFont)
{
	ofxTextObject::setFont(iFont);
	rebuildLetters();
}


void ofxLetterTextObject::setColor(float iR, float iG, float iB, float iA)
{
  ofxTextObject::setColor(iR, iG, iB, iA);
  for (int i=0; i < letters.size(); i++)
    letters[i]->setColor(iR, iG, iB, iA);
}

void ofxLetterTextObject::setColor(ofVec4f iColor)
{
  ofxTextObject::setColor(iColor);
  for (int i=0; i < letters.size(); i++)
    letters[i]->setColor(iColor);
}

void ofxLetterTextObject::setColor(ofColor iColor)
{
  ofxTextObject::setColor(iColor);
  for (int i=0; i < letters.size(); i++)
    letters[i]->setColor(iColor);
}

