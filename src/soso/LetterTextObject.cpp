#include "soso/LetterTextObject.h"

using namespace std;
using namespace cinder;
using namespace soso;

//
//	Letter
//

LetterTextObject::Letter::Letter( gl::TextureFontRef iFont, const GlyphMeasure &iGlyph, float iX, float iY, float iScaleFactor ):
	font( iFont ),
	glyph( iGlyph ),
	home( iX, iY, 0 ),
	scaleFactor( iScaleFactor )
{
	hasSpecialTransparency = true;
}

void LetterTextObject::Letter::render()
{
	glPushMatrix();
	glScalef(scaleFactor, scaleFactor, 1.0);
	font->drawGlyphs( { glyph }, Vec2f( 0, 0 ) );
	glPopMatrix();
}

//
// LetterTextObject
//

LetterTextObject::LetterTextObject( gl::TextureFontRef iFont, const string &iString ):
	font( iFont ),
	text( iString )
{
	//Reconstruct the letter objects based on the updated formatting.
	rebuildLetters();
}

void LetterTextObject::rebuildLetters()
{
	letters.clear();
	auto placements = font->getGlyphPlacements( text );
	for( const GlyphMeasure &glyph : placements )
	{
		Vec3f pos( glyph.second, 0.0f );
		auto letter = make_shared<Letter>( font, glyph, pos.x, pos.y, 1.0f );
		letter->setTrans( pos );
		letter->setColor( material->color );
		letters.push_back( letter );
		addChild( letter );
	}
}

/*
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
*/

//Empty render() because the letter objects handle all of the drawing.
void LetterTextObject::render(){}


void LetterTextObject::setString(const string &iString)
{
	text = iString;

	rebuildLetters();
}

void LetterTextObject::setColumnWidth(float iWidth)
{
	columnWidth = iWidth;
	rebuildLetters();
}

/*
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
*/
void LetterTextObject::setFont( gl::TextureFontRef iFont )
{
	font = iFont;
	rebuildLetters();
}


void LetterTextObject::setColor(float iR, float iG, float iB, float iA)
{
	Object::setColor(iR, iG, iB, iA);
	for (int i=0; i < letters.size(); i++)
		letters[i]->setColor(iR, iG, iB, iA);
}

void LetterTextObject::setColor(ci::Vec4f iColor)
{
	Object::setColor(iColor);
	for (int i=0; i < letters.size(); i++)
		letters[i]->setColor(iColor);
}

void LetterTextObject::setColor(ci::ColorA8u iColor)
{
	Object::setColor(iColor);
	for (int i=0; i < letters.size(); i++)
		letters[i]->setColor(iColor);
}
