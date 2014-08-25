#include "soso/DynamicPolygonObject.h"

using namespace std;
using namespace soso;

DynamicPolygonObject::DynamicPolygonObject(int iNumVertices):PolygonObject(iNumVertices)
{
	for(int i=0; i < numVertices; i++){

		auto v = make_shared<DynamicPolygonVert>();
		v->posTracker = make_shared<Object>();
		addChild(v->posTracker);
		v->texTracker = make_shared<Object>();
		addChild(v->texTracker);
		v->colorTracker = make_shared<Object>();
		v->colorTracker->setColor(255, 255, 255, 255);
		addChild(v->colorTracker);

		dynamicVerts.push_back(v);
	}
}

DynamicPolygonObject::~DynamicPolygonObject(){}

void DynamicPolygonObject::idle(float iTime)
{
	for(int i=0; i < numVertices; i++){
		PolygonObject::setVertexPos(i, dynamicVerts[i]->posTracker->getTrans());
		PolygonObject::setVertexTexCoords(i, dynamicVerts[i]->texTracker->getTrans().x, dynamicVerts[i]->texTracker->getTrans().y);
		if(vertexColoringEnabled)
			PolygonObject::setVertexColor(i, dynamicVerts[i]->colorTracker->getColor().r, dynamicVerts[i]->colorTracker->getColor().g, dynamicVerts[i]->colorTracker->getColor().b, dynamicVerts[i]->colorTracker->getColor().a);
	}
}


void DynamicPolygonObject::setVertexPos(int iVertexNum, ci::Vec3f iPos)
{
	if((iVertexNum >= 0) && (iVertexNum < numVertices)){
		dynamicVerts[iVertexNum]->posTracker->stopMessages();
		dynamicVerts[iVertexNum]->posTracker->setTrans(iPos);
	}
}

void DynamicPolygonObject::setVertexTexCoords(int iVertexNum, float iU, float iV)
{
	if((iVertexNum >= 0) && (iVertexNum < numVertices)){
		dynamicVerts[iVertexNum]->texTracker->stopMessages();
		dynamicVerts[iVertexNum]->texTracker->setTrans(ci::Vec3f(iU, iV, 0));
	}
}

void DynamicPolygonObject::setVertexColor(int iVertexNum, float iR, float iG, float iB, float iA)
{
	vertexColoringEnabled = true;

	if((iVertexNum >= 0) && (iVertexNum < numVertices)){
		dynamicVerts[iVertexNum]->colorTracker->stopMessages();
		dynamicVerts[iVertexNum]->colorTracker->setColor(ci::Vec4f(iR, iG, iB, iA));
	}
}


void DynamicPolygonObject::gotoVertexPos(int iVertexNum, ci::Vec3f iPos, float iDur, float iDelay, int iInterp)
{
	if((iVertexNum >= 0) && (iVertexNum < numVertices)){
		dynamicVerts[iVertexNum]->posTracker->stopMessages();
		if(iDur == -1.0)
			dynamicVerts[iVertexNum]->posTracker->setTrans(iPos);
		else
			dynamicVerts[iVertexNum]->posTracker->doMessage3f(OF_TRANSLATE, iDelay, iDur, iInterp, iPos.x, iPos.y, iPos.z);

	}
}

void DynamicPolygonObject::gotoVertexTexCoords(int iVertexNum, float iU, float iV, float iDur, float iDelay, int iInterp)
{
	if((iVertexNum >= 0) && (iVertexNum < numVertices)){
		dynamicVerts[iVertexNum]->texTracker->stopMessages();
		if(iDur == -1.0)
			dynamicVerts[iVertexNum]->texTracker->setTrans(ci::Vec3f(iU, iV, 0));
		else
			dynamicVerts[iVertexNum]->texTracker->doMessage3f(OF_TRANSLATE, iDelay, iDur, iInterp, iU, iV, 0);

	}

}

void DynamicPolygonObject::gotoVertexColor(int iVertexNum, float iR, float iG, float iB, float iA, float iDur, float iDelay, int iInterp)
{
	vertexColoringEnabled = true;

	if((iVertexNum >= 0) && (iVertexNum < numVertices)){
		dynamicVerts[iVertexNum]->colorTracker->stopMessages();
		if(iDur == -1.0)
			dynamicVerts[iVertexNum]->colorTracker->setColor(ci::Vec4f(iR, iG, iB, iA));
		else
			dynamicVerts[iVertexNum]->texTracker->doMessage3f(OF_SETCOLOR, iDelay, iDur, iInterp, iR, iG, iB);
			dynamicVerts[iVertexNum]->texTracker->doMessage1f(OF_SETALPHA, iDelay, iDur, iInterp, iA);

	}

}