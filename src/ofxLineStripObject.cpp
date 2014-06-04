#include "ofxLineStripObject.h"

ofxLineStripObject::ofxLineStripObject(int iNumVerts)
{
	lineWidth = 1.0;
	for(int i=0; i < iNumVerts; i++){
		addVertex(0,0,0);
  }
  
  isVertexColoringEnabled = false;
}

ofxLineStripObject::~ofxLineStripObject(){
  for (auto vertex : vertices){
    delete vertex;
  }
  vertices.clear();
}

void ofxLineStripObject::render()
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i < vertices.size(); i++){
		if(isVertexColoringEnabled)
      glColor4f(vertices[i]->color.r/255.0f, vertices[i]->color.g/255.0f, vertices[i]->color.b/255.0f, drawMaterial->color.a/255.0f * vertices[i]->color.a/255.0f);
    
    glVertex3f(vertices[i]->position.x, vertices[i]->position.y, vertices[i]->position.z);
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
		vertices[iVertNum]->position.set(iX, iY, iZ);
	}
}

void ofxLineStripObject::setVertexColor(int iVertNum, float iR, float iG, float iB, float iA)
{
	if(ofInRange(iVertNum, 0, vertices.size()-1)){
		vertices[iVertNum]->color.set(iR, iG, iB, iA);
	}
}

void ofxLineStripObject::addVertex(float iX, float iY, float iZ)
{
  ofxLineStripVertex *vert = new ofxLineStripVertex();
  vert->position.set(iX, iY, iZ);
  vert->color.set(255, 255, 255, 255);
	vertices.push_back(vert);
}

void ofxLineStripObject::addVertex(float iX, float iY, float iZ, ofColor iColor)
{
  ofxLineStripVertex *vert = new ofxLineStripVertex();
  vert->position.set(iX, iY, iZ);
  vert->color = iColor;
	vertices.push_back(vert);
}

void ofxLineStripObject::enableVertexColoring(bool iEnable)
{
  isVertexColoringEnabled = iEnable;
}