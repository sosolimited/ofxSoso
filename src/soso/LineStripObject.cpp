#include "soso/LineStripObject.h"

using namespace soso;
using namespace cinder;

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
	gl::begin(GL_LINE_STRIP);
	for(int i=0; i < vertices.size(); i++){
		if(isVertexColoringEnabled)
            gl::color(vertices[i]->color.x/255.0f, vertices[i]->color.y/255.0f, vertices[i]->color.z/255.0f, drawMaterial->color.a/255.0f * vertices[i]->color.w/255.0f);

        gl::vertex(vertices[i]->position.x, vertices[i]->position.y, vertices[i]->position.z);
	}
	gl::end();
}


void LineStripObject::setLineWidth(float iWidth)
{
	lineWidth = iWidth;
}

void LineStripObject::setVertexPos(int iVertNum, float iX, float iY, float iZ)
{
	if( iVertNum >= 0 && iVertNum < vertices.size() ){
		vertices[iVertNum]->position = vec3(iX, iY, iZ);
	}
}

void LineStripObject::setVertexColor(int iVertNum, float iR, float iG, float iB, float iA)
{
	if( iVertNum >= 0 && iVertNum < vertices.size() ){
		vertices[iVertNum]->color = vec4(iR, iG, iB, iA);
	}
}

void LineStripObject::addVertex(float iX, float iY, float iZ)
{
    LineStripVertex *vert = new LineStripVertex();
    vert->position = vec3(iX, iY, iZ);
    vert->color = vec4(255, 255, 255, 255);
	vertices.push_back(vert);
}

void LineStripObject::addVertex(float iX, float iY, float iZ, ci::vec4 iColor)
{
    LineStripVertex *vert = new LineStripVertex();
    vert->position = vec3(iX, iY, iZ);
    vert->color = iColor;
	vertices.push_back(vert);
}

void LineStripObject::enableVertexColoring(bool iEnable)
{
    isVertexColoringEnabled = iEnable;
}