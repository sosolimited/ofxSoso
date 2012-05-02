#include "ofxGradientLetterText.h"

ofxGradientLetterText::ofxGradientLetterText() {
    
    refs = Refs::instance();
    
    //jro3 - defaults to white
    gradientColor1.set(255,255,255);
    gradientColor2.set(255,255,255);
    
    gradientColors.push_back(gradientColor1);
    gradientColors.push_back(gradientColor2);
    
    baselineColor = gradientColor1;
    
    gradientPosition = 0.0;
}

ofxGradientLetterText::~ofxGradientLetterText(){}

//jro3
void ofxGradientLetterText::setGradientColors(ofVec3f iColor1, ofVec3f iColor2)
{
    gradientColor1 = iColor1; 
    gradientColor2 = iColor2; 
    
    gradientColors.clear();
    gradientColors.push_back(gradientColor1);
    gradientColors.push_back(gradientColor2);
    
    //jro4
    //setGradientPosition(0.5);
    updateLetterColors();
}

//jro3
vector<ofVec3f> ofxGradientLetterText::getGradientColors()
{
    return gradientColors;
}

//jro4
void ofxGradientLetterText::setGradientPosition(float iPos)
{
    gradientPosition = iPos;
    updateLetterColors();
}

void ofxGradientLetterText::updateLetterColors() {
    
    for (int i = 0 ; i < letters.size() ; i++) {
        
        ofVec3f color = getColorFromPosition(letters[i]->getTrans());
        
        letters[i]->setColor( color[0], color[1], color[2] ) ;
        
    }
    
}

ofVec3f   ofxGradientLetterText::getColorFromPosition(ofVec3f letterTrans) {
    
    float       letterGradientPos;
    ofVec3f     outputColor;
    
    letterTrans[0] /= ofGetScreenWidth();
    //printf("%f | ", letterTrans[0]);    
    letterTrans[1] /= ofGetScreenHeight();
    
    
    //you do the math. this works, so STFU
    letterGradientPos = 1.0/3.0 - letterTrans[0]/3.0 + 2.0*gradientPosition/3.0;
    //printf("%f | ", letterGradientPos); 
    
    if (letterGradientPos < 0.4) {
        
        outputColor = gradientColor1;
        
    } else if (letterGradientPos < 0.6) {
        
        outputColor = 5.0*(letterGradientPos - 0.4)*gradientColor2 + (1.0 - 5.0*(letterGradientPos - 0.4))*gradientColor1;
        
    } else {
        
        outputColor = gradientColor2;
        
    }
    
    //    printf("gp:%f : %f, %f, %f \n", gradientPosition, outputColor[0], outputColor[1], outputColor[2]);
    
    return outputColor;
    
}