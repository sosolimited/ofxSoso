#include "ofxVideoPlayerObject.h"

ofxVideoPlayerObject::ofxVideoPlayerObject(char *iPath)
{
	player = new ofVideoPlayer();	
	player->loadMovie(iPath);
	player->getTextureReference().texData.bFlipTexture = true;	

	isCentered = false;
	isAutoIdling = true;
		
}

ofxVideoPlayerObject::~ofxVideoPlayerObject(){}

void ofxVideoPlayerObject::idle(float iTime)
{		
	if(isAutoIdling) player->idleMovie();	
}

void ofxVideoPlayerObject::render()
{
	player->getTextureReference().texData.bFlipTexture = true;	//This has to be called here every frame to keep it flipped right.

	if(isCentered) player->draw(-player->width/2, -player->height/2);
	else player->draw(0,0);		
}

void ofxVideoPlayerObject::start()
{
	player->play();
}

void ofxVideoPlayerObject::stop()
{
	player->stop();
}

void ofxVideoPlayerObject::reset()
{
	player->setFrame(0);
	player->stop();
}

void ofxVideoPlayerObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
}