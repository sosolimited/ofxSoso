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
  
  setScreenParams(orthographic, vFlip, fov, nearDist, farDist);
}

ofxSosoRenderer::~ofxSosoRenderer(){}

void ofxSosoRenderer::setScreenParams(bool iOrthographic, bool iVFlip, float iFov, float iNearDist, float iFarDist)
{
  // Set params.
  orthographic = iOrthographic;
  vFlip = iVFlip;
  fov = iFov;
  nearDist = iNearDist;
  farDist = iFarDist;
  
  // Rebuild projection and modelview matrices.
  
	float eyeX = width / 2;
	float eyeY = height / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) width / height;
  
	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;
  
  // Load perpsective values into projectionMatrix.
	projectionMatrix.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
  //cout << "ofxSosoRenderer : Perspective matrix built.\n";
  
  // Load lookAt values into modelViewMatrix.
  modelViewMatrix.makeLookAtViewMatrix( ofVec3f(0, 0, dist),  ofVec3f(0, 0, 0),  ofVec3f(0, 1, 0) );
  //lookAt.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist),  ofVec3f(eyeX, eyeY, 0),  ofVec3f(0, 1, 0) );
  
}

void ofxSosoRenderer::setupScreen()
{
  // Load the projection matrix.
  matrixMode(OF_MATRIX_PROJECTION);
	loadMatrix( projectionMatrix );
  
  // ModelView LookAt calculations are now multiplied into default scene root matrix in ofxScene.
  
}

