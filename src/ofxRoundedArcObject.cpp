/*
 *  ofxRoundedArcObject.cpp
 *  LG_MusicApp
 *
 *  Created by jroth on 3/5/10.
 *  Copyright 2010 Sosolimited. All rights reserved.
 *
 */

#include "ofxRoundedArcObject.h"
//#include "ofxArcObject.h"

//start and end angles in degrees (converted to radians internally)
ofxRoundedArcObject::ofxRoundedArcObject(float iOuterRadius, float iInnerRadius, float iStartAngle, float iEndAngle)
:ofxArcObject(iOuterRadius,  iInnerRadius,  iStartAngle,  iEndAngle)
{
	//inner rounded corner
	curveRadius = 5;
	
	//where the inner corner circles are located
	newInnerRadius = innerRadius + curveRadius;

	deltaAngle = atan2(curveRadius, iInnerRadius + curveRadius);
	curveRadius2 = deltaAngle*outerRadius / (1 + deltaAngle);
	
	//where the outer corner circles are located
	newOuterRadius = outerRadius - curveRadius2;
	
	
	newStartAngle = startAngle + deltaAngle;
	newEndAngle = endAngle - deltaAngle;
	
	circPos1.set(newInnerRadius*cos(newStartAngle), newInnerRadius*sin(newStartAngle), 0);
	circPos2.set(newOuterRadius*cos(newStartAngle), newOuterRadius*sin(newStartAngle), 0);
	circPos3.set(newInnerRadius*cos(newEndAngle), newInnerRadius*sin(newEndAngle), 0);
	circPos4.set(newOuterRadius*cos(newEndAngle), newOuterRadius*sin(newEndAngle), 0);
	
	/*
	static int numArcs = 0;
	id = numArcs++;
	
	resolution = 100;
	
	outerRadius = iOuterRadius;
	innerRadius = iInnerRadius;	
	
	//add -90 offset if you want 0 degrees at 12 o'clock
	startAngle = ofDegToRad(iStartAngle);	
	endAngle = ofDegToRad(iEndAngle);		
	 */
	
	height = 60;
	
}

ofxRoundedArcObject::~ofxRoundedArcObject(){

}

/*
void ofxRoundedArcObject::setResolution(int iRes)
{

	resolution = iRes;

}
*/

