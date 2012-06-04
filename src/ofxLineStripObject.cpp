#include "ofxLineStripObject.h"


ofxLineStripObject::ofxLineStripObject(int iNumVerts)
{
	lineWidth = 1.0;
	for(int i=0; i < iNumVerts; i++){
		addVertex(0,0,0);
		addVertexColor(255,255,255,255);
	}

}

ofxLineStripObject::ofxLineStripObject(){}

void ofxLineStripObject::render()
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i < vertices.size(); i++){

		glColor4f(vertColors[i].x, vertColors[i].y, vertColors[i].z, vertColors[i].w);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

}


void ofxLineStripObject::setLineWidth(float iWidth)
{
	lineWidth = iWidth;
}

void ofxLineStripObject::setVertexPos(int iVertNum, float iX, float iY, float iZ)
{
	if(ofInRange(iVertNum, 0, vertices.size()-1)){
		vertices[iVertNum].set(iX, iY, iZ);
	}
}

void ofxLineStripObject::setVertexColor(int iVertNum, float iR, float iG, float iB, float iA)
{
	if(ofInRange(iVertNum, 0, vertColors.size()-1)){
		vertColors[iVertNum].set(iR, iG, iB, iA);
	}
}

void ofxLineStripObject::addVertex(float iX, float iY, float iZ)
{
	vertices.push_back(ofVec3f(iX, iY, iZ));
}

void ofxLineStripObject::addVertexColor(float iR, float iG, float iB, float iA)
{
	vertColors.push_back(ofVec4f(iR, iG, iB, iA));
}