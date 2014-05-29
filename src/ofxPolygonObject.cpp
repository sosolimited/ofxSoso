#include "ofxPolygonObject.h"

ofxPolygonObject::ofxPolygonObject(int iNumVertices)
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

ofxPolygonObject::~ofxPolygonObject()
{
	delete[] vertices;
  delete[] texCoords;
  delete[] vertexColors;
}

void ofxPolygonObject::render()
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
	if(numVertices < 4) glDrawArrays((drawMode == OF_FILLED) ? GL_TRIANGLES : GL_LINE_LOOP, 0, numVertices);	//GL_TRIANGLE_FAN don't work for < 4 verts
	else glDrawArrays((drawMode == OF_FILLED) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, numVertices);
  
	if(texture){
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisable(texture->getTextureData().textureTarget);
	}
  
	if(vertexColoringEnabled)
		glDisableClientState(GL_COLOR_ARRAY);
}

void ofxPolygonObject::setVertexPos(int iVertexNum, ofVec3f iPos)
{
	if(iVertexNum < numVertices){
		vertices[3*iVertexNum] = iPos.x;
		vertices[3*iVertexNum + 1] = iPos.y;
		vertices[3*iVertexNum + 2] = iPos.z;
	}
}

void ofxPolygonObject::setVertexTexCoords(int iVertexNum, float iU, float iV)
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

ofVec3f ofxPolygonObject::getVertexPos(int iVertexNum)
{
	ofVec3f result;
  
	if(iVertexNum < numVertices){
		result.x = vertices[3*iVertexNum];
		result.y = vertices[3*iVertexNum + 1];
		result.z = vertices[3*iVertexNum + 2];
	}
  
	return result;
}

ofVec2f ofxPolygonObject::getVertexTexCoords(int iVertexNum)
{
	if(iVertexNum < numVertices){
		return ofVec2f(texCoords[2*iVertexNum], texCoords[2*iVertexNum + 1]);
	}
}

void ofxPolygonObject::setVertexColor(int iVertexNum, float iR, float iG, float iB, float iA)
{
	if(iVertexNum < numVertices){
		vertexColors[4*iVertexNum] = iR/255.0f;
		vertexColors[4*iVertexNum + 1] = iG/255.0f;
		vertexColors[4*iVertexNum + 2] = iB/255.0f;
		vertexColors[4*iVertexNum + 3] = drawMaterial->color.w/255.0f * iA/255.0f;
		//automatically enable vertex coloring if this is called
		vertexColoringEnabled = true;
	}
}

//OF_FILLED, OF_OUTLINE
void ofxPolygonObject::setDrawMode(int iDrawMode)
{
	drawMode = iDrawMode;
}

void ofxPolygonObject::setTexture(ofImage *iTex)
{
	texture = &iTex->getTextureReference();
}

void ofxPolygonObject::setTextureByReference(ofTexture &iTex) {
  texture = &iTex;
}

void ofxPolygonObject::enableVertexColoring(bool iEnable)
{
	vertexColoringEnabled = iEnable;
}

ofTexture* ofxPolygonObject::getTexture()
{
	return texture;
}