void ofxRoundedArcObject::render()
{		

	if (renderDirty) {
		
		// PEND PORT - v2.59 - opengles
		glDeleteLists (displayList, 1);
		glNewList(displayList, GL_COMPILE_AND_EXECUTE);
				
		//corner circle extrusions
		glBegin(GL_QUAD_STRIP);	
		for (float i = newStartAngle + PI; i <= newStartAngle + 3*PI/2 ; i+= PI/16.0f) {
			//glColor3f((i-(newStartAngle + PI))/PI/2.0, 0, 1-(i-(newStartAngle + PI))/PI/2.0);
			glNormal3f(cos(i), sin(i), 0);
			glVertex3f(circPos1.x + curveRadius*cos(i), circPos1.y + curveRadius*sin(i), height);
			glVertex3f(circPos1.x + curveRadius*cos(i), circPos1.y + curveRadius*sin(i), 0);	
		}
		glEnd();
		
		glBegin(GL_QUAD_STRIP);	
		for (float i = newEndAngle +7*PI/16; i <= newEndAngle + PI ; i+= PI/16.0f) {
			//glColor3f((i-(newEndAngle + PI/2.0))/PI/2.0, 0, 1-(i-(newEndAngle + PI/2.0))/PI/2.0);
			glNormal3f(cos(i), sin(i), 0);
			glVertex3f(circPos3.x + curveRadius*cos(i), circPos3.y + curveRadius*sin(i), height);
			glVertex3f(circPos3.x + curveRadius*cos(i), circPos3.y + curveRadius*sin(i), 0);
		}
		glEnd();

		/*
		glBegin(GL_QUAD_STRIP);	
		for (float i = 0 ; i <= 2.0*PI ; i+= PI/4.0f) {
			glNormal3f(cos(i), sin(i), 0);
			glVertex3f(circPos2.x + curveRadius2*cos(i), circPos2.y + curveRadius2*sin(i), height);
			glVertex3f(circPos2.x + curveRadius2*cos(i), circPos2.y + curveRadius2*sin(i), 0);
			
		}
		glEnd();		
		glBegin(GL_QUAD_STRIP);	
		for (float i = 0 ; i <= 2.0*PI ; i+= PI/4.0f) {
			glNormal3f(cos(i), sin(i), 0);
			glVertex3f(circPos4.x + curveRadius2*cos(i), circPos4.y + curveRadius2*sin(i), height);
			glVertex3f(circPos4.x + curveRadius2*cos(i), circPos4.y + curveRadius2*sin(i), 0);
			
			
		}
		glEnd();		
		
		 
		
		//beginning and end caps
		
		glBegin(GL_QUADS); 
		
		glNormal3f(cos(startAngle+PI/2), sin(startAngle+PI/2), 0);
		glVertex3f((newInnerRadius)*cos(startAngle), (newInnerRadius)*sin(startAngle), height);
		glVertex3f((newOuterRadius)*cos(startAngle), (newOuterRadius)*sin(startAngle), height);
		glVertex3f((newOuterRadius)*cos(startAngle), (newOuterRadius)*sin(startAngle), 0);
		glVertex3f((newInnerRadius)*cos(startAngle), (newInnerRadius)*sin(startAngle), 0);
		

		glNormal3f(cos(startAngle-PI/2), sin(startAngle-PI/2), 0);
		glVertex3f((newInnerRadius)*cos(startAngle), (newInnerRadius)*sin(startAngle), height);
		glVertex3f((newOuterRadius)*cos(startAngle), (newOuterRadius)*sin(startAngle), height);
		glVertex3f((newOuterRadius)*cos(startAngle), (newOuterRadius)*sin(startAngle), 0);
		glVertex3f((newInnerRadius)*cos(startAngle), (newInnerRadius)*sin(startAngle), 0);
		
		glEnd();	
		*/
		

		
		int k = 0;
		float angleInc = M_TWO_PI/(float)resolution;
		float angle = newStartAngle;
		
		//inner side
		glBegin(GL_QUAD_STRIP);
		
		while(angle < newEndAngle){
			glNormal3f(cos(angle+PI), sin(angle+PI), 0);			
			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), 0);
			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), height);
			
			angle += angleInc;
		}
		//finish it
		glNormal3f(cos(angle+PI), sin(angle+PI), 0);		
		glVertex3f(innerRadius * cos(newEndAngle), innerRadius * sin(newEndAngle), 0);
		glVertex3f(innerRadius * cos(newEndAngle), innerRadius * sin(newEndAngle), height);
		glVertex3f(innerRadius * cos(newEndAngle+PI/360.0), innerRadius * sin(newEndAngle+PI/360.0), 0);
		glVertex3f(innerRadius * cos(newEndAngle+PI/360.0), innerRadius * sin(newEndAngle+PI/360.0), height);
		
		glEnd();	
		
		/*
		//outer side
		glBegin(GL_QUAD_STRIP);
		
		while(angle < newEndAngle){
			glNormal3f(cos(angle), sin(angle), 0);
			glVertex3f(outerRadius * cos(angle), outerRadius * sin(angle), 0);
			glVertex3f(outerRadius * cos(angle), outerRadius * sin(angle), height);
			
			angle += angleInc;
		}
		//finish it
		glNormal3f(cos(newEndAngle), sin(newEndAngle), 0);
		glVertex3f(outerRadius * cos(newEndAngle), outerRadius * sin(newEndAngle), 0);
		glVertex3f(outerRadius * cos(newEndAngle), outerRadius * sin(newEndAngle), height);
		
		glEnd();	
		*/
		/*
		//trapezoids filling in missing part between corners
		glBegin(GL_QUADS); 
		glNormal3f(0,0,1);
		glVertex3f((innerRadius )*cos(startAngle), (innerRadius + curveRadius)*sin(startAngle), 0);
		glVertex3f((outerRadius)*cos(startAngle), (outerRadius - curveRadius2)*sin(startAngle), 0);
		glVertex3f((outerRadius)*cos(newStartAngle), (outerRadius)*sin(newStartAngle), 0);
		glVertex3f((innerRadius )*cos(newStartAngle), (innerRadius)*sin(newStartAngle), 0);
		
		glVertex3f((innerRadius )*cos(newEndAngle), (innerRadius)*sin(newEndAngle), 0);
		glVertex3f((outerRadius )*cos(newEndAngle), (outerRadius)*sin(newEndAngle), 0);
		glVertex3f((outerRadius - curveRadius2)*cos(endAngle), (outerRadius - curveRadius2)*sin(endAngle), 0);
		glVertex3f((innerRadius + curveRadius)*cos(endAngle), (innerRadius + curveRadius)*sin(endAngle), 0);
		
		glEnd();		
		*/
		//BEGINNING & END curved strips
		glBegin(GL_QUAD_STRIP);
	
		//1st curved part of button
		for (float i = 0 ; i <= PI/2.0f ; i+= PI/8.0f) {
			glNormal3f(0,0,1);

			//glColor3f(1,0,0);
			glVertex3f(circPos1.x + curveRadius*cos(newStartAngle + PI + i), circPos1.y + curveRadius*sin(newStartAngle + PI + i), height);
			//glColor3f(0,0,1);
			glVertex3f(circPos2.x + curveRadius2*cos(newStartAngle - i), circPos2.y + curveRadius2*sin(newStartAngle - i), height);
			
		}
		
		glEnd();
		
		glBegin(GL_QUAD_STRIP);
		
		//end curved part of button
		for (float i = 0 ; i <= PI/2.0f ; i+= PI/8.0f) {
			glNormal3f(0,0,1);

			//glColor3f(0,1,0);
			glVertex3f(circPos3.x + curveRadius*cos(newEndAngle + PI - i ), circPos3.y + curveRadius*sin(newEndAngle + PI - i), height);
			//glColor3f(1,1,0);
			glVertex3f(circPos4.x + curveRadius2*cos(newEndAngle + i), circPos4.y + curveRadius2*sin(newEndAngle + i), height);
			
		}
		
		glEnd();
		
		//main part of button
		glBegin(GL_QUAD_STRIP);
		//glColor4f(1,1,1,1);
		angle = newStartAngle;
		while(angle < newEndAngle){
			glNormal3f(0,0,1);

			glVertex3f(innerRadius * cos(angle), innerRadius * sin(angle), height);
			glVertex3f(outerRadius * cos(angle), outerRadius * sin(angle), height);
			
			angle += angleInc;
		}
		//finish it
		glVertex3f(innerRadius * cos(newEndAngle), innerRadius * sin(newEndAngle), height);
		glVertex3f(outerRadius * cos(newEndAngle), outerRadius * sin(newEndAngle), height);
		
		glEnd();	

		glEndList();
		
		renderDirty = false;
		
	}	else {	
		
		glCallList(displayList);
		
	}
	
}

/*
 //these are all set in degrees and converted to radians internally
void ofxRoundedArcObject::setStartAngle(float iAngle)
{
	startAngle = ofDegToRad(iAngle);	
}

void ofxRoundedArcObject::setEndAngle(float iAngle)
{
	endAngle = ofDegToRad(iAngle);		
}
*/