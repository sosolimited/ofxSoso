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
    float                       getStartAngle(); //LM 062012
    float                       getEndAngle(); //LM 062012
	void						setColors(ci::Vec4f iColor1, ci::Vec4f iColor2);

public:
	int							resolution;
	float						outerRadius,
								innerRadius,
								startAngle,
								endAngle;

	ci::Vec4f                     color1, color2, nullColor;
	float						height;


};
