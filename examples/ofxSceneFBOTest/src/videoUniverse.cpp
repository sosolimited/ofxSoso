//
//  videoUniverse.cpp
//  baccaratApp
//
//  Created by Sam Kronick on 5/7/13.
//
//

#include "videoUniverse.h"


videoUniverse::videoUniverse(int width, int height) {
    
    this->width = width;
    this->height = height;
    
    screenFov = 0.5f;
        
    //scene = new ofxScene(width, height);
    scene = new tempScene(width, height);
    scene->setRenderMode(tempScene::RENDER_NORMAL);
	scene->setBackgroundColor(200, 200, 200);
    scene->getRoot()->setTrans(0, 0, 0);
    
    root = new ofxObject();
    root->setTrans(width/2.0, -height/2.0, 0); //Position the root at the center
    scene->getRoot()->addChild(root);
    
    ofxRectangleObject *rootV = new ofxRectangleObject(250.0, 250.0);
    rootV->setColor(255., 255., 0.);
    rootV->isCentered = true;
    rootV->setTrans(100,100,0);
    root->addChild(rootV);
    
    //following same logic as SE
    //TODO: viewports also failing
    //addViewport(0, -width/2.0, -height/2.0, -width/2.0, -height/2.0, width, height);
    
    //Create FBO.
    fbo = new ofxFboObject(width, height, GL_RGBA);
    addChild(fbo);
    
    fbo->fbo->begin();
	ofClear(255,255,255, 0);
    fbo->fbo->end();

}

videoUniverse::~videoUniverse() { }

void videoUniverse::idle(float time)
{
    scene->update(time);
}

void videoUniverse::drawFBOScene() {
    
    //scene is rendered to the FBO
    fbo->fbo->begin();
    
    //TODO: setupViewports also failing
    //setupViewport(viewports[0]);
    
    ofEnableAlphaBlending();
//    ofClear(140,128,128,255);
    
    scene->draw(); //TODO: scene draw is breaking the fbo
    
    /*
    ofPushMatrix();
    ofTranslate(width*0.5, height*0.5, 5);
    ofFill();
    ofSetColor(255,0,255,128);
	ofRect(0,0,100,100);
	ofPopMatrix();
    */
    
    fbo->fbo->end();
}

ofxFboObject* videoUniverse::getFbo() {
    return fbo;
}

//code snatched from Schneider

//--------------------------------------------------------------
tempViewportData* videoUniverse::addViewport(int iScreenNum, int iScreenX, int iScreenY, int iWorldX, int iWorldY, int iWidth, int iHeight)
{
	tempViewportData *v = new tempViewportData(iScreenNum, iScreenX, iScreenY, iWorldX, iWorldY, iWidth, iHeight);
	viewports.push_back(v);
    
	printf("sosoApp::addViewport(%02d):\nscreen: %d, %d - world: %d, %d - dimensions: %d, %d\n", iScreenNum, iScreenX, iScreenY, iWorldX, iWorldY, iWidth, iHeight);
    
	return v;
}

void videoUniverse::setupViewport(tempViewportData *iView)
{
	//int width, height;
    
	//width  = ofGetWidth();
	//height = ofGetHeight();
    
	//height = height > 0 ? height : 1;
	// set viewport, clear the screen
	//glViewport( 0, 0, width, height );
    
	glViewport(iView->screenOrigin[0], iView->screenOrigin[1], iView->dimensions[0], iView->dimensions[1]);
    
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();
    
    // to do non auto clear on PC for now - we do something like "single" buffering --
    // it's not that pretty but it work for the most part
#ifdef TARGET_WIN32
    if (bClearAuto == false){
        glDrawBuffer (GL_FRONT);
    }
#endif
    
	//this stuff is handled in ofxScene::draw() already
	//if ( bClearAuto == true || nFrameCount < 3){
    //glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}
    
	//scissor scene to viewport dimensions
	if(scene) scene->setScissorParameters(iView->screenOrigin[0], iView->screenOrigin[1], iView->dimensions[0], iView->dimensions[1]);
    
	//if( bEnableSetupScreen )ofSetupScreen();
	//ofSetupScreen();
	
    setupMultiScreen(iView->screenNum);	//this is the special version for doing multiscreen
}

//--------------------------------------------------------------
//soso - taken from manor's ofSetupMultiscreen() in ofGraphics.cpp
//so that we can do all of the multiscreen stuff from within sosoApp instead of ofAppGlutWindow
void videoUniverse::setupMultiScreen(int iScreenIndex){
    
	int w, h, b, n;
    
	//w = ofGetWidth();
	w = viewports[iScreenIndex]->dimensions[0];	//temp!
	h = height;
	//n = ofGetNumScreens();
	n = viewports.size();
	//b = ofGetScreenBezel();
	b = 0;
    
    // num bezels per world
	int worldWidth = n*w + (n-1)*b;
	int screenWidth = w;
	int screenHeight = h;
	//int screenNum = ofGetScreenIndex();
	int screenNum = iScreenIndex;
    
	float halfFov, theTan, aspect;
    
	//PEND: fov could be dynamic, based on the lookat for more advanced versions
	//screenFov 		= 0.5f;
    
	float eyeX 		= (float)worldWidth / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 2.0;
	aspect 			= (float)w/(float)h;
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	//gluPerspective(screenFov, aspect, 1.0, 100000.0);	//SOSO
	gluLookAt(0, 0., dist,		//SOSO - this has to be called before glMatrixMode(GL_MODELVIEW) below
              -worldWidth/2 + screenNum*(w+b) + w/2, 0., 0.,
              0., 1.0, 0.);
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);	//SOSO moved above
    
}