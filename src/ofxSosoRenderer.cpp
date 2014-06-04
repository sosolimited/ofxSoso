#include <iostream>
#include "ofxSosoRenderer.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

ofxSosoRenderer::ofxSosoRenderer(float iWidth, float iHeight, bool iOrthographic, bool iVFlip, float iFov, float iNearDist, float iFarDist):ofGLRenderer(true)
{
  width = iWidth;
  height = iHeight;
  orthographic = false; // Not supported yet.
  vFlip = iVFlip;
  fov = iFov;
  nearDist = iNearDist;
  farDist = iFarDist;
  
}

ofxSosoRenderer::~ofxSosoRenderer(){}

void ofxSosoRenderer::setupScreen()
{
  if(!orthographic)
    setupScreenPerspective(width, height, orthographic, vFlip, fov, nearDist, farDist);
  else
    cout << "oxfSosoRenderer : Orthographic not supported yet.\n";
}

void ofxSosoRenderer::setupScreenPerspective(float iWidth, float iHeight, bool iOrthographic, bool iVFlip, float iFov, float iNearDist, float iFarDist) {
  

 

  // Soso
	//if(width == 0) width = ofGetWidth();
	//if(height == 0) height = ofGetHeight();
	//float viewW = ofGetViewportWidth();
	//float viewH = ofGetViewportHeight();

  // Current OF approach uses cur viewport
  //ofRectangle currentViewport = getCurrentViewport();
	//float viewW = currentViewport.width;
	//float viewH = currentViewport.height;
  
  
  
	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;
  
	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;
  
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	
  
  matrixMode(OF_MATRIX_PROJECTION);
	ofMatrix4x4 persp;
	persp.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
	loadMatrix( persp );


  
  // Soso
  matrixMode(OF_MATRIX_MODELVIEW);
  loadIdentityMatrix();
  //cout << "dist = " << dist << "\n";
  
  gluLookAt(0, 0, dist,
            0, 0, 0,
            0, 1, 0);
  
  
	//ofMatrix4x4 lookAt;
	////lookAt.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0) );
  //lookAt.makeLookAtViewMatrix( ofVec3f(0, 0, dist),  ofVec3f(0, 0, 0),  ofVec3f(0, 1, 0) );
	//loadMatrix(lookAt);
  
  
  
  
}
