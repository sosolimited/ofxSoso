#include "ofxFboObject.h"

ofxFboObject::ofxFboObject(int iWidth, int iHeight, int iInternalFormat, int iNumSamples)
{
	fbo = new ofFbo();
	fbo->allocate(iWidth, iHeight, iInternalFormat, iNumSamples);
	fbo->getTextureReference().texData.bFlipTexture = false;
}

// Destructor.
ofxFboObject::~ofxFboObject(){
  delete fbo;
}

void ofxFboObject::render()
{
	fbo->draw(0,0);
}

void ofxFboObject::idle(float iTime){}
