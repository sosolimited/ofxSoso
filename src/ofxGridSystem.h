/*
 ofxGridSystem
 
 ofxGridSystem lets you define a classic Swiss grid. If you position your objects based on the grid,
 using the methods shown below, you can adjust the grid and your objects will always conform. 
 It also draws itself on screen. The grid has a lower left corner origin.
 
 Usage: Make your grid as a reference. i.e. ofxGridSystem grid; Add it with addChild(&grid); 
 Then call grid.buildGrid(...) with parameters.
 
 To get 2nd column, 6 lines down from top, call grid[1][5], which returns an ofVec3 at that position.
 To get the X position of the 3rd column, call grid[3].x or grid.colX(3);
 To get the width of 3 columns (including gutters between them) call col(3);
 
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
