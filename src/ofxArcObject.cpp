#include "ofxArcObject.h"
#include "cinder/CinderMath.h"

//start and end angles in degrees (converted to radians internally)
ofxArcObject::ofxArcObject(float iOuterRadius, float iInnerRadius, float iStartAngle, float iEndAngle)
{
	resolution = 100;

	outerRadius = iOuterRadius;
	innerRadius = iInnerRadius;

	//add -90 offset if you want 0 degrees at 12 o'clock
	startAngle = ci::toRadians(iStartAngle);
	endAngle = ci::toRadians(iEndAngle);

	height = 0;
    nullColor.set(-1,-1,-1,-1); // LM added this to fix compatability w 0072
	color1 = nullColor;
	color2 = nullColor;

}


ofxArcObject::~ofxArcObject(){}

void ofxArcObject::setResolution(int iRes)
{
	resolution = iRes;
}

//these are all set in degrees and converted to radians internally
void ofxArcObject::setStartAngle(float iAngle)
{
	startAngle = ci::toRadians(iAngle);
}

void ofxArcObject::setEndAngle(float iAngle)
{
	endAngle = ci::toRadians(iAngle);
}

//LM 062012
float ofxArcObject::getStartAngle() {
	return ci::toDegrees(startAngle);
}

float ofxArcObject::getEndAngle() {
    return ci::toDegrees(endAngle);
}

void ofxArcObject::setColors(ci::Vec4f iColor1, ci::Vec4f iColor2) {

	color1 = iColor1/255.0;
	color2 = iColor2/255.0;
}


void ofxArcObject::render()
{
	// note that we should do solid/filled rendering
//	ofFill();

	int k = 0;
	float angleInc = 2 * M_PI/(float)resolution;
	float angle = startAngle;
	float portion = 0;

	if (height > 0) {

		glBegin(GL_QUAD_STRIP);
		while(angle < endAngle){
			glNormal3f(cos(angle+M_PI), sin(angle+M_PI),0);
			if (color1 != nullColor && color2 != nullColor) {
				portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
				glColor4f(portion*color1[0] + (1.0-portion)*color2[0], portion*color1[1] + (1.0-portion)*color2[1], portion*color1[2] + (1.0-portion)*color2[2], portion*color1[3] + (1.0-portion)*color2[3]);

			}
			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), 0);
			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), height);

			angle += angleInc;
		}

		//finish it
		glNormal3f(cos(angle+M_PI), sin(angle+M_PI),0);

		if (color1 != nullColor && color2 != nullColor) {
			portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
			glColor4f(portion*color1[0] + (1.0-portion)*color2[0], portion*color1[1] + (1.0-portion)*color2[1], portion*color1[2] + (1.0-portion)*color2[2], portion*color1[3] + (1.0-portion)*color2[3]);

		}

		glVertex3f(innerRadius * cos(endAngle), innerRadius * sin(endAngle), 0);
		glVertex3f(innerRadius * cos(endAngle), innerRadius * sin(endAngle), height);

		glEnd();

	}
	angle = startAngle;
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0,0,1);
	while(angle < endAngle){

		if (color1 != nullColor && color2 != nullColor) {
			portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
			glColor4f(portion*color1[0] + (1.0-portion)*color2[0], portion*color1[1] + (1.0-portion)*color2[1], portion*color1[2] + (1.0-portion)*color2[2], portion*color1[3] + (1.0-portion)*color2[3]);
		}
		glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), height);
		glVertex3f(outerRadius * cos(angle), outerRadius * sin(angle), height);

		angle += angleInc;
	}
	//finish it
	if (color1 != nullColor && color2 != nullColor) {
		portion = 1.0 -(angle - startAngle) / (endAngle - startAngle);
		glColor4f(portion*color1[0] + (1.0-portion)*color2[0], portion*color1[1] + (1.0-portion)*color2[1], portion*color1[2] + (1.0-portion)*color2[2], portion*color1[3] + (1.0-portion)*color2[3]);

	}
	glVertex3f(innerRadius * cos(endAngle), innerRadius * sin(endAngle), height);
	glVertex3f(outerRadius * cos(endAngle), outerRadius * sin(endAngle), height);

	glEnd();

}