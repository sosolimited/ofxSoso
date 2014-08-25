#include "soso/Animation.h"
#include "cinder/app/App.h"

using namespace std;
using namespace cinder;

//class Tween
Tween::Tween(Object *iObject, Message *iMessage)
{
	object = iObject;
	message = iMessage;
}

Tween::~Tween()
{
	stop();
	delete(message);
}

void Tween::start()
{
	message->setStartTime( app::getElapsedSeconds() );
	message->enableMessage(true);
	message->isRunning = false;
	object->doMessage(message);
}

void Tween::stop()
{
	message->enableMessage(false);
	object->removeMessage(message);
}




//class Animation
Animation::Animation()
{

}

Animation::~Animation()
{
	stop();
	for(unsigned int i=0; i < tweens.size(); i++){
		delete(tweens[i]);
		tweens.erase(tweens.begin() + i);
		i--;
	}
}

void Animation::start()
{
	for(unsigned int i=0; i < tweens.size(); i++){
		tweens[i]->start();
	}
}

void Animation::stop()
{
	for(unsigned int i=0; i < tweens.size(); i++){
		tweens[i]->stop();
	}
}

//Note: This relies on the fact that Messages are "enabled" until they are completed.
bool Animation::isAnimating()
{
	bool animating = false;

	//adding second part of OR to account for delay period before isRunning gets set
	for(unsigned int i=0; i < tweens.size(); i++){
		if(tweens[i]->message->isRunning ||
           (app::getElapsedSeconds() >= tweens[i]->message->startTime && app::getElapsedSeconds() < tweens[i]->message->startTime + tweens[i]->message->startDelay))
			animating = true;
	}

	return animating;
}

float Animation::getDuration()
{
	float dur = 0;
	for(unsigned int i=0; i < tweens.size(); i++){
		dur = MAX(dur, (float)(tweens[i]->message->startDelay + tweens[i]->message->duration));
	}

	return dur;
}

Tween* Animation::tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, float iX0, float iX1, int iPlayMode)
{
	//make target vals
	float *args = new float[1];
	args[0] = iX1;
	//make message
	Message *message = new Message(iID, (void *)args, iInterp, iEndTime-iStartTime, iStartTime, iPlayMode);
	message->setBaseStartVals(iX0);
	//message->isRelative = false;
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

Tween* Animation::tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, float iX0, float iY0, float iZ0, float iX1, float iY1, float iZ1, int iPlayMode)
{
	//make target vals
	ci::Vec3f *args = new ci::Vec3f();
	args->set(iX1, iY1, iZ1);
	//make message
	Message *message = new Message(iID, (void *)args, iInterp, iEndTime-iStartTime, iStartTime, iPlayMode);
	message->setBaseStartVals(iX0, iY0, iZ0);
	//message->isRelative = false;
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

Tween* Animation::tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, float iX0, float iY0, float iZ0, float iW0, float iX1, float iY1, float iZ1, float iW1, int iPlayMode)
{
	//make target vals
	ci::Vec4f *args = new ci::Vec4f();
	args->set(iX1, iY1, iZ1, iW1);
	//make message
	Message *message = new Message(iID, (void *)args, iInterp, iEndTime-iStartTime, iStartTime, iPlayMode);
	message->setBaseStartVals(iX0, iY0, iZ0, iW0);
	//message->isRelative = false;
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

Tween* Animation::tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, ci::Vec3f iV0, ci::Vec3f iV1, int iPlayMode)
{
	return tween(iObject,iID, iStartTime, iEndTime, iInterp, iV0.x, iV0.y, iV0.z, iV1.x, iV1.y, iV1.z, iPlayMode);
}

Tween* Animation::tween(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, ci::Vec4f iV0, ci::Vec4f iV1, int iPlayMode)
{
	return tween(iObject,iID, iStartTime, iEndTime, iInterp, iV0.x, iV0.y, iV0.z, iV0.w, iV1.x, iV1.y, iV1.z, iV1.w, iPlayMode);
}

Tween* Animation::showObject(Object *iObject, float iTime, int iPlayMode)
{
	Message *message = new Message(OF_SHOW, NULL, OF_LINEAR, 0, iTime, iPlayMode);
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

Tween* Animation::hideObject(Object *iObject, float iTime, int iPlayMode)
{
	Message *message = new Message(OF_HIDE, NULL, OF_LINEAR, 0, iTime, iPlayMode);
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

//This method lets you specify bezier animation paths, through a vector of ci::Vec4f, which are treated as control points for the path.
Tween* Animation::tweenBezier(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, vector<ci::Vec4f> iPath, int iPlayMode)
{
	//make message
	Message *message = new Message(iID, iInterp, OF_BEZIER_PATH, iPath, iEndTime-iStartTime, iStartTime, iPlayMode);
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

//This method lets you specify spline animation paths, through a vector of ci::Vec4f, which are treated as control points for the path.
Tween* Animation::tweenSpline(Object *iObject, int iID, float iStartTime, float iEndTime, int iInterp, vector<ci::Vec4f> iPath, int iPlayMode)
{
	//make message
	Message *message = new Message(iID, iInterp, OF_SPLINE_PATH, iPath, iEndTime-iStartTime, iStartTime, iPlayMode);
	message->autoDelete = false;

	Tween *tween = new Tween(iObject, message);
	tweens.push_back(tween);

	return tween;
}

//This method lets you call an arbitrary function with arbitrary args at a specified time in the animation.
Tween* Animation::tweenFunc(Object *iObject, float (*iFunction)(void *), void *iArgs, float iTime, int iPlayMode) //LM
{
    //make message
    Message *message = new Message(OF_FUNCTION, iFunction, iArgs, iTime, iPlayMode);
    message->autoDelete = false;

    Tween *tween = new Tween(iObject, message);
    tweens.push_back(tween);

    return tween;
}


