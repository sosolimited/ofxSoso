#include "soso/LetterTextObject.h"

using namespace soso;

//class LetterTextObjectLetter
//LetterTextObjectLetter::LetterTextObjectLetter(SosoTrueTypeFont *iFont, char iChar, float iX, float iY, float iScaleFactor)
LetterTextObjectLetter::LetterTextObjectLetter(SosoTrueTypeFont *iFont, char *iChar, float iX, float iY, float iScaleFactor)   //eg 0701412
{
	//character = iChar;
    if(iChar) character = iChar[0]; //If iChar is a mapped sequence, then this is irrelevant.
    else character = 0;
	//charPointer = new char[2];
	//charPointer[0] = iChar;
	//charPointer[1] = 0;
    charPointer = iChar;    //eg 0701412
	scaleFactor = iScaleFactor;

	home.set(iX, iY, 0);
	font = iFont;

	hasSpecialTransparency = true;

}

LetterTextObjectLetter::~LetterTextObjectLetter()
{
    if(charPointer) delete(charPointer);    //eg 0701412
	//if(font) delete(font); //LM 070612
}

void LetterTextObjectLetter::render()
{
	glPushMatrix();
    glScalef(scaleFactor, scaleFactor, 1.0);
	font->drawString(charPointer,0,0);
	glPopMatrix();
}






//class LetterTextObject
LetterTextObject::LetterTextObject(SosoTrueTypeFont *iFont, char *iString):TextObject(iFont, iString)
{
	//Format the text with a call to drawText(). Note last arg is false, which says format, but do not draw.
	//drawText(0,0,false);
	//Reconstruct the letter objects based on the updated formatting.
	rebuildLetters();
}

LetterTextObject::~LetterTextObject(){

    for ( int i = 0; i < letters.size(); i++ )
    {
        delete letters[i];
    }

    letters.clear();

}



void LetterTextObject::cleanupLetters()
{
	for(unsigned int i=0; i < letters.size(); i++){
		removeChild(letters[i]);
		delete(letters[i]);
	}
	letters.clear();

	/*while (letters.size() > 0) {
		removeChild(letters[0]);
		delete(letters[0]);
		letters.erase(letters.begin()); //LM 070612
	}*/
}

void LetterTextObject::rebuildLetters()
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
					ci::vec3 pos((words[currentWordID].pos.x + words[currentWordID].charPositions[p].x) * scaleFactor,
								 (words[currentWordID].pos.y + words[currentWordID].charPositions[p].y) * scaleFactor,
								 0);

                    //Check for special unicode sequences, as defined in buildMappedChars() //eg 0701412
                    char *cSeq = NULL; //getFont()->getMappedCharSequence(words[currentWordID].rawWord, p);
                    if(cSeq==NULL){    //If, not a mapped char sequence, just grab character straight from word.
                        cSeq = new char[2];
                        cSeq[0] = words[currentWordID].rawWord.c_str()[p];
                        cSeq[1] = 0;
                    }

					//Create letter text letter and pass position and char to it.
					//LetterTextObjectLetter *letter = new LetterTextObjectLetter(font, words[currentWordID].rawWord.c_str()[p], pos.x, pos.y, scaleFactor);
                    LetterTextObjectLetter *letter = new LetterTextObjectLetter(font, cSeq, pos.x, pos.y, scaleFactor);       //eg 070412
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
void LetterTextObject::predraw()
{
	Object::predraw();

	glPushMatrix();
    glScalef(scaleFactor, scaleFactor, 1.0);
}

void LetterTextObject::postdraw()
{
	glPopMatrix();
	Object::postdraw();
}
*/

void LetterTextObject::setWordColor(int iIndex, float iR, float iG, float iB, float iA)
{
    TextObject::setWordColor(iIndex, iR, iG, iB, iA);
    rebuildLetters();
}

void LetterTextObject::setWordColor(string iWord, float iR, float iG, float iB, float iA, bool iExactMatch)
{
    TextObject::setWordColor(iWord, iR, iG, iB, iA, iExactMatch);
    rebuildLetters();

}

//Empty render() because the letter objects handle all of the drawing.
void LetterTextObject::render(){}


void LetterTextObject::setString(char *iString)
{
	TextObject::setString(iString);
	//drawText(0,0,false);
	rebuildLetters();
}

void LetterTextObject::setString(string iString) //JM 080513
{
	setString((char*)iString.c_str());
}

void LetterTextObject::setColumnWidth(float iWidth)
{
	TextObject::setColumnWidth(iWidth);
	//drawText(0,0,false);
	rebuildLetters();
}

void LetterTextObject::setPointSize(float iPointSize)
{
	TextObject::setPointSize(iPointSize);
	//drawText(0,0,false);
	rebuildLetters();
}

void LetterTextObject::setLeading(float iLeading)
{
	TextObject::setLeading(iLeading);
	//drawText(0,0,false);
	rebuildLetters();
}

void LetterTextObject::setSpaceWidth(float iWidth)
{
	TextObject::setSpaceWidth(iWidth);
	//drawText(0,0,false);
	rebuildLetters();
}

void LetterTextObject::setAlignment(TextObjectAlignment iAlignment)
{
	TextObject::setAlignment(iAlignment);
	rebuildLetters();
}

void LetterTextObject::setFont(SosoTrueTypeFont *iFont)
{
	TextObject::setFont(iFont);
	//drawText(0,0,false);
	rebuildLetters();
}


void LetterTextObject::setColor(float iR, float iG, float iB, float iA)
{
    TextObject::setColor(iR, iG, iB, iA);
    for (int i=0; i < letters.size(); i++)
        letters[i]->setColor(iR, iG, iB, iA);
}

void LetterTextObject::setColor(ci::vec4 iColor)
{
    TextObject::setColor(iColor);
    for (int i=0; i < letters.size(); i++)
        letters[i]->setColor(iColor);
}

void LetterTextObject::setColor(ci::ColorA8u iColor)
{
    TextObject::setColor(iColor);
    for (int i=0; i < letters.size(); i++)
        letters[i]->setColor(iColor);
}



/*
void LetterTextObject::normalizeScale()
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