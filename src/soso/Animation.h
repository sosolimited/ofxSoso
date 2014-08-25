/*
Animation

Animation can be used to animate the position, scale, rotation, color, alpha, and shown/hidden status of Objects.
You animate by "tweening" Objects. For each tween, specify an object, a start/end time, a start/end value, and the interpolation.
Tweens can also be played once or looped. See tweenBez and tweenSpline for creating non-linear tween paths.

Soso OpenFrameworks Addon
Copyright (C) 2012 Sosolimited

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#pragma once

#include "soso/Object.h"
#include "soso/Message.h"

namespace soso
{

class Tween
{
public:
	Tween(Object *iObject, Message *iMessage);
	~Tween();

	void							start();
	void							stop();

public:
	Object						*object;
	Message						*message;
};


class Animation
{
public:
	Animation();
	~Animation();

	void							start();
	void							stop();
	bool							isAnimating();
	float							getDuration();


	Tween*						tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
											float iX0, float iX1,
											int iPlayMode=OF_ONE_TIME_PLAY);
	Tween*						tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
											float iX0, float iY0, float iZ0,
											float iX1, float iY1, float iZ1,
											int iPlayMode=OF_ONE_TIME_PLAY);
	Tween*						tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
											float iX0, float iY0, float iZ0, float iW0,
											float iX1, float iY1, float iZ1, float iW1,
											int iPlayMode=OF_ONE_TIME_PLAY);

	Tween*						tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
											ci::Vec3f iV0, ci::Vec3f iV1,
											int iPlayMode=OF_ONE_TIME_PLAY);
	Tween*						tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp,
											ci::Vec4f iV0, ci::Vec4f iV1,
											int iPlayMode=OF_ONE_TIME_PLAY);

	Tween*						showObject(Object *iObject, float iTime, int iPlayMode=OF_ONE_TIME_PLAY);
	Tween*						hideObject(Object *iObject, float iTime, int iPlayMode=OF_ONE_TIME_PLAY);

	Tween*						tweenBezier(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, std::vector<ci::Vec4f> iPath, int iPlayMode=OF_ONE_TIME_PLAY);
	Tween*						tweenSpline(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, std::vector<ci::Vec4f> iPath, int iPlayMode=OF_ONE_TIME_PLAY);

	Tween*						tweenFunc(Object *iObject,float (*iFunction)(void *), void *iArgs, float iTime, int iPlayMode=OF_ONE_TIME_PLAY); //LM

public:
	std::vector<Tween *>				tweens;

};

} // namespace soso
