#include <iostream>
#include "ofxSosoRenderer.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"

ofxSosoRenderer::ofxSosoRenderer(bool useShapeColor):ofGLRenderer(useShapeColor){}
ofxSosoRenderer::~ofxSosoRenderer(){}

void ofxSosoRenderer::setupScreen()
{
    setupScreenPerspective();
}

void ofxSosoRenderer::setupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist) {
       
    
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
    
	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();
    
	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;
    
	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearDist, farDist);
    
	//glMatrixMode(GL_MODELVIEW);       //from ofGLRenderer
	//glLoadIdentity();                 //from ofGLRenderer
	//gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0, 0, 1, 0);  //from ofGLRenderer
    
    gluLookAt(0, 0, dist, 0, 0, 0, 0, 1, 0);    
    glMatrixMode(GL_MODELVIEW);      
	glLoadIdentity();                
    
	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	if(ofDoesHWOrientation()){
		if(vFlip){
			glScalef(1, -1, 1);
			glTranslatef(0, -height, 0);
		}
	}else{
		if( orientation == OF_ORIENTATION_UNKNOWN ) orientation = ofGetOrientation();
		switch(orientation) {
			case OF_ORIENTATION_180:
				glRotatef(-180, 0, 0, 1);
				if(vFlip){
					glScalef(1, -1, 1);
					glTranslatef(-width, 0, 0);
				}else{
					glTranslatef(-width, -height, 0);
				}
                
				break;
                
			case OF_ORIENTATION_90_RIGHT:
				glRotatef(-90, 0, 0, 1);
				if(vFlip){
					glScalef(-1, 1, 1);
				}else{
					glScalef(-1, -1, 1);
					glTranslatef(0, -height, 0);
				}
				break;
                
			case OF_ORIENTATION_90_LEFT:
				glRotatef(90, 0, 0, 1);
				if(vFlip){
					glScalef(-1, 1, 1);
					glTranslatef(-width, -height, 0);
				}else{
					glScalef(-1, -1, 1);
					glTranslatef(-width, 0, 0);
				}
				break;
                
			case OF_ORIENTATION_DEFAULT:
			default:
				if(vFlip){
					glScalef(1, -1, 1);
					glTranslatef(0, -height, 0);
				}
				break;
		}
	}
    
}
