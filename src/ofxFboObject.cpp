#include "ofxFboObject.h"

using namespace cinder;

ofxFboObject::ofxFboObject(int iWidth, int iHeight, int iInternalFormat, int iNumSamples)
{
	gl::Fbo::Format format;
	format.setColorInternalFormat( iInternalFormat );
	format.setCoverageSamples( iNumSamples );
	fbo = gl::Fbo( iWidth, iHeight, format );
	fbo.getTexture().setFlipped( false );
}

ofxFboObject::~ofxFboObject(){
}

void ofxFboObject::render()
{
	gl::draw( fbo.getTexture() );
}

void ofxFboObject::idle(float iTime){}
