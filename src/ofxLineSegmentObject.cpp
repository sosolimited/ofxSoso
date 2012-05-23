#include "ofxLineSegmentObject.h"


ofxLineSegmentObject::ofxLineSegmentObject(int iNumVerts)
{
	lineWidth = 1.0;
	for(int i=0; i < iNumVerts; i++){
		addVertex(0,0,0);
	}

}

ofxLineSegmentObject::~ofxLineSegmentObject(){}

void ofxLineSegmentObject::render()
{
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	for(int i=0; i < vertices.size(); i++){
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

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