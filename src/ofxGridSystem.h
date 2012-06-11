// Copyright (c) 1998-2009 Small Design Firm, Inc. All rights reserved.


#pragma once 
#include "ofxObject.h"
#include "ofxLineStripObject.h"

class ofxGridSystem : public ofxObject{

public:
    ofxGridSystem(float iPageWidth, float iPageHeight, int iNumColumns, float iBaselineLeading, float iBack, float iGutter, float iHead, float iTail);
    ~ofxGridSystem();

    float           colX(int iNumber);
    void            setLineColor(ofVec4f iColor);
    void            setLineWidth(float iWidth);

    void            showLines(bool iShowHide);

    float           operator[](int iIndex){return colX(iIndex);}
    
    
private:
    void            buildLines();
    
public:
    float           width,
                    height;
    float           colW;
    int             numCol;
    
    float           back,
                    gutter,
                    head,
                    tail;
    
    float           topY,
                	bottomY;
    float           leftX,
                    rightX;
    
    float           leading;
   
    vector<ofxLineStripObject *>    lines;
    
};
