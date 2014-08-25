#include "soso/FboObject.h"

using namespace cinder;

FboObject::FboObject(int iWidth, int iHeight, int iInternalFormat, int iNumSamples)
{
	gl::Fbo::Format format;
	format.setColorInternalFormat( iInternalFormat );
	format.setCoverageSamples( iNumSamples );
	fbo = gl::Fbo( iWidth, iHeight, format );
	fbo.getTexture().setFlipped( false );
}

FboObject::~FboObject(){
}

void FboObject::render()
{
	gl::draw( fbo.getTexture() );
}

void FboObject::idle(float iTime){}
