#include "soso/LineSegmentObject.h"

using namespace soso;

LineSegmentObject::LineSegmentObject(int iNumVerts)
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

LineSegmentObject::~LineSegmentObject(){
}

void LineSegmentObject::render()
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


void LineSegmentObject::setLineWidth(float iWidth)
{
	lineWidth = iWidth;
}

void LineSegmentObject::setVertexPos(int iVertNum, float iX, float iY, float iZ)
{
	if( iVertNum >= 0 && iVertNum < vertices.size() ){
		vertices[iVertNum].set(iX, iY, iZ);
	}
}

void LineSegmentObject::addVertex(float iX, float iY, float iZ)
{
	vertices.push_back(ci::Vec3f(iX, iY, iZ));
}


void LineSegmentObject::enableStippling(bool iEnable)
{
	isStipplingEnabled = iEnable;
}



void LineSegmentObject::setStipplingPattern(GLushort iPattern, GLint iFactor)
{
	stipplingPattern = iPattern;
	stipplingFactor = iFactor;
}
