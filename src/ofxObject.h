/*
ofxObject

Base object class. All onscreen objects in the scene inherit from this, usually by defining their own render() method.
Each object has a translation, rotation, scale, color, and shown/hidden flag. Child objects can be added
to an object with addChild(). All the characteristics of an ofxObject can be animated using the doMessage() methods.
By default, children inherit the transparency of their parents, but this can be disabled by enableAlphaInheritance(false).

When an object is added to the render tree of an ofxScene, it's render() and idle(float iTime) methods are called once per frame.
See ofxVideoPlayerObject or ofxRectangleObject for examples of extending ofxObject to make your own objects.

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

#include "ofxMessage.h"

#include "cinder/Vector.h"

enum{
	OF_RENDER_ALL = 0x00,
	OF_RENDER_OPAQUE = 0x01,
	OF_RENDER_TRANSPARENT = 0x02,
	OF_RENDER_ONTOP = 0x03
};

enum ofFillFlag{
	OF_OUTLINE=	0,
	OF_FILLED = 1,
};


class ofxObjectMaterial{

public:
	ofxObjectMaterial();
	~ofxObjectMaterial();

public:
	ci::ColorA8u			color;
	bool							inheritAlphaFlag;	//firebrand
};

// using is equivalent to a typedef here, but maybe easier to read (C++11, DW)
using ofxObjectRef = std::shared_ptr<class ofxObject>;

class ofxObject {

	public :

	ofxObject();
	virtual ~ofxObject();

	int								addChild(ofxObjectRef child);
	void 							removeChild(ofxObjectRef child) { removeChild( child.get() ); }
	void							removeChild(ofxObject *child);
	//! variant of removeChild that is safe to call within idle()
	void              removeChildSafe(ofxObjectRef child);
	int 							isDescendant(ofxObjectRef iObject);

	virtual void					predraw();
	virtual void					postdraw();
	void							draw(ofxObjectMaterial *iMaterial, float *iMatrix, int iSelect=OF_RENDER_ALL, bool iDrawAlone=false);		//v4.0 added material arg
	virtual void					render();
	void							idleBase(float iTime);
	virtual void					idle(float iTime){};

	void							setTrans(ci::Vec3f vec);
	void							setTrans(float x, float y, float z);
	ci::Vec3f         getTrans();

	void							setRot(ci::Vec3f r);
	void							setRot(float x, float y, float z);
	ci::Vec3f                         getRot();
	float							getAlpha();

	virtual ci::Vec3f   				getScale();			//v4.0 virtual for text object
	virtual void     				setScale(float s);	//v4.0 virtual for text object
	virtual void     				setScale(float x, float y, float z);
	virtual void     				setScale(ci::Vec3f vec);

	virtual void					setColor(float r, float g, float b, float a=255.0);
	DEPRECATED_ATTRIBUTE // use setColor( Color ) instead
	virtual void					setColor(ci::Vec4f c);	//v4.0 virtual for text object
	virtual void          setColor(const ci::ColorA8u &c);
	ci::ColorA8u					getColor();
	virtual void					setAlpha(float iA);		//v4.0 virtual for text object
	bool							hasTransparency();
	void							setSpecialTransparency(bool iFlag);

	const ci::Matrix44f&		getMatrix();
	const ci::Matrix44f&		getLocalMatrix();
	virtual const ci::Matrix44f&	updateMatrix( const ci::Matrix44f &iParentMatrix );
	void							updateMatrices( const ci::Matrix44f &iParentMatrix = ci::Matrix44f::identity() );
	void							updateLocalMatrix();

    GLuint                          getDisplayList(){ return displayList; } //eg 070112
    void                            setDisplayList(GLuint iList);    //eg 070112

	ofxObjectMaterial*				updateMaterial(ofxObjectMaterial *iMat);
	void							enableAlphaInheritance(bool iEnable);	//firebrand

	int								collectNodes(int iSelect, ofxObject *iNodes[], int iNumber, int iMax);
	ci::Vec3f							getWindowCoords();

	void   							hide();
	void   							show();
	bool   							isShown();
	int								getID();
	bool							isAnimating();
	void							setLighting(bool iOnOff);


	//void							cleanupMessages();	//replaced by deleteMessage below
	void							updateMessages();
	void							deleteMessage(ofxMessage *iMessage);
	bool							hasMessage(ofxMessage *iMessage);
	bool							removeMessage(ofxMessage *iMessage);

	ofxMessage*						doMessage(ofxMessage *iMessage);
	ofxMessage*						doMessage0f(int iID, float iDelay, float iDuration=0, int iInterp=OF_LINEAR);
	ofxMessage*						doMessage1f(int iID, float iDelay, float iDuration, int iInterp, float iVal);
	ofxMessage*						doMessage3f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2);
	ofxMessage*						doMessage4f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2, float iVal3);
	ofxMessage*						doMessageNf(int iID, float iDelay, float iDuration, int iInterp, int iPath, std::vector<ci::Vec4f> iPathPoints);
	void							stopMessages(int iMessageType=-1);

	static void						Mul(float *source1, float *source2, float *dest);
	static void						LoadIdentity(float *dest);
	static void						Transpose(float *source, float *dest);



protected:
	std::vector <ofxObjectRef>			children;
	std::vector <ofxObject*>				parents;
	std::vector <ofxObjectRef>      children_to_remove;

	bool							shown;
	bool							renderDirty;
	bool							matrixDirty;
	bool							localMatrixDirty;
	bool							isSortedObject;

	GLuint							displayList;
    bool                            displayListFlag;    //eg 070112

	ci::Matrix44f			localMatrix;
	ci::Matrix44f			matrix;
	//ofxObjectMaterial				*material,
		//							*drawMaterial;

	static int						numObjects;
	int								id;

	bool							isLit;
	static bool						prevLit;

	ci::Vec3f							xyzRot,
									xyz,
									scale;
	//ci::Vec4f						color;	//v4.0
	bool							hasSpecialTransparency;
	bool							renderOntop;

	float							timeElapsed,
									timePrev;

	std::vector<ofxMessage *>			messages;


public:
	float							sortedObjectsWindowZ;
	static bool						alwaysMatrixDirty;

	ofxObjectMaterial				*material,
									*drawMaterial;
    bool                            inheritColor;

	static float					curTime;

};

