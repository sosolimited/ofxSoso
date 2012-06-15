#include "ofxLineSegmentObject.h"


ofxLineSegmentObject::ofxLineSegmentObject(int iNumVerts)
{
	lineWidth = 1.0;
	for(int i=0; i < iNumVerts; i++){
		addVertex(0,0,0);
	}
    
    isStipplingEnabled = false;
	stipplingPattern = 0xAA;
	stipplingFactor = 1.0;
    smoothingHint = GL_NICEST;
	
    
}

ofxLineSegmentObject::~ofxLineSegmentObject(){}

void ofxLineSegmentObject::render()
{
    glEnable(GL_LINE_SMOOTH); 		
    glHint(GL_LINE_SMOOTH_HINT, smoothingHint);		
    
    glLineWidth(lineWidth);
    
    if(isStipplingEnabled){
        glLineStipple(stipplingFactor, stipplingPattern);
        glEnable(GL_LINE_STIPPLE);
    }
    else{
        glDisable(GL_LINE_STIPPLE);
    }

	glBegin(GL_LINES);
	for(int i=0; i < vertices.size(); i++){
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

    if(isStipplingEnabled) glDisable(GL_LINE_STIPPLE);
}


void ofxLineSegmentObject::setLineWidth(float iWidth)
{
	lineWidth = iWidth;
}

void ofxLineSegmentObject::setVertexPos(int iVertNum, float iX, float iY, float iZ)
{
	if(ofInRange(iVertNum, 0, vertices.size()-1)){
		vertices[iVertNum].set(iX, iY, iZ);
	}
}

void ofxLineSegmentObject::addVertex(float iX, float iY, float iZ)
{
	vertices.push_back(ofVec3f(iX, iY, iZ));
}


void ofxLineSegmentObject::enableStippling(bool iEnable)
{
	isStipplingEnabled = iEnable;
}



void ofxLineSegmentObject::setStipplingPattern(GLushort iPattern, GLint iFactor)
{
	stipplingPattern = iPattern;
	stipplingFactor = iFactor;    
}
