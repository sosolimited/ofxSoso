#include "ofxLetterTextObject.h"

//class ofxLetterTextObjectLetter 
ofxLetterTextObjectLetter::ofxLetterTextObjectLetter(ofxSosoTrueTypeFont *iFont, char iChar, float iX, float iY, float iScaleFactor)
{
	character = iChar;
	charPointer = new char[2];
	charPointer[0] = iChar;
	charPointer[1] = 0;
	scaleFactor = iScaleFactor;

	home.set(iX, iY, 0);	
	font = iFont;
	
	hasSpecialTransparency = true;	

}

ofxLetterTextObjectLetter::~ofxLetterTextObjectLetter()
{
	delete(charPointer);
}

void ofxLetterTextObjectLetter::render()
{	    
	glPushMatrix();
    glScalef(scaleFactor, scaleFactor, 1.0);
	font->drawString(charPointer,0,0);	
	glPopMatrix();
}






//class ofxLetterTextObject
ofxLetterTextObject::ofxLetterTextObject(ofxSosoTrueTypeFont *iFont, char *iString):ofxTextObject(iFont, iString)
{	
	//Format the text with a call to drawText(). Note last arg is false, which says format, but do not draw.
	//drawText(0,0,false);		
	//Reconstruct the letter objects based on the updated formatting.
	rebuildLetters();
}

ofxLetterTextObject::~ofxLetterTextObject(){}



void ofxLetterTextObject::cleanupLetters()
{
	for(unsigned int i=0; i < letters.size(); i++){
		removeChild(letters[i]);
		delete(letters[i]);
		letters.erase(letters.begin() + i);
		i--;
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
					
					//Create letter text letter and pass position and char to it.
					ofxLetterTextObjectLetter *letter = new ofxLetterTextObjectLetter(font, words[currentWordID].rawWord.c_str()[p], pos.x, pos.y, scaleFactor); 					
					letter->setTrans(pos.x, pos.y, pos.z);
					letter->setColor(words[currentWordID].color.r, words[currentWordID].color.g, words[currentWordID].color.b, words[currentWordID].color.a);	//Gotta grab word color or else it gets reset to white.					
					letters.push_back(letter);
					addChild(letter);								
				}
			}
		}
	}
}

/*
void ofxLetterTextObject::predraw()
{
	ofxObject::predraw();

	glPushMatrix();
    glScalef(scaleFactor, scaleFactor, 1.0);
}

void ofxLetterTextObject::postdraw()
{
	glPopMatrix();    
	ofxObject::postdraw();
}
*/

//Empty render() because the letter objects handle all of the drawing.
void ofxLetterTextObject::render(){}


void ofxLetterTextObject::setString(char *iString)
{	
	ofxTextObject::setString(iString);
	//drawText(0,0,false);	
	rebuildLetters();	
}

void ofxLetterTextObject::setColumnWidth(float iWidth)
{
	ofxTextObject::setColumnWidth(iWidth);
	//drawText(0,0,false);	
	rebuildLetters();		
}

void ofxLetterTextObject::setPointSize(float iPointSize)
{
	ofxTextObject::setPointSize(iPointSize);
	//drawText(0,0,false);	
	rebuildLetters();		
}

void ofxLetterTextObject::setLeading(float iLeading)
{
	ofxTextObject::setLeading(iLeading);
	//drawText(0,0,false);	
	rebuildLetters();		
}

void ofxLetterTextObject::setSpaceWidth(float iWidth)
{
	ofxTextObject::setSpaceWidth(iWidth);
	//drawText(0,0,false);	
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
	//drawText(0,0,false);	
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




/*
void ofxLetterTextObject::normalizeScale()
{       
    float textScale = getScale()[0];

    //scale words + home positions
    for(unsigned int i=0; i < letters.size(); i++){
        letters[i]->setScale(textScale/getPointSize());       
        //mBaselinePositions[i] *= textScale;      
		letters[i]->home *= (textScale/getPointSize());
        //mQuads[i]->setTrans(*mBaselinePositions[i]);
		letters[i]->setTrans(letters[i]->home);
    }
   
    setScale(getPointSize());   
}
*/