#include "ofxVideoPlayerObject.h"

ofxVideoPlayerObject::ofxVideoPlayerObject(char *iPath)
{
    player = new ofxAVFVideoPlayer();
  
    setUseAmplitudes(false); // Disable raw audio storage, ideally do this BEFORE memory load
  
    player->loadMovie(iPath);
    player->getTextureReference().texData.bFlipTexture = true;
        
    isCentered = false;
    isAutoIdling = true;
  
}

ofxVideoPlayerObject::~ofxVideoPlayerObject(){
    delete player;
    if(mShader) delete mShader;
}

void ofxVideoPlayerObject::idle(float iTime)
{		
	//if(isAutoIdling) player->idleMovie();
  player->update();
  if (isPlaying) {
    player->play();
  }
}

void ofxVideoPlayerObject::render()
{
    if(!player->isLoaded()) return;
    
    player->getTextureReference().texData.bFlipTexture = true;	//This has to be called here every frame to keep it flipped right.

    if(mShader) {
        mShader->begin();
        mShader->setUniformTexture( "u_texture", player->getTextureReference(), 0);
        float bounds[4] = {0, 0, player->getWidth(), player->getHeight()};
        mShader->setUniform4fv( "u_texture_bounds", &bounds[0]);
    }
    
    // TODO: Need a way to set alpha that actually works
    // ofSetColor(255,255,255, getAlpha());

    
	if(isCentered) player->draw(-player->getWidth()/2, -player->getHeight()/2);
	else player->draw(0,0);
    
    if(mShader)
        mShader->end();
    
}

void ofxVideoPlayerObject::start()
{
  isPlaying = true;
	player->play();
}

void ofxVideoPlayerObject::pause()
{
  isPlaying = false;
	player->setPaused(true);
}

void ofxVideoPlayerObject::stop()
{
  isPlaying = false;
	player->stop();
}

void ofxVideoPlayerObject::reset()
{
	player->setPosition(0);
  if (isPlaying) {
    player->play();
  }
}

void ofxVideoPlayerObject::setCentered(bool iEnable)
{
	isCentered = iEnable;
}

// AO, 6/17/14:
// If amplitudes are enabled, video player stores raw sound memory
// This eats up memory, leave disabled by default
void ofxVideoPlayerObject::setUseAmplitudes(bool iEnable)
{
	usingAmplitudes = iEnable;
  
  if (!iEnable)
    player->disableAmplitude();
  else player->enableAmplitude();
  
}

void ofxVideoPlayerObject::useShaders(string vert, string frag) {
    mShader = new ofShader();
    mShader->load(vert, frag);
}