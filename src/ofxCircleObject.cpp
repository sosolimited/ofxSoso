#include "ofxCircleObject.h"


ofxCircleObject::ofxCircleObject(int iResolution, float iOuterRadius, float iInnerRadius)
{
	resolution = iResolution;

	outerRadius = iOuterRadius;
	innerRadius = iInnerRadius;	

	numCirclePoints = 4*resolution;
	circlePoints = new float[numCirclePoints];

	
}

ofxCircleObject::~ofxCircleObject(){}

void ofxCircleObject::render()
{		
//	ofFill();

	float angleInc = 2 * M_PI/(float)resolution;

	glBegin(GL_TRIANGLE_STRIP);
	for(int i = 0; i < (resolution+1); i++){		
		float angle = (float)i*angleInc;
		glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), 0);
		glVertex3f(outerRadius * cos(angle), outerRadius * sin(angle), 0);				
	}
	glEnd();
}