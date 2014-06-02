#include "ofxArcObject.h"


//start and end angles in degrees (converted to radians internally)
ofxArcObject::ofxArcObject(float iOuterRadius, float iInnerRadius, float iStartAngle, float iEndAngle)
{	
	resolution = 100;

	outerRadius = iOuterRadius;
	innerRadius = iInnerRadius;	

	//add -90 offset if you want 0 degrees at 12 o'clock
	startAngle = ofDegToRad(iStartAngle);	
	endAngle = ofDegToRad(iEndAngle);		
	
	height = 0;
	color1 = NULL;
	color2 = NULL;
	
}


ofxArcObject::~ofxArcObject(){}

void ofxArcObject::setResolution(int iRes)
{
	resolution = iRes;
}

//these are all set in degrees and converted to radians internally
void ofxArcObject::setStartAngle(float iAngle)
{
	startAngle = ofDegToRad(iAngle);	
}

void ofxArcObject::setEndAngle(float iAngle)
{
	endAngle = ofDegToRad(iAngle);		
}

//LM 062012
float ofxArcObject::getStartAngle() {
    return ofRadToDeg(startAngle);
}

float ofxArcObject::getEndAngle() {
    return ofRadToDeg(endAngle);
}

void ofxArcObject::setColors(ofColor *iColor1, ofColor *iColor2) {

	color1 = iColor1;
	color2 = iColor2;
}


void ofxArcObject::render()
{		
	ofFill();			
	
	int k = 0;
	float angleInc = M_TWO_PI/(float)resolution;
	float angle = startAngle;
	float portion = 0;
	
	if (height > 0) {
		
		glBegin(GL_QUAD_STRIP);
		while(angle < endAngle){
			glNormal3f(cos(angle+PI), sin(angle+PI),0);
			if (color1 != NULL && color2 != NULL) {
				portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
				glColor4f(portion*color1->r/255.0f + (1.0-portion)*color2->r/255.0f, portion*color1->g/255.0f + (1.0-portion)*color2->g/255.0f, portion*color1->b/255.0f + (1.0-portion)*color2->b/255.0f, portion*color1->a/255.0f + (1.0-portion)*color2->a/255.0f);
						  
			}
			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), 0);
			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), height);
			
			angle += angleInc;
		}
		
		//finish it
		glNormal3f(cos(angle+PI), sin(angle+PI),0);
		
		if (color1 != NULL && color2 != NULL) {
			portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
			glColor4f(portion*color1->r/255.0f + (1.0-portion)*color2->r/255.0f, portion*color1->g/255.0f + (1.0-portion)*color2->g/255.0f, portion*color1->b/255.0f + (1.0-portion)*color2->b/255.0f, portion*color1->a/255.0f + (1.0-portion)*color2->a/255.0f);
			
		}
		
		glVertex3f(innerRadius * cos(endAngle), innerRadius * sin(endAngle), 0);
		glVertex3f(innerRadius * cos(endAngle), innerRadius * sin(endAngle), height);
		
		glEnd();	
		
	}
	angle = startAngle;
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0,0,1);
	while(angle < endAngle){

		if (color1 != NULL && color2 != NULL) {
			portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
			glColor4f(portion*color1->r/255.0f + (1.0-portion)*color2->r/255.0f, portion*color1->g/255.0f + (1.0-portion)*color2->g/255.0f, portion*color1->b/255.0f + (1.0-portion)*color2->b/255.0f, portion*color1->a/255.0f + (1.0-portion)*color2->a/255.0f);
		}		
		glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), height);
		glVertex3f(outerRadius * cos(angle), outerRadius * sin(angle), height);
		
		angle += angleInc;
	}
	//finish it
	if (color1 != NULL && color2 != NULL) {
		portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
		glColor4f(portion*color1->r/255.0f + (1.0-portion)*color2->r/255.0f, portion*color1->g/255.0f + (1.0-portion)*color2->g/255.0f, portion*color1->b/255.0f + (1.0-portion)*color2->b/255.0f, portion*color1->a/255.0f + (1.0-portion)*color2->a/255.0f);
		
	}	
	glVertex3f(innerRadius * cos(endAngle), innerRadius * sin(endAngle), height);
	glVertex3f(outerRadius * cos(endAngle), outerRadius * sin(endAngle), height);
	
	glEnd();	
	
}