#include "soso/PolygonObject.h"

#include <algorithm>	// for min and max
using namespace cinder;

PolygonObject::PolygonObject(int iNumVertices)
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

PolygonObject::~PolygonObject()
{
	delete vertices;
}

void PolygonObject::render()
{
	if(texture){
		texture->enableAndBind();

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
		texture->disable();
	}

	if(vertexColoringEnabled)
		glDisableClientState(GL_COLOR_ARRAY);
}

void PolygonObject::setVertexPos(int iVertexNum, ci::Vec3f iPos)
{
	if(iVertexNum < numVertices){
		vertices[3*iVertexNum] = iPos.x;
		vertices[3*iVertexNum + 1] = iPos.y;
		vertices[3*iVertexNum + 2] = iPos.z;
	}
}

void PolygonObject::setVertexTexCoords(int iVertexNum, float iU, float iV)
{
	if(iVertexNum < numVertices){
		//texCoords[2*iVertexNum] = iU;
		//texCoords[2*iVertexNum + 1] = iV;

		//adding auto clamping
		if(texture){
			texCoords[2*iVertexNum] = std::min<float>(iU, texture->getWidth());
			texCoords[2*iVertexNum] = std::max(texCoords[2*iVertexNum], 0.0f);

			texCoords[2*iVertexNum + 1] = std::min<float>(iV, texture->getHeight());
			texCoords[2*iVertexNum + 1] = std::max(texCoords[2*iVertexNum + 1], 0.0f);
		}

	}
}

ci::Vec3f PolygonObject::getVertexPos(int iVertexNum)
{
	ci::Vec3f result;

	if(iVertexNum < numVertices){
		result.x = vertices[3*iVertexNum];
		result.y = vertices[3*iVertexNum + 1];
		result.z = vertices[3*iVertexNum + 2];
	}

	return result;
}

ci::Vec2f PolygonObject::getVertexTexCoords(int iVertexNum)
{
	if(iVertexNum < numVertices){
		return ci::Vec2f(texCoords[2*iVertexNum], texCoords[2*iVertexNum + 1]);
	}
}

void PolygonObject::setVertexColor(int iVertexNum, float iR, float iG, float iB, float iA)
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

//OF_FILLED, OF_OUTLINE
void PolygonObject::setDrawMode(int iDrawMode)
{
	drawMode = iDrawMode;
}

void PolygonObject::setTexture(const ci::gl::TextureRef &iTex)
{
	texture = iTex;
}

void PolygonObject::enableVertexColoring(bool iEnable)
{
	vertexColoringEnabled = iEnable;
}

gl::TextureRef PolygonObject::getTexture()
{
	return texture;
}