#include <iostream>
#include "ofxSosoRenderer.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

ofxSosoRenderer::ofxSosoRenderer(float iWidth, float iHeight, bool iOrthographic, bool iVFlip, float iFov, float iNearDist, float iFarDist):ofGLRenderer(ofGetWindowPtr())
{
  width = iWidth;
  height = iHeight;
  orthographic = false; // Not supported yet.
  vFlip = iVFlip;
  fov = iFov;
  nearDist = iNearDist;
  farDist = iFarDist;
  
  setScreenParams(orthographic, vFlip, fov, nearDist, farDist);
}

ofxSosoRenderer::~ofxSosoRenderer(){}

void ofxSosoRenderer::setScreenParams(bool iOrthographic, bool iVFlip, float iFov, float iNearDist, float iFarDist)
{
	
	ofLogNotice("soso setup screen");
  // Set params.
  orthographic = iOrthographic;
  vFlip = iVFlip;
  fov = iFov;
  nearDist = iNearDist;
  farDist = iFarDist;
  
  // Rebuild projection and modelview matrices.
  
//	float eyeX = width / 2;
//	float eyeY = height / 2;
	float viewW = ofGetNumScreens() * ofGetViewportWidth() + 2 * ofGetScreenBezel() * ( ofGetNumScreens() - 1 ); //soso - this is actually world width
	float viewH = ofGetViewportHeight();
	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
//	float halfFov = PI * fov / 360;
	float halfFov = PI * 0.5f / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	

	float aspect = (float) ofGetViewportWidth() / viewH; //SOSO - aspect is not for universe, just one screen
//	float aspect = (float) width / height;
	
	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;
  
  // Load perpsective values into projectionMatrix.
	projectionMatrix.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
  //cout << "ofxSosoRenderer : Perspective matrix built.\n";
  
  // Load lookAt values into modelViewMatrix.
  modelViewMatrix.makeLookAtViewMatrix( ofVec3f(0, 0, dist),  ofVec3f(-eyeX + ofGetViewportWidth()/2 + ofGetScreenIndex()*(ofGetViewportWidth() + 2*ofGetScreenBezel()), 0, 0),  ofVec3f(0, 1, 0) );
  //lookAt.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0) );
  
}

void ofxSosoRenderer::setupScreen()
{
	
//	ofLogNotice("Setting up screen soso ");
	float viewW = ofGetNumScreens() * ofGetViewportWidth() + 2 * ofGetScreenBezel() * ( ofGetNumScreens() - 1 ); //soso - this is actually world width
	float viewH = ofGetViewportHeight();
	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
//  float eyeX = width / 2;
//	float eyeY = height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
//	float aspect = (float) width / height;
	
	float aspect = (float) ofGetViewportWidth() / viewH; //SOSO - aspect is not for universe, just one screen
  
	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;
  
  
  // Load the projection matrix.
  matrixMode(OF_MATRIX_PROJECTION);
  loadMatrix( projectionMatrix );
	
//	glMatrixMode(GL_MODELVIEW); //soso moved from above
//	glLoadIdentity(); //soso moved from above
  
  // ModelView LookAt calculations are now multiplied into default scene root matrix in ofxScene.
  
}

