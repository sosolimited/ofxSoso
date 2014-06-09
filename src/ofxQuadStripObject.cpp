#include "ofxQuadStripObject.h"

ofxQuadStripObject::ofxQuadStripObject(int iNumVertices)
{
	numVertices = iNumVertices;
	drawMode = OF_FILLED;
	vertices = new float[3*iNumVertices];
	texCoords = new float[2*iNumVertices];
	vertexColors = new float[4*iNumVertices];
	//init all vertex colors to white
	for(int i=0; i < (4*numVertices); i++){
		vertexColors[i] = 1.0;
	}
	vertexColoringEnabled = false;
	texture = NULL;
  
}

ofxQuadStripObject::~ofxQuadStripObject()
{
	delete[] vertices;
  delete[] texCoords;
  delete[] vertexColors;
}

void ofxQuadStripObject::render()
{
	if(texture){
		glEnable(texture->getTextureData().textureTarget);
		glBindTexture(texture->getTextureData().textureTarget, (GLuint)texture->getTextureData().textureID);
    
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	}
  
	if(vertexColoringEnabled){
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, 0, vertexColors);
	}
  
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays((drawMode == OF_FILLED) ? GL_QUAD_STRIP : GL_LINE_LOOP, 0, numVertices);
  
	if(texture){
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisable(texture->getTextureData().textureTarget);
	}
  
	if(vertexColoringEnabled)
		glDisableClientState(GL_COLOR_ARRAY);
}

void ofxQuadStripObject::setVertexPos(int iVertexNum, ofVec3f iPos)
{
	if(iVertexNum < numVertices){
		vertices[3*iVertexNum] = iPos.x;
		vertices[3*iVertexNum + 1] = iPos.y;
		vertices[3*iVertexNum + 2] = iPos.z;
	}
}

void ofxQuadStripObject::setVertexTexCoords(int iVertexNum, float iU, float iV)
{
	if(iVertexNum < numVertices){
		//texCoords[2*iVertexNum] = iU;
		//texCoords[2*iVertexNum + 1] = iV;
    
		//adding auto clamping
		if(texture){
			texCoords[2*iVertexNum] = min(iU, texture->getWidth());
			texCoords[2*iVertexNum] = max(texCoords[2*iVertexNum], 0.0f);
      
			texCoords[2*iVertexNum + 1] = min(iV, texture->getHeight());
			texCoords[2*iVertexNum + 1] = max(texCoords[2*iVertexNum + 1], 0.0f);
		}
		
	}
}

ofVec3f ofxQuadStripObject::getVertexPos(int iVertexNum)
{
	ofVec3f result;
  
	if(iVertexNum < numVertices){
		result.x = vertices[3*iVertexNum];
		result.y = vertices[3*iVertexNum + 1];
		result.z = vertices[3*iVertexNum + 2];
	}
  
	return result;
}

ofVec2f ofxQuadStripObject::getVertexTexCoords(int iVertexNum)
{
	if(iVertexNum < numVertices){
		return ofVec2f(texCoords[2*iVertexNum], texCoords[2*iVertexNum + 1]);
	}
}

void ofxQuadStripObject::setVertexColor(int iVertexNum, float iR, float iG, float iB, float iA)
{
	if(iVertexNum < numVertices){
		vertexColors[4*iVertexNum] = iR/255.0f;
		vertexColors[4*iVertexNum + 1] = iG/255.0f;
		vertexColors[4*iVertexNum + 2] = iB/255.0f;
		vertexColors[4*iVertexNum + 3] = drawMaterial->color.a/255.0f * iA/255.0f;
		//automatically enable vertex coloring if this is called
		vertexColoringEnabled = true;
	}
}

void ofxQuadStripObject::setVertexColorOnly(int iVertexNum, float iR, float iG, float iB)
{
	if(iVertexNum < numVertices){
		vertexColors[4*iVertexNum] = iR/255.0f;
		vertexColors[4*iVertexNum + 1] = iG/255.0f;
		vertexColors[4*iVertexNum + 2] = iB/255.0f;
    
		//automatically enable vertex coloring if this is called
		vertexColoringEnabled = true;
	}
}
void ofxQuadStripObject::setVertexAlpha(int iVertexNum, float iA)
{
	if(iVertexNum < numVertices){
		vertexColors[4*iVertexNum + 3] = drawMaterial->color.a/255.0f * iA/255.0f;
		//automatically enable vertex coloring if this is called
		vertexColoringEnabled = true;
	}
}

//OF_FILLED, OF_OUTLINE
void ofxQuadStripObject::setDrawMode(int iDrawMode)
{
	drawMode = iDrawMode;
}

void ofxQuadStripObject::setTexture(ofImage *iTex)
{
	texture = &iTex->getTextureReference();
}

void ofxQuadStripObject::enableVertexColoring(bool iEnable)
{
	vertexColoringEnabled = iEnable;
}

ofTexture* ofxQuadStripObject::getTexture()
{
	return texture;
}

ofVec4f ofxQuadStripObject::getVertexColor(int iVertexNum) {
  return ofVec4f(vertexColors[iVertexNum*4]*255, vertexColors[iVertexNum*4+1]*255, vertexColors[iVertexNum*4+2]*255, vertexColors[iVertexNum*4+3]*255);
}

float ofxQuadStripObject::getVertexAlpha(int iVertexNum) {
  return vertexColors[iVertexNum*4+3]*255;
}
