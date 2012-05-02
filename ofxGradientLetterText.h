#pragma once

#include "ofxObject.h"
#include "ofxLetterText.h"
#include "Refs.h"

class ofxGradientLetterText : public ofxLetterText
{
public:
    
	ofxGradientLetterText();
	~ofxGradientLetterText();
    
    void                            setGradientColors(ofVec3f iColor1, ofVec3f iColor2);
    void                            setGradientPosition(float iPos); //jro4
    void                            updateLetterColors();
    
    ofVec3f                         getColorFromPosition(ofVec3f letterTrans);
    
    vector<ofVec3f>                 getGradientColors(); //jro3
    
protected:
    
    Refs                            *refs;
    
    ofVec3f                         gradientColor1;
    ofVec3f                         gradientColor2;
    ofVec3f                         baselineColor;
    float                           gradientPosition;
    
    vector<ofVec3f>                 gradientColors; //jro
    
public:
    
};
