/*
 ofxGridSystem
 
 ofxGridSystem lets you define a classic Swiss grid. If you position your objects based on the grid, 
 you can adjust the grid and your objects will always conform to the grid. It also draws itself on screen.
 The grid has a lower left right origin.
 
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

#include <iostream>
#include "ofxGridSystem.h"


ofxGridSystem::ofxGridSystem(float iPageWidth, float iPageHeight, int iNumColumns, float iBaselineLeading, float iBack, float iGutter, float iHead, float iTail)
{
    width = iPageWidth;
    height = iPageHeight;
    
    numCol = iNumColumns;
    
    //Left, right, bottom, top in grid terms.
    back = iBack;
    gutter = iGutter;
    tail = iTail;
    head = iHead;
        
    leading = iBaselineLeading;
    
    //Back size determines position of left margin line.
    leftX = iBack;
    //Gutter size determines position of left margin line.
    rightX = width - iGutter;
    //Head determines top margin location.
    topY = height - head;
    //Tail determines bottom margin location.
    bottomY = tail;
    
    //Determine column width, Swiss grid style.
    //There is a baselineLeading space between each column. 
    colW = (width - back - gutter - (numCol-1)*leading)/(float)numCol;
           
    buildLines();
    
}

ofxGridSystem::~ofxGridSystem(){}

float ofxGridSystem::colX(int iNumber)
{
    if(iNumber < 0) 
        return 0;
    else
        return (leftX + iNumber*(colW + leading)); 
}


void ofxGridSystem::buildLines()
{
    float curX = leftX; 
    
    for(int i=0; i < (numCol+1); i++){ 
            
        if(i>0){
            ofxLineStripObject *line = new ofxLineStripObject(2);
            line->setVertexPos(0, 0, 0, 0);
            line->setVertexPos(1, 0, height - tail - head, 0);
            line->setTrans(curX, tail, 0);
            lines.push_back(line);
            addChild(line);
            
            curX += leading;
        }
        
        if(i < numCol){  //For last line on right.
            ofxLineStripObject *line = new ofxLineStripObject(2);
            line->setVertexPos(0, 0, 0, 0);
            line->setVertexPos(1, 0, height - tail - head, 0);
            line->setTrans(curX, tail, 0);
            lines.push_back(line);
            addChild(line);
        }
        
        curX += colW;
    }
    
    //Head line
    ofxLineStripObject *headLine = new ofxLineStripObject(2);
    headLine->setVertexPos(0, 0, 0, 0);
    headLine->setVertexPos(1, width-back-gutter, 0, 0);
    headLine->setTrans(leftX, topY, 0);
    lines.push_back(headLine);
    addChild(headLine);
    
    //Tail line
    ofxLineStripObject *tailLine = new ofxLineStripObject(2);
    tailLine->setVertexPos(0, 0, 0, 0);
    tailLine->setVertexPos(1, width-back-gutter, 0, 0);
    tailLine->setTrans(leftX, bottomY, 0);
    lines.push_back(tailLine);
    addChild(tailLine);
 
    //Set line color and width.
    for(int i=0; i < lines.size(); i++){
        lines[i]->setColor(100, 100, 255, 255);
        lines[i]->setLineWidth(1.0);
    }
}


void ofxGridSystem::setLineColor(ofVec4f iColor)
{
    for(int i=0; i < lines.size(); i++)
        lines[i]->setColor(iColor);
}


void ofxGridSystem::setLineWidth(float iWidth)
{
    for(int i=0; i < lines.size(); i++)
        lines[i]->setLineWidth(iWidth);
}

void ofxGridSystem::showLines(bool iShowHide)
{
    for(int i=0; i < lines.size(); i++){
        if(iShowHide) lines[i]->show();
        else lines[i]->hide();
    }
}

