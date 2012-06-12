#include <iostream>
#include "ofxGridSystem.h"


ofxGridSystem::ofxGridSystem(float iPageWidth, float iPageHeight, int iNumColumns, float iBaselineLeading, float iBack, float iGutter, float iHead, float iTail)
{
    buildGrid(iPageWidth, iPageHeight, iNumColumns, iBaselineLeading, iBack, iGutter, iHead, iTail);    
}

//NOTE If you use this constructor, you need to call buildGrid()
ofxGridSystem::ofxGridSystem()
{
    width = 0;
    height = 0;
    numCol = 0;
    left = 0;
    right = 0;
    tail = 0;
    head = 0;
    leading = 0;
    leftX = 0;
    rightX = 0;
    topY = 0;
    bottomY = 0;
    colW = 0;
    baselines = NULL;
    numBaselines = 0;
    isGridShown = false;
    isBaselineGridShown = false;
}

ofxGridSystem::~ofxGridSystem(){}

//Returns the X position of a column.
float ofxGridSystem::colX(int iNumber)
{
    if(iNumber < 0) 
        return 0;
    else
        return (leftX + iNumber*(colW + leading)); 
}

//Returns the width of a number of columns.
float ofxGridSystem::col(int iNumber)
{
    return (colW * iNumber + leading * (iNumber - 1));
}

void ofxGridSystem::buildGrid(float iPageWidth, float iPageHeight, int iNumColumns, float iBaselineLeading, float iLeft, float iRight, float iHead, float iTail)
{
    width = iPageWidth;
    height = iPageHeight;
    
    numCol = iNumColumns;
    
    //Left, right, bottom, top in grid terms.
    left = iLeft;
    right = iRight;
    tail = iTail;
    head = iHead;
    
    leading = iBaselineLeading;
    
    
    //Back size determines position of left margin line.
    leftX = iLeft;
    //Gutter size determines position of left margin line.
    rightX = width - iRight;
    //Head determines top margin location.
    topY = height - head;
    //Tail determines bottom margin location.
    bottomY = tail;
    
    //Determine column width, Swiss grid style.
    //There is a baselineLeading space between each column. 
    colW = (width - left - right - (numCol-1)*leading)/(float)numCol;
    
    //Calculate baseline grid positions. 
    //NOTE Baseline grid starts at top of page. So you are responsible for aligning grid with head and tail.
    numBaselines = (int)(height/(float)leading);
    baselines = new float[numBaselines];
    for(int i=0; i < numBaselines; i++)
        baselines[i] = topY - (float)i*leading;
    
    isGridShown = false;
    isBaselineGridShown = false;
    
    buildLines();
    //By default, start with lines hidden.
    showLines(false);
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
    headLine->setVertexPos(1, width-left-right, 0, 0);
    headLine->setTrans(leftX, topY, 0);
    headLine->hide();
    lines.push_back(headLine);
    addChild(headLine);
    
    //Tail line
    ofxLineStripObject *tailLine = new ofxLineStripObject(2);
    tailLine->setVertexPos(0, 0, 0, 0);
    tailLine->setVertexPos(1, width-left-right, 0, 0);
    tailLine->setTrans(leftX, bottomY, 0);
    tailLine->hide();
    lines.push_back(tailLine);
    addChild(tailLine);
 
    //Set line color and width.
    for(int i=0; i < lines.size(); i++){
        lines[i]->setColor(100, 100, 255, 255);
        lines[i]->setLineWidth(1.0);
        lines[i]->hide();
    }
    
    //Create baseline lines.
    for(int i=0; i < numBaselines; i++){
        ofxLineStripObject *line = new ofxLineStripObject(2);
        line->setVertexPos(0, 0, 0, 0);
        line->setVertexPos(1, width, 0, 0);
        line->setTrans(0, height - (float)i*leading, 0);
        line->setColor(100, 100, 255, 100); 
        line->setLineWidth(1.0);
        line->hide();
        baselineLines.push_back(line);
        addChild(line);
    }
}


//Method for accessing grid positions.
//Basically, this returns an ofxGridSystemArray, which also has an overloaded [] operator.
//Usage: grid[2][2] where this will return an ofVec3f for the column 2 and 2 baselines down from top of page.
ofxGridSystemArray ofxGridSystem::operator[](int iColumn)
{
    ofxGridSystemArray array;
    array.x = colX(iColumn);
    array.leading = leading;
    array.height = height;
    
    return array;
}

void ofxGridSystem::setLineColor(ofVec4f iColor)
{
    for(int i=0; i < lines.size(); i++)
        lines[i]->setColor(iColor);
    
    for(int i=0; i < baselineLines.size(); i++)
        baselineLines[i]->setColor(iColor.x, iColor.y, iColor.z, 100);
}


void ofxGridSystem::setLineWidth(float iWidth)
{
    for(int i=0; i < lines.size(); i++)
        lines[i]->setLineWidth(iWidth);
    
    for(int i=0; i < baselineLines.size(); i++)
        baselineLines[i]->setLineWidth(iWidth);
}


bool ofxGridSystem::isGridVisible()
{
    return isGridShown;
}

bool ofxGridSystem::isBaselineGridVisible()
{
    return isBaselineGridShown;
}

//By default this shows all lines in grid. To hide baseline grid, use showBaselines()
void ofxGridSystem::showLines(bool iShowHide)
{
    for(int i=0; i < lines.size(); i++){
        if(iShowHide) lines[i]->show();
        else lines[i]->hide();
    }
    
    for(int i=0; i < baselineLines.size(); i++){
        if(iShowHide){ 
            if(isBaselineGridShown) baselineLines[i]->show();
        }else{ 
            baselineLines[i]->hide();
        }
    }
    
    isGridShown = iShowHide;
    
}

void ofxGridSystem::showBaselines(bool iShowHide)
{
    for(int i=0; i < baselineLines.size(); i++){
        if(iShowHide) baselineLines[i]->show();
        else baselineLines[i]->hide();
    }
    
    isBaselineGridShown = iShowHide;
}
