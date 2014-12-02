//
//  ofxSosoTrueTypeFont.cpp
//  sosoExample
//
//  Created by Eric Gunther on 2/9/12.
//  Copyright 2012 Sosolimited. All rights reserved.
//

#include <iostream>
#include "ofxSosoTrueTypeFont.h"


#include "ft2build.h"
#include "freetype2/freetype/freetype.h"
#include "freetype2/freetype/ftglyph.h"
#include "freetype2/freetype/ftoutln.h"
#include "freetype2/freetype/fttrigon.h"

#include <algorithm>

#include "ofUtils.h"
#include "ofGraphics.h"
#include "ofTexture.h"  //for mipmap building

//helper class for mapping higher Unicode characters down into the 0-255 range
ofxSosoMappedChar::ofxSosoMappedChar(unsigned char iMapToIndex, int iUnicodeIndex, string iNamedEntity, char iUTFByte0, char iUTFByte1, char iUTFByte2, char iUTFByte3, char iUTFByte4, char iUTFByte5)

{
	//index within 0-255 range to which we're mapping
	mapToIndex = iMapToIndex;
	
  //standard unicode index
	unicodeIndex = iUnicodeIndex;

	
  //HTML named entity
	namedEntity = iNamedEntity;
	
  //UTF-8 byte sequence (not the unicode val:0x00A9, but the utf-8 bytes: 0xC2, 0xA9)
  utf8Sequence = "";
  
  if (iUTFByte0 != 0) utf8Sequence += iUTFByte0;
  if (iUTFByte1 != 0) utf8Sequence += iUTFByte1;
  if (iUTFByte2 != 0) utf8Sequence += iUTFByte2;
  if (iUTFByte3 != 0) utf8Sequence += iUTFByte3;
  if (iUTFByte4 != 0) utf8Sequence += iUTFByte4;
  if (iUTFByte5 != 0) utf8Sequence += iUTFByte5;
  
}


ofxSosoMappedChar::~ofxSosoMappedChar()
{
  //destructor is currently empty
}



//BAD! static global vars in ofTrueTypeFont.cpp
static bool printVectorInfo = false;
static int ttfGlobalDpi = 72;   //this is standard dpi to get you pixel-accurate pointsizes. OF uses 96, which yields incorrect point-sizing

bool ofxSosoTrueTypeFont::areMappedCharsBuilt = false;
vector<ofxSosoMappedChar *> ofxSosoTrueTypeFont::mappedChars;


//BAD! static global functions in ofTrueTypeFont.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ofTTFCharacter makeContoursForCharacter(FT_Face &face);
static ofTTFCharacter makeContoursForCharacter(FT_Face &face){
  
  //int num			= face->glyph->outline.n_points;
  int nContours	= face->glyph->outline.n_contours;
  int startPos	= 0;
  
  char * tags		= face->glyph->outline.tags;
  FT_Vector * vec = face->glyph->outline.points;
  
  ofTTFCharacter charOutlines;
  charOutlines.setUseShapeColor(false);
  
  for(int k = 0; k < nContours; k++){
    if( k > 0 ){
      startPos = face->glyph->outline.contours[k-1]+1;
    }
    int endPos = face->glyph->outline.contours[k]+1;
    
    if( printVectorInfo )printf("--NEW CONTOUR\n\n");
    
    //vector <ofPoint> testOutline;
    ofPoint lastPoint;
    
    for(int j = startPos; j < endPos; j++){
      
      if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON ){
        lastPoint.set((float)vec[j].x, (float)-vec[j].y, 0);
        if( printVectorInfo )printf("flag[%i] is set to 1 - regular point - %f %f \n", j, lastPoint.x, lastPoint.y);
        //testOutline.push_back(lastPoint);
        charOutlines.lineTo(lastPoint/64);
        
      }else{
        if( printVectorInfo )printf("flag[%i] is set to 0 - control point \n", j);
        
        if( FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CUBIC ){
          if( printVectorInfo )printf("- bit 2 is set to 2 - CUBIC\n");
          
          int prevPoint = j-1;
          if( j == 0){
            prevPoint = endPos-1;
          }
          
          int nextIndex = j+1;
          if( nextIndex >= endPos){
            nextIndex = startPos;
          }
          
          ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );
          
          //we need two control points to draw a cubic bezier
          bool lastPointCubic =  ( FT_CURVE_TAG(tags[prevPoint]) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG(tags[prevPoint]) == FT_CURVE_TAG_CUBIC);
          
          if( lastPointCubic ){
            ofPoint controlPoint1((float)vec[prevPoint].x,	(float)-vec[prevPoint].y);
            ofPoint controlPoint2((float)vec[j].x, (float)-vec[j].y);
            ofPoint nextPoint((float) vec[nextIndex].x,	-(float) vec[nextIndex].y);
            
            //cubic_bezier(testOutline, lastPoint.x, lastPoint.y, controlPoint1.x, controlPoint1.y, controlPoint2.x, controlPoint2.y, nextPoint.x, nextPoint.y, 8);
            charOutlines.bezierTo(controlPoint1.x/64, controlPoint1.y/64, controlPoint2.x/64, controlPoint2.y/64, nextPoint.x/64, nextPoint.y/64);
          }
          
        }else{
          
          ofPoint conicPoint( (float)vec[j].x,  -(float)vec[j].y );
          
          if( printVectorInfo )printf("- bit 2 is set to 0 - conic- \n");
          if( printVectorInfo )printf("--- conicPoint point is %f %f \n", conicPoint.x, conicPoint.y);
          
          //If the first point is connic and the last point is connic then we need to create a virutal point which acts as a wrap around
          if( j == startPos ){
            bool prevIsConnic = (  FT_CURVE_TAG( tags[endPos-1] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[endPos-1]) != FT_CURVE_TAG_CUBIC );
            
            if( prevIsConnic ){
              ofPoint lastConnic((float)vec[endPos - 1].x, (float)-vec[endPos - 1].y);
              lastPoint = (conicPoint + lastConnic) / 2;
              
              if( printVectorInfo )	printf("NEED TO MIX WITH LAST\n");
              if( printVectorInfo )printf("last is %f %f \n", lastPoint.x, lastPoint.y);
            }
          }
          
          //bool doubleConic = false;
          
          int nextIndex = j+1;
          if( nextIndex >= endPos){
            nextIndex = startPos;
          }
          
          ofPoint nextPoint( (float)vec[nextIndex].x,  -(float)vec[nextIndex].y );
          
          if( printVectorInfo )printf("--- last point is %f %f \n", lastPoint.x, lastPoint.y);
          
          bool nextIsConnic = (  FT_CURVE_TAG( tags[nextIndex] ) != FT_CURVE_TAG_ON ) && ( FT_CURVE_TAG( tags[nextIndex]) != FT_CURVE_TAG_CUBIC );
          
          //create a 'virtual on point' if we have two connic points
          if( nextIsConnic ){
            nextPoint = (conicPoint + nextPoint) / 2;
            if( printVectorInfo )printf("|_______ double connic!\n");
          }
          if( printVectorInfo )printf("--- next point is %f %f \n", nextPoint.x, nextPoint.y);
          
          //quad_bezier(testOutline, lastPoint.x, lastPoint.y, conicPoint.x, conicPoint.y, nextPoint.x, nextPoint.y, 8);
          charOutlines.quadBezierTo(lastPoint.x/64, lastPoint.y/64, conicPoint.x/64, conicPoint.y/64, nextPoint.x/64, nextPoint.y/64);
          
          if( nextIsConnic ){
            lastPoint = nextPoint;
          }
        }
      }
      
			//end for
    }
    charOutlines.close();
  }
  
	return charOutlines;
}

