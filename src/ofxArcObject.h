#pragma once

#include "ofxObject.h"

class ofxArcObject : public ofxObject{

public:
	ofxArcObject(float iOuterRadius, float iInnerRadius, float iStartAngle, float iEndAngle);
	~ofxArcObject();

	void						render();
	void						setResolution(int iRes);

	void						setStartAngle(float iAngle);
	void						setEndAngle(float iAngle);
	void						setColors(ofVec4f iColor1, ofVec4f iColor2);
	
public:	
	int							resolution;								
	float						outerRadius,
								innerRadius,
								startAngle,
								endAngle;		

	ofVec4f					color1, color2;
	float						height;


};
