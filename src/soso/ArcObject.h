#pragma once

#include "soso/Object.h"

namespace soso
{

class ArcObject : public Object{

public:
	ArcObject(float iOuterRadius, float iInnerRadius, float iStartAngle, float iEndAngle);
	~ArcObject();

	void						render();
	void						setResolution(int iRes);

	void						setStartAngle(float iAngle);
	void						setEndAngle(float iAngle);
    float                       getStartAngle(); //LM 062012
    float                       getEndAngle(); //LM 062012
	void						setColors(ci::vec4 iColor1, ci::vec4 iColor2);

public:
	int							resolution;
	float						outerRadius,
								innerRadius,
								startAngle,
								endAngle;

	ci::vec4                     color1, color2, nullColor;
	float						height;


};

} // namespace soso