//had to do this for windows version, on mac it was ok to redefine it here
/*
 bool compare_cps(const charProps & c1, const charProps & c2){
 if(c1.tH == c2.tH) return c1.tW > c2.tW;
 else return c1.tH > c2.tH;
 }
 */
extern bool compare_cps(const charProps & c1, const charProps & c2);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



ofxSosoTrueTypeFont::ofxSosoTrueTypeFont()
{
	buildMappedChars();
  
	isKerningEnabled = true;
}

ofxSosoTrueTypeFont::~ofxSosoTrueTypeFont(){ //LM 070612

  cout<<"0 mappedChars size = "<< mappedChars.size() <<endl;
  
  for ( int i=0; i < mappedChars.size(); i++ )
  {
    delete mappedChars[i];
  }
	
  cout<<"1 mappedChars size = "<< mappedChars.size() <<endl;
	mappedChars.clear();
	
  cout<<"2 mappedChars size = "<< mappedChars.size() <<endl;

}



//-----------------------------------------------------------
bool ofxSosoTrueTypeFont::loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours, bool makeMipMaps, float simplifyAmt, int dpi){	//soso - added makeMipMaps (see below)
  
  
	bMakeContours = makeContours;
  
	//------------------------------------------------
	if (bLoadedOk == true){
    
		// we've already been loaded, try to clean up :
		unloadTextures();
	}
	//------------------------------------------------
  
	if( dpi == 0 ){
		dpi = ttfGlobalDpi;
	}
  
	filename = ofToDataPath(filename);
  
	bLoadedOk 			= false;
	bAntiAliased 		= _bAntiAliased;
	bFullCharacterSet 	= _bFullCharacterSet;
	fontSize			= fontsize;
  
	//--------------- load the library and typeface
	
  FT_Error err;
  
  FT_Library library;
  if (err = FT_Init_FreeType( &library )){
		ofLog(OF_LOG_ERROR,"ofTrueTypeFont::loadFont - Error initializing freetype lib: FT_Error = %d", err);
		return false;
	}
  
	FT_Face face;
  
	if (err = FT_New_Face( library, filename.c_str(), 0, &face )) {
    // simple error table in lieu of full table (see fterrors.h)
    string errorString = "unknown freetype";
    if(err == 1) errorString = "INVALID FILENAME";
    ofLog(OF_LOG_ERROR,"ofTrueTypeFont::loadFont - %s: %s: FT_Error = %d", errorString.c_str(), filename.c_str(), err);
		return false;
	}
  
	//FT_Set_Char_Size( face, fontsize << 6, fontsize << 6, dpi, dpi); //of
	//FT_Set_Char_Size( face, 0, fontsize*dpi, 0, dpi); //soso
  FT_Set_Char_Size( face, 0, fontsize*64, 0, dpi); //soso
	
	lineHeight = fontsize * 1.43f;
  
	//------------------------------------------------------
	//kerning would be great to support:
	//ofLog(OF_LOG_NOTICE,"FT_HAS_KERNING ? %i", FT_HAS_KERNING(face));
	//------------------------------------------------------
  
	//nCharacters = bFullCharacterSet ? 256 : 128 - NUM_CHARACTER_TO_START;
	
  //nCharacters was 512, but we are mapping everything down to 0 to 255
  nCharacters = bFullCharacterSet ? 256 : 128 - NUM_CHARACTER_TO_START;

  
	//--------------- initialize character info and textures
	cps.resize(nCharacters);
  
	if(bMakeContours){
		charOutlines.clear();
		charOutlines.assign(nCharacters, ofTTFCharacter());
	}
  
	vector<ofPixels> expanded_data(nCharacters);
  
	long areaSum=0;
  
	//--------------------- load each char -----------------------
	for (int i = 0 ; i < nCharacters; i++){
    
    // AO
    int glyph = (unsigned char)(i+NUM_CHARACTER_TO_START);
    
    // TODO: May be handled by special chars map
    if (glyph == 0xA4) glyph = 0x20AC; // hack to load the euro sign, all codes in 8859-15 match with utf-32 except for this one
    
		//------------------------------------------ anti aliased or not:
		//if(err = FT_Load_Glyph( face, FT_Get_Char_Index( face, (unsigned char)(i+NUM_CHARACTER_TO_START) ), FT_LOAD_DEFAULT )){
		if((err = FT_Load_Glyph( face, getFTCharIndex( face, (unsigned char)(i+NUM_CHARACTER_TO_START) ), FT_LOAD_DEFAULT ))){		//soso replaced FT_Get_Char_Index with our custom version
			ofLog(OF_LOG_ERROR,"ofTrueTypeFont::loadFont - Error with FT_Load_Glyph %i: FT_Error = %d", i, err);
      
		}
    
		if (bAntiAliased == true) FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		else FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
    
		//------------------------------------------

    
    
		// prepare the texture:
		//int width  = ofNextPow2( bitmap.width + border*2 );
    // int height = ofNextPow2( bitmap.rows  + border*2 );
    
    
    //// ------------------------- this is fixing a bug with small type
    //// ------------------------- appearantly, opengl has trouble with
    //// ------------------------- width or height textures of 1, so we
    //// ------------------------- we just set it to 2...
    //if (width == 1) width = 2;
    //if (height == 1) height = 2;
    
    
		if(bMakeContours){
			if( printVectorInfo )printf("\n\ncharacter %c: \n", char( i+NUM_CHARACTER_TO_START ) );
      
			//int character = i + NUM_CHARACTER_TO_START;
			charOutlines[i] = makeContoursForCharacter( face );
			if(simplifyAmt>0)
				charOutlines[i].simplify(simplifyAmt);
			charOutlines[i].getTessellation();
		}
    
    
		// -------------------------
		// info about the character:
    FT_Bitmap& bitmap= face->glyph->bitmap;
    int width  = bitmap.width;
    int height = bitmap.rows;
    
		cps[i].characterIndex		= i;
    cps[i].glyph        = glyph;
		cps[i].height 			= face->glyph->metrics.height>>6;
		cps[i].width        = face->glyph->metrics.width>>6;
    cps[i].bearingX     = face->glyph->metrics.horiBearingX>>6;
    cps[i].bearingY     = face->glyph->metrics.horiBearingY>>6;
    cps[i].xmin         = face->glyph->bitmap_left;
    cps[i].xmax         = cps[i].xmin + cps[i].width;
    cps[i].ymin         = -face->glyph->bitmap_top;
    cps[i].ymax         = cps[i].ymin + cps[i].height;
    cps[i].advance      = face->glyph->metrics.horiAdvance>>6;
    
    
    cps[i].tW				= cps[i].width;
    cps[i].tH				= cps[i].height;
    
    areaSum += (cps[i].tW+border*2)*(cps[i].tH+border*2);
    
		GLint fheight	= cps[i].height;
		GLint bwidth	= cps[i].width;

		GLfloat	corr, stretch;
    
		//this accounts for the fact that we are showing 2*visibleBorder extra pixels
		//so we make the size of each char that many pixels bigger
		stretch = 0;//(float)(visibleBorder * 2);

    // TODO: Ask Eric about this correction (AO)
//		corr	= (float)(( (fontSize - fheight) + top) - fontSize);
    

		// Allocate Memory For The Texture Data.
		expanded_data[i].allocate(width, height, 2);
		//-------------------------------- clear data:
		expanded_data[i].set(0,255); // every luminance pixel = 255
		expanded_data[i].set(1,0);
    
    
		if (bAntiAliased == true){
			ofPixels bitmapPixels;
			bitmapPixels.setFromExternalPixels(bitmap.buffer,bitmap.width,bitmap.rows,1);
			expanded_data[i].setChannel(1,bitmapPixels);
		} else {
			//-----------------------------------
			// true type packs monochrome info in a
			// 1-bit format, hella funky
			// here we unpack it:
			unsigned char *src =  bitmap.buffer;
			for(int j=0; j <bitmap.rows;j++) {
				unsigned char b=0;
				unsigned char *bptr =  src;
				for(int k=0; k < bitmap.width ; k++){
					expanded_data[i][2*(k+j*width)] = 255;
          
					if (k%8==0){
						b = (*bptr++);
					}
          
					expanded_data[i][2*(k+j*width) + 1] = b&0x80 ? 255 : 0;
					b <<= 1;
				}
				src += bitmap.pitch;
			}
			//-----------------------------------
		}
    
		
	}
  
  
	vector<charProps> sortedCopy = cps;
	sort(sortedCopy.begin(),sortedCopy.end(),&compare_cps);
  
	// pack in a texture, algorithm to calculate min w/h from
	// http://upcommons.upc.edu/pfc/bitstream/2099.1/7720/1/TesiMasterJonas.pdf
	//cout << areaSum << endl;
  
	bool packed = false;
	float alpha = logf(areaSum)*1.44269;
  
	int w;
	int h;
	while(!packed){
		w = pow(2,floor((alpha/2.f) + 0.5)); // there doesn't seem to be a round in cmath for windows.
		//w = pow(2,round(alpha/2.f));
		h = w;//pow(2,round(alpha - round(alpha/2.f)));
		int x=0;
		int y=0;
		int maxRowHeight = sortedCopy[0].tH + border*2;
		for(int i=0;i<(int)cps.size();i++){
			if(x+sortedCopy[i].tW + border*2>w){
				x = 0;
				y += maxRowHeight;
				maxRowHeight = sortedCopy[i].tH + border*2;
				if(y + maxRowHeight > h){
					alpha++;
					break;
				}
			}
			x+= sortedCopy[i].tW + border*2;
			if(i==(int)cps.size()-1) packed = true;
		}
    
	}
  
  
  // TODO:
	ofPixels atlasPixels;
	atlasPixels.allocate(w,h,2);
	atlasPixels.set(0,255);
	atlasPixels.set(1,0);
  
  
	int x=0;
	int y=0;
	int maxRowHeight = sortedCopy[0].tH + border*2;
	for(int i=0;i<(int)cps.size();i++){

    ofPixels & charPixels = expanded_data[sortedCopy[i].characterIndex];
    
		if(x+sortedCopy[i].tW + border*2>w){
			x = 0;
			y += maxRowHeight;
			maxRowHeight = sortedCopy[i].tH + border*2;
		}

    // alex
    cps[sortedCopy[i].characterIndex].t1		= float(x + border)/float(w);
    cps[sortedCopy[i].characterIndex].v1		= float(y + border)/float(h);
    cps[sortedCopy[i].characterIndex].t2		= float(cps[sortedCopy[i].characterIndex].tW + x + border)/float(w);
    cps[sortedCopy[i].characterIndex].v2		= float(cps[sortedCopy[i].characterIndex].tH + y + border)/float(h);

    charPixels.pasteInto(atlasPixels,x+border,y+border);
    
		x+= sortedCopy[i].tW + border*2;
    
	}
  
  
	texAtlas.allocate(atlasPixels.getWidth(),atlasPixels.getHeight(),GL_LUMINANCE_ALPHA,false);
  
	if(bAntiAliased && fontsize>20){
		if (makeMipMaps) { //soso
			//texAtlas.enableMipmaps();
			//texAtlas.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR); //soso
      
		} else	//soso
			texAtlas.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);
	}else{
		texAtlas.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
	}
  
	texAtlas.loadData(atlasPixels.getPixels(),atlasPixels.getWidth(),atlasPixels.getHeight(),GL_LUMINANCE_ALPHA);
  
  
  ///////////////////////////////////////////////////////////////////////sosoAddon
  //until ofTexture fully supports mipmaps, we gotta do it manually here - AFTER loadData is called on the texture
  //it's a redo of what happens inside tex.loadData(), but instead we build the mipmaps
  if(makeMipMaps){
    glEnable(texAtlas.getTextureData().textureTarget);
    glBindTexture(texAtlas.getTextureData().textureTarget, (GLuint) texAtlas.getTextureData().textureID);
    
    glTexParameteri(texAtlas.getTextureData().textureTarget, GL_GENERATE_MIPMAP_SGIS, true);
    glTexParameteri( texAtlas.getTextureData().textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri( texAtlas.getTextureData().textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( texAtlas.getTextureData().textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri( texAtlas.getTextureData().textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(texAtlas.getTextureData().textureTarget, texAtlas.getTextureData().glTypeInternal,
                      w, h, texAtlas.getTextureData().glTypeInternal, ofGetGlTypeFromInternal(texAtlas.getTextureData().glTypeInternal), atlasPixels.getPixels());
    glDisable(texAtlas.getTextureData().textureTarget);
  }
  //////////////////////////////////////////////////////////////////////
  
  
	//Sosolimited - load kerning pairs
	
	//initialize all pairs to 0
	for (int i = 0; i < FONT_NUM_CHARS; i++) {
		for (int j = 0; j < FONT_NUM_CHARS; j++) {
			kerningPairs[i][j] = 0;
		}
	}
  
	//find out if the face has kerning
	FT_Bool use_kerning = (FT_Bool)FT_HAS_KERNING(face);
	if(!use_kerning) printf("ofxSosoTrueTypeFont::loadFont() - kerning is NOT supported for %s\n", (char*)filename.c_str());
	
	
	FT_UInt glyph_index_r, glyph_index_l;
	
	for (int i = 0; i < FONT_NUM_CHARS; i++) {
		// convert character code to glyph index
		glyph_index_r = FT_Get_Char_Index(face, i + NUM_CHARACTER_TO_START);
		
		for (int j = 0; j < FONT_NUM_CHARS; j++) {
			// convert character code to glyph index
			glyph_index_l = FT_Get_Char_Index(face, j + NUM_CHARACTER_TO_START);
      
			// retrieve kerning distance
			if (use_kerning  &&  glyph_index_l  &&  glyph_index_r) {
        
				FT_Vector  delta;
				FT_Get_Kerning( face, glyph_index_l, glyph_index_r, FT_KERNING_DEFAULT, &delta );
        
				kerningPairs[i][j] = delta.x >> 6;
        
				//if(i<127)
				//if(fabs((float)kerningPairs[i][j]) > 0) printf("kerningPairs: %c%c = %d, delta = %d\n", i + NUM_CHARACTER_TO_START, j + NUM_CHARACTER_TO_START, kerningPairs[i][j], delta.x);
			}
		}
	}
  
  //TODO: Soso - can we clean up the memory footprint here, by deleting the pixels
  //not using the pixels any more
  //atlasPixels.clear();
  
	// ------------- close the library and typeface
	FT_Done_Face(face);
	FT_Done_FreeType(library);
  bLoadedOk = true;
	return true;
}


FT_UInt ofxSosoTrueTypeFont::getFTCharIndex(FT_Face iFace, unsigned char iIndex)
{
	FT_UInt i = FT_Get_Char_Index(iFace, iIndex);
	
	//check if it's a special mapping (as specified in buildMappedChars())
	for(int a=0; a < mappedChars.size(); a++){
		if(iIndex == mappedChars[a]->mapToIndex){
			i = FT_Get_Char_Index(iFace, mappedChars[a]->unicodeIndex);
		}
	}
	
	return i;
}


void ofxSosoTrueTypeFont::buildMappedChars()
{
	if(!areMappedCharsBuilt){
		
    //UTF-8 byte sequences for special characters within the 0 to 255 unicode range
    mappedChars.push_back(new ofxSosoMappedChar(0x22, 0x0022, "&quot;", 0x22));
		mappedChars.push_back(new ofxSosoMappedChar(0x26, 0x0026, "&amp;", 0x26));
		mappedChars.push_back(new ofxSosoMappedChar(0x27, 0x0027, "&apos;", 0x27));
		mappedChars.push_back(new ofxSosoMappedChar(0x3C, 0x003C, "&lt;", 0x3C));
		mappedChars.push_back(new ofxSosoMappedChar(0x3E, 0x003E, "&gt;", 0x3E));
		mappedChars.push_back(new ofxSosoMappedChar(0xA0, 0x00A0, "&nbsp;", 0xC2, 0xA0));
		mappedChars.push_back(new ofxSosoMappedChar(0xA1, 0x00A1, "&iexcl;", 0xC2, 0xA1));
		mappedChars.push_back(new ofxSosoMappedChar(0xA2, 0x00A2, "&cent;", 0xC2, 0xA2));
		mappedChars.push_back(new ofxSosoMappedChar(0xA3, 0x00A3, "&pound;", 0xC2, 0xA3));
		mappedChars.push_back(new ofxSosoMappedChar(0xA4, 0x00A4, "&curren;", 0xC2, 0xA4));
		mappedChars.push_back(new ofxSosoMappedChar(0xA5, 0x00A5, "&yen;", 0xC2, 0xA5));
		mappedChars.push_back(new ofxSosoMappedChar(0xA6, 0x00A6, "&brvbar;", 0xC2, 0xA6));
		mappedChars.push_back(new ofxSosoMappedChar(0xA7, 0x00A7, "&sect;", 0xC2, 0xA7));
		mappedChars.push_back(new ofxSosoMappedChar(0xA8, 0x00A8, "&uml;", 0xC2, 0xA8));
    mappedChars.push_back(new ofxSosoMappedChar(0xA9, 0x00A9, "&copy;", 0xC2, 0xA9));
		mappedChars.push_back(new ofxSosoMappedChar(0xAA, 0x00AA, "&ordf;", 0xC2, 0xAA));
		mappedChars.push_back(new ofxSosoMappedChar(0xAB, 0x00AB, "&laquo;", 0xC2, 0xAB));
		mappedChars.push_back(new ofxSosoMappedChar(0xAC, 0x00AC, "&not;", 0xC2, 0xAC));
		mappedChars.push_back(new ofxSosoMappedChar(0xAD, 0x00AD, "&shy;", 0xC2, 0xAD));
		mappedChars.push_back(new ofxSosoMappedChar(0xAE, 0x00AE, "&reg;", 0xC2, 0xAE));
		mappedChars.push_back(new ofxSosoMappedChar(0xAF, 0x00AF, "&macr;", 0xC2, 0xAF));
		mappedChars.push_back(new ofxSosoMappedChar(0xB0, 0x00B0, "&deg;", 0xC2, 0xB0));
		mappedChars.push_back(new ofxSosoMappedChar(0xB1, 0x00B1, "&plusmn;", 0xC2, 0xB1));
		mappedChars.push_back(new ofxSosoMappedChar(0xB2, 0x00B2, "&sup2;", 0xC2, 0xB2));
		mappedChars.push_back(new ofxSosoMappedChar(0xB3, 0x00B3, "&sup3;", 0xC2, 0xB3));
		mappedChars.push_back(new ofxSosoMappedChar(0xB4, 0x00B4, "&acute;", 0xC2, 0xB4));
		mappedChars.push_back(new ofxSosoMappedChar(0xB5, 0x00B5, "&micro;", 0xC2, 0xB5));
		mappedChars.push_back(new ofxSosoMappedChar(0xB6, 0x00B6, "&para;", 0xC2, 0xB6));
		mappedChars.push_back(new ofxSosoMappedChar(0xB7, 0x00B7, "&middot;", 0xC2, 0xB7));
		mappedChars.push_back(new ofxSosoMappedChar(0xB8, 0x00B8, "&cedil;", 0xC2, 0xB8));
		mappedChars.push_back(new ofxSosoMappedChar(0xB9, 0x00B9, "&sup1;", 0xC2, 0xB9));
		mappedChars.push_back(new ofxSosoMappedChar(0xBA, 0x00BA, "&ordm;", 0xC2, 0xBA));
		mappedChars.push_back(new ofxSosoMappedChar(0xBB, 0x00BB, "&raquo;", 0xC2, 0xBB));
		mappedChars.push_back(new ofxSosoMappedChar(0xBC, 0x00BC, "&frac14;", 0xC2, 0xBC));
		mappedChars.push_back(new ofxSosoMappedChar(0xBD, 0x00BD, "&frac12;", 0xC2, 0xBD));
		mappedChars.push_back(new ofxSosoMappedChar(0xBE, 0x00BE, "&frac34;", 0xC2, 0xBE));
		mappedChars.push_back(new ofxSosoMappedChar(0xBF, 0x00BF, "&iquest;", 0xC2, 0xBF));
		mappedChars.push_back(new ofxSosoMappedChar(0xC0, 0x00C0, "&Agrave;", 0xC3, 0x80));
		mappedChars.push_back(new ofxSosoMappedChar(0xC1, 0x00C1, "&Aacute;", 0xC3, 0x81));
		mappedChars.push_back(new ofxSosoMappedChar(0xC2, 0x00C2, "&Acirc;", 0xC3, 0x82));
		mappedChars.push_back(new ofxSosoMappedChar(0xC3, 0x00C3, "&Atilde;", 0xC3, 0x83));
		mappedChars.push_back(new ofxSosoMappedChar(0xC4, 0x00C4, "&Auml;", 0xC3, 0x84));
		mappedChars.push_back(new ofxSosoMappedChar(0xC5, 0x00C5, "&Aring;", 0xC3, 0x85));
		mappedChars.push_back(new ofxSosoMappedChar(0xC6, 0x00C6, "&AElig;", 0xC3, 0x86));
		mappedChars.push_back(new ofxSosoMappedChar(0xC7, 0x00C7, "&Ccedil;", 0xC3, 0x87));
    mappedChars.push_back(new ofxSosoMappedChar(0xC8, 0x00C8, "&Egrave;", 0xC3, 0x88));
		mappedChars.push_back(new ofxSosoMappedChar(0xC9, 0x00C9, "&Eacute;", 0xC3, 0x89));
		mappedChars.push_back(new ofxSosoMappedChar(0xCA, 0x00CA, "&Ecirc;", 0xC3, 0x8A));
		mappedChars.push_back(new ofxSosoMappedChar(0xCB, 0x00CB, "&Euml;", 0xC3, 0x8B));
		mappedChars.push_back(new ofxSosoMappedChar(0xCC, 0x00CC, "&lgrave;", 0xC3, 0x8C));
		mappedChars.push_back(new ofxSosoMappedChar(0xCD, 0x00CD, "&lacute;", 0xC3, 0x8D));
		mappedChars.push_back(new ofxSosoMappedChar(0xCE, 0x00CE, "&lcirc;", 0xC3, 0x8E));
		mappedChars.push_back(new ofxSosoMappedChar(0xCF, 0x00CF, "&luml;", 0xC3, 0x8F));
		mappedChars.push_back(new ofxSosoMappedChar(0xD0, 0x00D0, "&ETH;", 0xC3, 0x90));
		mappedChars.push_back(new ofxSosoMappedChar(0xD1, 0x00D1, "&Ntilde;", 0xC3, 0x91));
		mappedChars.push_back(new ofxSosoMappedChar(0xD2, 0x00D2, "&Ograve;", 0xC3, 0x92));
		mappedChars.push_back(new ofxSosoMappedChar(0xD3, 0x00D3, "&Oacute;", 0xC3, 0x93));
		mappedChars.push_back(new ofxSosoMappedChar(0xD4, 0x00D4, "&Ocirc;", 0xC3, 0x94));
    mappedChars.push_back(new ofxSosoMappedChar(0xD5, 0x00D5, "&Otilde;", 0xC3, 0x95));
		mappedChars.push_back(new ofxSosoMappedChar(0xD6, 0x00D6, "&Ouml;", 0xC3, 0x96));
		mappedChars.push_back(new ofxSosoMappedChar(0xD7, 0x00D7, "&times;", 0xC3, 0x97));
		mappedChars.push_back(new ofxSosoMappedChar(0xD8, 0x00D8, "&Oslash;", 0xC3, 0x98));
		mappedChars.push_back(new ofxSosoMappedChar(0xD9, 0x00D9, "&Ugrave;", 0xC3, 0x99));
		mappedChars.push_back(new ofxSosoMappedChar(0xDA, 0x00DA, "&Uacute;", 0xC3, 0x9A));
		mappedChars.push_back(new ofxSosoMappedChar(0xDB, 0x00DB, "&Ucirc;", 0xC3, 0x9B));
		mappedChars.push_back(new ofxSosoMappedChar(0xDC, 0x00DC, "&Uuml;", 0xC3, 0x9C));
		mappedChars.push_back(new ofxSosoMappedChar(0xDD, 0x00DD, "&Yacute;", 0xC3, 0x94));
		mappedChars.push_back(new ofxSosoMappedChar(0xDE, 0x00DE, "&THORN;", 0xC3, 0x9E));
		mappedChars.push_back(new ofxSosoMappedChar(0xDF, 0x00DF, "&szlig;", 0xC3, 0x9F));
		mappedChars.push_back(new ofxSosoMappedChar(0xE0, 0x00E0, "&agrave;", 0xC3, 0xA0));
		mappedChars.push_back(new ofxSosoMappedChar(0xE1, 0x00E1, "&aacute;", 0xC3, 0xA1));
		mappedChars.push_back(new ofxSosoMappedChar(0xE2, 0x00E2, "&acirc;", 0xC3, 0xA2));
		mappedChars.push_back(new ofxSosoMappedChar(0xE3, 0x00E3, "&atilde;", 0xC3, 0xA3));
		mappedChars.push_back(new ofxSosoMappedChar(0xE4, 0x00E4, "&auml;", 0xC3, 0xA4));
		mappedChars.push_back(new ofxSosoMappedChar(0xE5, 0x00E5, "&aring;", 0xC3, 0xA5));
		mappedChars.push_back(new ofxSosoMappedChar(0xE6, 0x00E6, "&aelig;", 0xC3, 0xA6));
		mappedChars.push_back(new ofxSosoMappedChar(0xE7, 0x00E7, "&ccedil;", 0xC3, 0xA7));
    mappedChars.push_back(new ofxSosoMappedChar(0xE8, 0x00E8, "&egrave;", 0xC3, 0xA8));
		mappedChars.push_back(new ofxSosoMappedChar(0xE9, 0x00E9, "&eacute;", 0xC3, 0xA9));
    mappedChars.push_back(new ofxSosoMappedChar(0xEA, 0x00EA, "&ecirc;", 0xC3, 0xAA));
		mappedChars.push_back(new ofxSosoMappedChar(0xEB, 0x00EB, "&euml;", 0xC3, 0xAB));
		mappedChars.push_back(new ofxSosoMappedChar(0xEC, 0x00EC, "&igrave;", 0xC3, 0xAC));
		mappedChars.push_back(new ofxSosoMappedChar(0xED, 0x00ED, "&iacute;", 0xC3, 0xAD));
		mappedChars.push_back(new ofxSosoMappedChar(0xEE, 0x00EE, "&icirc;", 0xC3, 0xAE));
		mappedChars.push_back(new ofxSosoMappedChar(0xEF, 0x00EF, "&iuml;", 0xC3, 0xAF));
		mappedChars.push_back(new ofxSosoMappedChar(0xF0, 0x00F0, "&eth;", 0xC3, 0xB0));
		mappedChars.push_back(new ofxSosoMappedChar(0xF1, 0x00F1, "&ntilde;", 0xC3, 0xB1));
		mappedChars.push_back(new ofxSosoMappedChar(0xF2, 0x00F2, "&ograve;", 0xC3, 0xB2));
		mappedChars.push_back(new ofxSosoMappedChar(0xF3, 0x00F3, "&oacute;", 0xC3, 0xB3));
		mappedChars.push_back(new ofxSosoMappedChar(0xF4, 0x00F4, "&ocirc;", 0xC3, 0xB4));
		mappedChars.push_back(new ofxSosoMappedChar(0xF5, 0x00F5, "&otilde;", 0xC3, 0xB5));
		mappedChars.push_back(new ofxSosoMappedChar(0xF6, 0x00F6, "&ouml;", 0xC3, 0xB6));
		mappedChars.push_back(new ofxSosoMappedChar(0xF7, 0x00F7, "&divide;", 0xC3, 0xB7));
		mappedChars.push_back(new ofxSosoMappedChar(0xF8, 0x00F8, "&oslash;", 0xC3, 0xB8));
		mappedChars.push_back(new ofxSosoMappedChar(0xF9, 0x00F9, "&ugrave;", 0xC3, 0xB9));
		mappedChars.push_back(new ofxSosoMappedChar(0xFA, 0x00FA, "&uacute;", 0xC3, 0xBA));
		mappedChars.push_back(new ofxSosoMappedChar(0xFB, 0x00FB, "&ucirc;", 0xC3, 0xBB));
		mappedChars.push_back(new ofxSosoMappedChar(0xFC, 0x00FC, "&uuml;", 0xC3, 0xBC));
		mappedChars.push_back(new ofxSosoMappedChar(0xFD, 0x00FD, "&yacute;", 0xC3, 0xBD));
		mappedChars.push_back(new ofxSosoMappedChar(0xFE, 0x00FE, "&thorn;", 0xC3, 0xBE));
		mappedChars.push_back(new ofxSosoMappedChar(0xFF, 0x00FF, "&yuml;", 0xC3, 0xBF));
    
		//useful characters mapped from above Unicode 255 down into unused range below 255 from 0x007F-0x009F or 127-159
		//NOTE: 0xC5=197, 0xC6=198, 0x5E=94, 0xE2=226, 0xEF=239
		mappedChars.push_back(new ofxSosoMappedChar(0x7F, 0x0152, "&OElig;", 0xC5, 0x92));        //capital ligature OE
		mappedChars.push_back(new ofxSosoMappedChar(0x80, 0x0153, "&oelig;", 0xC5, 0x93));        //small ligature oe
		mappedChars.push_back(new ofxSosoMappedChar(0x81, 0x0160, "&Scaron;", 0xC5, 0xA0));       //capital letter S with caron
		mappedChars.push_back(new ofxSosoMappedChar(0x82, 0x0161, "&scaron;", 0xC5, 0xA1));       //small letter s with caron
		mappedChars.push_back(new ofxSosoMappedChar(0x83, 0x0178, "&Yuml;", 0xC5, 0xB8));         //capital Y with diaeresis
		mappedChars.push_back(new ofxSosoMappedChar(0x84, 0x017D, "&#x17d;", 0xC5, 0xBD));        //capital letter Z with caron
		mappedChars.push_back(new ofxSosoMappedChar(0x85, 0x017E, "&#x17e;", 0xC5, 0xBE));        //small letter z with caron
		mappedChars.push_back(new ofxSosoMappedChar(0x86, 0x0192, "&fnof;", 0xC6, 0x92));         //small letter f with hook
		mappedChars.push_back(new ofxSosoMappedChar(0x87, 0x02C6, "&#x5e;", 0x5E));               //circumflex
		mappedChars.push_back(new ofxSosoMappedChar(0x88, 0x02DC, "&sim;", 0xE2, 0x88, 0xBC));		//tilde
		mappedChars.push_back(new ofxSosoMappedChar(0x89, 0x2013, "&ndash;", 0xE2, 0x80, 0x93));	//en dash
		mappedChars.push_back(new ofxSosoMappedChar(0x8A, 0x2014, "&mdash;", 0xE2, 0x80, 0x94));	//em dash
		mappedChars.push_back(new ofxSosoMappedChar(0x8B, 0x2018, "&lsquo;", 0xE2, 0x80, 0x98));	//left single quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x8C, 0x2019, "&rsquo;", 0xE2, 0x80, 0x99));	//right single quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x8D, 0x201A, "&sbquo;", 0xE2, 0x80, 0x9A));	//single low quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x8E, 0x201C, "&ldquo;", 0xE2, 0x80, 0x9C));	//left double quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x8F, 0x201D, "&rdquo;", 0xE2, 0x80, 0x9D));	//right double quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x90, 0x201E, "&bdquo;", 0xE2, 0x80, 0x9E));	//double low quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x91, 0x2020, "&dagger;", 0xE2, 0x80, 0xA0));	//dagger
		mappedChars.push_back(new ofxSosoMappedChar(0x92, 0x2021, "&Dagger;", 0xE2, 0x80, 0xA1));	//double dagger
		mappedChars.push_back(new ofxSosoMappedChar(0x93, 0x2022, "&bull;", 0xE2, 0x80, 0xA2));		//bullet
		mappedChars.push_back(new ofxSosoMappedChar(0x94, 0x2026, "&hellip;", 0xE2, 0x80, 0xA6));	//ellipsis
		mappedChars.push_back(new ofxSosoMappedChar(0x95, 0x2030, "&permil;", 0xE2, 0x80, 0xB0));	//per mille
		mappedChars.push_back(new ofxSosoMappedChar(0x96, 0x2039, "&lsaquo;", 0xE2, 0x80, 0xB9));	//single left pointing angle quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x97, 0x203A, "&rsaquo;", 0xE2, 0x80, 0xBA));	//single right pointing angle quotation mark
		mappedChars.push_back(new ofxSosoMappedChar(0x98, 0x20AC, "&euro;", 0xE2, 0x82, 0xAC));		//euro sign
		mappedChars.push_back(new ofxSosoMappedChar(0x99, 0x2122, "&trade;", 0xE2, 0x84, 0xA2));	//trademark sign
		mappedChars.push_back(new ofxSosoMappedChar(0x9A, 0x25A1, "&#x25a1;", 0xE2, 0x96, 0xA1));	//white square
		mappedChars.push_back(new ofxSosoMappedChar(0x9B, 0x25CA, "&loz;", 0xE2, 0x97, 0x8A));		//lozenge
		mappedChars.push_back(new ofxSosoMappedChar(0x9C, 0x25E6, "&#x25e6;", 0xE2, 0x97, 0xA6));	//white bullet
		mappedChars.push_back(new ofxSosoMappedChar(0x9D, 0xFB01, "&#xfb01;", 0xEF, 0xAC, 0x81));	//latin small ligature fi
		mappedChars.push_back(new ofxSosoMappedChar(0x9E, 0xFB02, "&#xfb02;", 0xEF, 0xAC, 0x82));	//latin small ligature fl
		mappedChars.push_back(new ofxSosoMappedChar(0xA6, 0x2212, "&minus;", 0xE2, 0x88, 0x92));  //for SEOptimist font: subtract / (U+2212)
    
		areMappedCharsBuilt = true;
	}
}

//This is a private method used by drawString(). if there is a special byte sequence starting at iIndex in iString, it returns the Unicode value to which the sequence has been mapped.
//and it increments the index (passed in from the while loop in drawString()) accordingly
int ofxSosoTrueTypeFont::getMappedChar(string iString, int &iIndex)
{
	//Named entities
	if(iString[iIndex] == '&'){
		for(int i=0; i < mappedChars.size(); i++){
			bool found = true;
			for(int j=0; j < mappedChars[i]->namedEntity.length(); j++){
				if(iString[iIndex + j] != mappedChars[i]->namedEntity[j])
					found = false;
			}
			if(found){
				//advance index for drawString loop
				iIndex += (mappedChars[i]->namedEntity.length() - 1);
				return mappedChars[i]->mapToIndex - NUM_CHARACTER_TO_START;
			}
		}
	}
	
	//Unicode sequences
	for(int i=0; i < mappedChars.size(); i++){
		if(iString[iIndex] == mappedChars[i]->utf8Sequence[0]){
			bool found = true;
			for(int j=0; j < mappedChars[i]->utf8Sequence.length(); j++){
				if(iString[iIndex + j] != mappedChars[i]->utf8Sequence[j])
					found = false;
			}
			if(found){
				//advance index for drawString loop
				iIndex += (mappedChars[i]->utf8Sequence.length() - 1);
				return mappedChars[i]->mapToIndex - NUM_CHARACTER_TO_START;
			}
		}
	}
	
	return ((unsigned char)iString[iIndex] - NUM_CHARACTER_TO_START);
}

//Variation on getMappedChar() that returns the sequence as is, without mapping it. If there isn't a mapped sequence at that point, NULL is returned.
char* ofxSosoTrueTypeFont::getMappedCharSequence(string iString, int &iIndex)   //eg 0701412
{
  //Named entities
	if(iString[iIndex] == '&'){
		for(int i=0; i < mappedChars.size(); i++){
			bool found = true;
			for(int j=0; j < mappedChars[i]->namedEntity.length(); j++){
				if(iString[iIndex + j] != mappedChars[i]->namedEntity[j])
					found = false;
			}
			if(found){
				//advance index for drawString loop
				iIndex += (mappedChars[i]->namedEntity.length() - 1);
				return (char*)iString.substr(iIndex, mappedChars[i]->namedEntity.length() - 1).c_str();
			}
		}
	}
	
	//Unicode sequences
	for(int i=0; i < mappedChars.size(); i++){
		if(iString[iIndex] == mappedChars[i]->utf8Sequence[0]){
			bool found = true;
			for(int j=0; j < mappedChars[i]->utf8Sequence.length(); j++){
				if(iString[iIndex + j] != mappedChars[i]->utf8Sequence[j])
					found = false;
			}
			if(found){
				//advance index for drawString loop
				iIndex += (mappedChars[i]->utf8Sequence.length() - 1);
				return (char*)iString.substr(iIndex, mappedChars[i]->utf8Sequence.length() - 1).c_str();
			}
		}
	}
	
	return NULL;
}

//using the mapped chars, convert a string from escape or byte sequences to single chars
string ofxSosoTrueTypeFont::convertStringTo255(string iString)
{
  //cout << iString << endl;
  
  string convertedString = "";
  int index = 0;
  while (index < iString.length())
  {
    int c = ofxSosoTrueTypeFont::getMappedChar(iString, index) + NUM_CHARACTER_TO_START;
    convertedString += c;
    index++;
  }
  
  //cout << convertedString << endl;
  
  return convertedString;
}


//these two are overriden from ofTrueTypeFont, because they were private.
void ofxSosoTrueTypeFont::unloadTextures()
{
	if(!bLoadedOk) return;
  
	texAtlas.clear();
	bLoadedOk = false;
}

void ofxSosoTrueTypeFont::reloadTextures()
{
	loadFont(filename, fontSize, bAntiAliased, bFullCharacterSet, false);
}


//sosolimited
float ofxSosoTrueTypeFont::getKerningAdjustment(int c1, int c2)
{
	if ((c1 < FONT_NUM_CHARS) && (c2 < FONT_NUM_CHARS)){
    return (float)kerningPairs[c1][c2];
    
//    return 10.0f; //TODO: test
	}else{
		return 0;
	}
}


//=====================================================================
void ofxSosoTrueTypeFont::drawString(string c, float x, float y) {
  
  ofLogNotice("Calling draw string!");
  /*glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   texAtlas.draw(0,0);*/
  
  if (!bLoadedOk){
    ofLog(OF_LOG_ERROR,"ofTrueTypeFont::drawString - Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    return;
  };
  
	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
  
	bool alreadyBinded = binded;
  
	if(!alreadyBinded) bind();
  
	int len = (int)c.length();
  
	while(index < len){
		
		if(index < (len-1)){
			//printf("charPair = %c%c\n", c[index], c[index+1]);
		}
    
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;

		if (cy < nCharacters){ 			// full char set or not?
      if (c[index] == '\n') {
        
				Y += lineHeight;
				X = x ; //reset X Pos back to zero
        
      }else if (c[index] == ' ') {
        int cy = (int)'p' - NUM_CHARACTER_TO_START;
        X += cps[cy].width * letterSpacing * spaceSize;
        //printf("c[%d] = ' '\n", index);
        
      } else {

				int cM = getMappedChar(c, index); //here is where we check for special unicode sequences, as defined in buildMappedChars()
				drawChar(cM, X, Y);

				if(isKerningEnabled && index < (len-1)){	//soso - kerning adjustment
					X += (cps[cM].advance + getKerningAdjustment(cM, (unsigned char)c[index+1] - NUM_CHARACTER_TO_START)) * letterSpacing;
          
          // TODO: Ask eric about difference between getKerningAdjustment and getKerning
//          X += getKerningAdjustment(cy, prevCy);
          
          
					//if(fabs(getKerningAdjustment(cM, (unsigned char)c[index+1] - NUM_CHARACTER_TO_START)) > 0)
          //printf("kern %c%c = %f\n", cM + NUM_CHARACTER_TO_START,  (unsigned char)c[index+1], getKerningAdjustment(cM, (unsigned char)c[index+1] - NUM_CHARACTER_TO_START));
				}else {

          // AO
          X += cps[cM].advance * letterSpacing;

				}
        
			}
		}
		index++;

	}
  
	if(!alreadyBinded) unbind();
  
}

//Modified to flip the vertically flip the text.
void ofxSosoTrueTypeFont::drawStringAsShapes(string c, float x, float y) {
  
  ofLogNotice("Calling draw string as shapes!");
  if (!bLoadedOk){
    ofLog(OF_LOG_ERROR,"ofTrueTypeFont::drawStringAsShapes - Error : font not allocated -- line %d in %s", __LINE__,__FILE__);
    return;
  };
  
	//----------------------- error checking
	if (!bMakeContours){
		ofLog(OF_LOG_ERROR,"ofTrueTypeFont::drawStringAsShapes - Error : contours not created for this font - call loadFont with makeContours set to true");
		return;
	}
  
	GLint		index	= 0;
	GLfloat		X		= 0;
	GLfloat		Y		= 0;
  
	ofPushMatrix();
	ofTranslate(x, y);
	ofScale(1.0, -1.0, 1.0);	//soso vertical flippage
	int len = (int)c.length();
  
	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
		  if (c[index] == '\n') {
        
				Y += lineHeight;
				X = 0 ; //reset X Pos back to zero
        
		  }else if (c[index] == ' ') {
        int cy = (int)'p' - NUM_CHARACTER_TO_START;
        X += cps[cy].width;
        
		  } else {
        // AO
        drawCharAsShape(cy, X, Y);
        X += (cps[cy].width);

		  }
		}
		index++;

	}
  
	ofPopMatrix();
  
}


//-----------------------------------------------------------
void ofxSosoTrueTypeFont::drawChar(int c, float x, float y) {
  
  ofLogNotice("Calling draw char!");
  //printf("ofxSosoTrueTypeFont::drawChar()\n");
  
	if (c >= nCharacters){
		//ofLog(OF_LOG_ERROR,"Error : char (%i) not allocated -- line %d in %s", (c + NUM_CHARACTER_TO_START), __LINE__,__FILE__);
		return;
	}

  
  int xmin, ymin, xmax, ymax;
  float t1, v1, t2, v2;
  
	t2		= cps[c].t2;
	v2		= cps[c].v2;
  t1		= cps[c].t1;
	v1		= cps[c].v1;

  // ofTrueTypeFont - original for inverted Y
//  xmin  = cps[c].xmin+x;
//  ymin  = cps[c].ymin;
//  xmax  = cps[c].xmax+x;
//  ymax  = cps[c].ymax;
  
  // soso - updated buttery flip flop
  xmin  = cps[c].xmin+x;
  ymin  = -cps[c].ymin;
  xmax  = cps[c].xmax+x;
  ymax  = -cps[c].ymax;

  
  
  //soso - this is the buttery flip-flop
//  x1		= cps[c].x1+x;
//	y1		= -cps[c].y1+y;
//  x2		= cps[c].x2+x;
//	y2		= -cps[c].y2+y;
	
	int firstIndex = stringQuads.getVertices().size();

  
  // AO
  stringQuads.addVertex(ofVec3f(xmin,ymin));
  stringQuads.addVertex(ofVec3f(xmax,ymin));
  stringQuads.addVertex(ofVec3f(xmax,ymax));
  stringQuads.addVertex(ofVec3f(xmin,ymax));
  
	stringQuads.addTexCoord(ofVec2f(t1,v1));
	stringQuads.addTexCoord(ofVec2f(t2,v1));
	stringQuads.addTexCoord(ofVec2f(t2,v2));
	stringQuads.addTexCoord(ofVec2f(t1,v2));
  
	stringQuads.addIndex(firstIndex);
	stringQuads.addIndex(firstIndex+1);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+2);
	stringQuads.addIndex(firstIndex+3);
	stringQuads.addIndex(firstIndex);
}



//==============================================================
//returns draw translations of all chars in vector, but doesn't actually draw
//guts are adapted from drawString()
vector <ofVec2f>  ofxSosoTrueTypeFont::getCharPositions(string c, float x, float y)
{
  
  ofLogNotice("Calling get char positions!");
	vector<ofVec2f> charPositions;
	
	if (!bLoadedOk){
    //ofLog(OF_LOG_ERROR,"Error : font not allocated -- line %d in %s", __LINE__,__FILE__);	//TEST
    return charPositions;
  };
	
	GLint		index	= 0;
	GLfloat		X		= x;
	GLfloat		Y		= y;
	
	int len = (int)c.length();
  
	while(index < len){
		int cy = (unsigned char)c[index] - NUM_CHARACTER_TO_START;
		if (cy < nCharacters){ 			// full char set or not?
			if (c[index] == '\n') {
				Y -= (float)lineHeight;
				X = x ; //reset X Pos back to zero (aka x)
			
      }else if (c[index] == ' ') {
				int cy = (int)'p' - NUM_CHARACTER_TO_START;

				X += (cps[cy].width + letterSpacing);					//soso

        
			} else {
        
				int cM = getMappedChar(c, index); //here is where we check for special unicode sequences, as defined in buildMappedChars()
			
				//instead of drawing, push back char draw position onto vector
				charPositions.push_back(ofVec2f(X, Y));
        
				if(isKerningEnabled && index < (len-1)){	//soso - kerning adjustment

          					X += (cps[cM].width + getKerningAdjustment(cM, (unsigned char)c[index+1] - NUM_CHARACTER_TO_START)) * letterSpacing;

				}else{

          X += cps[cy].width * letterSpacing;
				}
			}
		}
		index++;
	}
	
	return charPositions;
}


float ofxSosoTrueTypeFont::stringWidth(string c)
{
  ofRectangle rect = getStringBoundingBox(c, 0,0);
  return rect.width;
}

//alex
float ofxSosoTrueTypeFont::stringHeight(string c)
{
  ofRectangle rect = getStringBoundingBox(c, 0,0);
  return rect.height;
}


ofRectangle ofxSosoTrueTypeFont::getStringBoundingBox(string c, float x, float y)
{
  
  ofLogNotice("Calling get string bounding box!");
  ofRectangle myRect;
  
  if (!bLoadedOk){
    ofLog(OF_LOG_ERROR,"ofTrueTypeFont::getStringBoundingBox - font not allocated");
    return myRect;
  }
  
  // ao
  int           index = 0;
  int           xoffset = 0;
  int           yoffset = 0;
  int           len = (int) c.length();
  int           xmin = -1;
  int           ymin = -1;
  int           xmax = -1;
  int           ymax = -1;
  
  if ( len < 1 || cps.empty() ){
    
    myRect.x        = x;
    myRect.y        = y;
    myRect.width    = 0;
    myRect.height   = 0;
    return myRect;
  }
  
  bool bFirstCharacter = true;

	while(index < len){
    
		int cy = ((unsigned char)c[index]) - NUM_CHARACTER_TO_START;
    if (cy < nCharacters){ 			// full char set or not?
      if (c[index] == '\n') {
				yoffset += lineHeight;
				xoffset = 0 ; //reset X Pos back to zero
        
				int cM = getMappedChar(c, index); //soso - check for special unicode sequences, as defined in buildMappedChars()

      }else if (c[index] == ' '){
        
        int cy = (int)'p' - NUM_CHARACTER_TO_START;
        xoffset += cps[cy].width * letterSpacing * spaceSize;

        } else {
          
          //TODO TODO TODO
          
          int cM = getMappedChar(c, index);
      
          
          if (bFirstCharacter){
            xmin = cps[cM].xmin+x;
            ymin = cps[cM].ymin+y;
            xmax = cps[cM].xmax+x;
            ymax = cps[cM].ymax+y;
            bFirstCharacter = false;
          } else {

            xoffset += 	(cps[cM].width + getKerningAdjustment(cM, (unsigned char)c[index+1] - NUM_CHARACTER_TO_START)) * letterSpacing;
            
            int charxmin = cps[cM].xmin+xoffset+x;
            int charymin = cps[cM].ymin+yoffset+y;
            int charxmax = cps[cM].xmax+xoffset+x;
            int charymax = cps[cM].ymax+yoffset+y;
            
            if (charxmin < xmin) xmin = charxmin;
            if (charymin < ymin) ymin = charymin;
            if (charxmax > xmax) xmax = charxmax;
            if (charymax > ymax) ymax = charymax;
            
          }
          
          xoffset += cps[cM].advance * letterSpacing;

        }
      
		  }

    index++;


  }

  myRect.x        = min((int)x,xmin);
  myRect.y        = min((int)y,ymin);
  myRect.width    = xmax - x;
  myRect.height   = ymax - ymin;
  
  return myRect;

}


//Helper method to strip away selected characters. //eg 070412
void ofxSosoTrueTypeFont::removeCharacters(string &iString, string iCharsToRemove)
{
  for (unsigned int i = 0; i < iCharsToRemove.length(); ++i)
    iString.erase(std::remove(iString.begin(), iString.end(), iCharsToRemove[i]), iString.end());
}


void ofxSosoTrueTypeFont::enableKerning(bool iEnable)
{
	isKerningEnabled = iEnable;
}

//this method lets you do manual kerning
void ofxSosoTrueTypeFont::setKerningPair(unsigned char iChar0, unsigned char iChar1, int iUnits)
{
	if(((int)(iChar0 - NUM_CHARACTER_TO_START) < FONT_NUM_CHARS) && ((int)(iChar1 - NUM_CHARACTER_TO_START) < FONT_NUM_CHARS)){
		kerningPairs[(int)iChar0 - NUM_CHARACTER_TO_START][(int)iChar1 - NUM_CHARACTER_TO_START] = iUnits;
	}
}

void ofxSosoTrueTypeFont::printNonzeroKerningPairs()
{
	for (int i = 0; i < FONT_NUM_CHARS; i++) {
		for (int j = 0; j < FONT_NUM_CHARS; j++) {
			if(fabs((float)kerningPairs[i][j]) > 0) printf("kerningPairs: %c%c = %d\n", (unsigned char)(i + NUM_CHARACTER_TO_START), (unsigned char)(j + NUM_CHARACTER_TO_START), kerningPairs[i][j]);
		}
	}
}