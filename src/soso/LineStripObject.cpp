#include "soso/LineStripObject.h"


LineStripObject::LineStripObject(int iNumVerts)
{
	lineWidth = 1.0;
	for(int i=0; i < iNumVerts; i++){
		addVertex(0,0,0);
    }

    isVertexColoringEnabled = false;
}

LineStripObject::LineStripObject(){}

void LineStripObject::render()
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i < vertices.size(); i++){
		if(isVertexColoringEnabled)
            glColor4f(vertices[i]->color.x/255.0f, vertices[i]->color.y/255.0f, vertices[i]->color.z/255.0f, drawMaterial->color.a/255.0f * vertices[i]->color.w/255.0f);

        glVertex3f(vertices[i]->position.x, vertices[i]->position.y, vertices[i]->position.z);
	}
	glEnd();
}


void LineStripObject::setLineWidth(float iWidth)
{
	lineWidth = iWidth;
}

void LineStripObject::setVertexPos(int iVertNum, float iX, float iY, float iZ)
{
	if( iVertNum >= 0 && iVertNum < vertices.size() ){
		vertices[iVertNum]->position.set(iX, iY, iZ);
	}
}

void LineStripObject::setVertexColor(int iVertNum, float iR, float iG, float iB, float iA)
{
	if( iVertNum >= 0 && iVertNum < vertices.size() ){
		vertices[iVertNum]->color.set(iR, iG, iB, iA);
	}
}

void LineStripObject::addVertex(float iX, float iY, float iZ)
{
    LineStripVertex *vert = new LineStripVertex();
    vert->position.set(iX, iY, iZ);
    vert->color.set(255, 255, 255, 255);
	vertices.push_back(vert);
}

void LineStripObject::addVertex(float iX, float iY, float iZ, ci::Vec4f iColor)
{
    LineStripVertex *vert = new LineStripVertex();
    vert->position.set(iX, iY, iZ);
    vert->color = iColor;
	vertices.push_back(vert);
}

void LineStripObject::enableVertexColoring(bool iEnable)
{
    isVertexColoringEnabled = iEnable;
}