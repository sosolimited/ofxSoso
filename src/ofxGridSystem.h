// Copyright (c) 1998-2009 Small Design Firm, Inc. All rights reserved.


#pragma once 
#include "ofxObject.h"
#include "ofxLineStripObject.h"

//Funky little helper class for overloading [][]
class ofxGridSystemArray{
public:
    ofxGridSystemArray(){};
    ~ofxGridSystemArray(){};

public:
    float height;
    float leading;
    float x;
    ofVec3f operator[](int iIndex)
    {
        return ofVec3f(x, height-(float)iIndex*leading, 0); 
    };  
};

class ofxGridSystem : public ofxObject{

public:
    ofxGridSystem(float iPageWidth, float iPageHeight, int iNumColumns, float iBaselineLeading, float iLeft, float iRight, float iHead, float iTail);
    ofxGridSystem();
    ~ofxGridSystem();

    void            buildGrid(float iPageWidth, float iPageHeight, int iNumColumns, float iBaselineLeading, float iLeft, float iRight, float iHead, float iTail);
    
    float           colX(int iNumber);
    float           col(int iNumber=1);
    void            setLineColor(ofVec4f iColor);
    void            setLineWidth(float iWidth);

    void            showLines(bool iShowHide);
    void            showBaselines(bool iShowHide);

    ofxGridSystemArray  operator[](int iIndex);
    
    bool            isGridVisible();
    bool            isBaselineGridVisible();
    
private:
    void            buildLines();
    
    int             numBaselines;
    float           *baselines;
    bool            isGridShown,
                    isBaselineGridShown;
    
public:
    float           width,
                    height;
    float           colW;
    int             numCol;
    
    float           left,
                    right,
                    head,
                    tail;
    
    
    float           topY,
                	bottomY;
    float           leftX,
                    rightX;
    
    float           leading;
   
    vector<ofxLineStripObject *>    lines;
    vector<ofxLineStripObject *>    baselineLines;
    
};
