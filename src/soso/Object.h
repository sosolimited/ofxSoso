/*
Object

Base object class. All onscreen objects in the scene inherit from this, usually by defining their own render() method.
Each object has a translation, rotation, scale, color, and shown/hidden flag. Child objects can be added
to an object with addChild(). All the characteristics of an Object can be animated using the doMessage() methods.
By default, children inherit the transparency of their parents, but this can be disabled by enableAlphaInheritance(false).

When an object is added to the render tree of an Scene, it's render() and idle(float iTime) methods are called once per frame.
See VideoPlayerObject or RectangleObject for examples of extending Object to make your own objects.

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

#include "soso/Message.h"

#include "cinder/Vector.h"

namespace soso
{

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


class ObjectMaterial{

public:
	ObjectMaterial();
	~ObjectMaterial();

public:
	ci::ColorA8u			color;
	bool							inheritAlphaFlag;	//firebrand
};

// using is equivalent to a typedef here, but maybe easier to read (C++11, DW)
using ObjectRef = std::shared_ptr<class Object>;

class Object {

	public :

	Object();
	virtual ~Object();

	int								addChild(ObjectRef child);
	void 							removeChild(ObjectRef child) { removeChild( child.get() ); }
	void							removeChild(Object *child);
	//! variant of removeChild that is safe to call within idle()
	void              removeChildSafe(ObjectRef child);
	int 							isDescendant(ObjectRef iObject);

	virtual void					predraw();
	virtual void					postdraw();
	void							draw(ObjectMaterial *iMaterial, const ci::mat4 &iMatrix, int iSelect=OF_RENDER_ALL, bool iDrawAlone=false);		//v4.0 added material arg
	virtual void					render();
	void							idleBase(float iTime);
	virtual void					idle(float iTime){};

	void							setTrans(ci::vec3 vec);
	void							setTrans(float x, float y, float z);
	ci::vec3         getTrans();

	void							setRot(ci::vec3 r);
	void							setRot(float x, float y, float z);
	ci::vec3                         getRot();
	float							getAlpha();

	virtual ci::vec3   				getScale();			//v4.0 virtual for text object
	virtual void     				setScale(float s);	//v4.0 virtual for text object
	virtual void     				setScale(float x, float y, float z);
	virtual void     				setScale(ci::vec3 vec);

	virtual void					setColor(float r, float g, float b, float a=255.0);
	DEPRECATED_ATTRIBUTE // use setColor( Color ) instead
	virtual void					setColor(ci::vec4 c);	//v4.0 virtual for text object
	virtual void          setColor(const ci::ColorA8u &c);
	ci::ColorA8u					getColor();
	virtual void					setAlpha(float iA);		//v4.0 virtual for text object
	bool							hasTransparency();
	void							setSpecialTransparency(bool iFlag);

	const ci::mat4&		getMatrix();
	const ci::mat4&		getLocalMatrix();
	virtual const ci::mat4&	updateMatrix( const ci::mat4 &iParentMatrix );
	void							updateMatrices( const ci::mat4 &iParentMatrix = ci::mat4(1) );
	void							updateLocalMatrix();

    GLuint                          getDisplayList(){ return displayList; } //eg 070112
    void                            setDisplayList(GLuint iList);    //eg 070112

	ObjectMaterial*				updateMaterial(ObjectMaterial *iMat);
	void							enableAlphaInheritance(bool iEnable);	//firebrand

	int								collectNodes(int iSelect, Object *iNodes[], int iNumber, int iMax);
	ci::vec3							getWindowCoords();

	void   							hide();
	void   							show();
	bool   							isShown();
	int								getID();
	bool							isAnimating();
	void							setLighting(bool iOnOff);


	//void							cleanupMessages();	//replaced by deleteMessage below
	void							updateMessages();
	void							deleteMessage(Message *iMessage);
	bool							hasMessage(Message *iMessage);
	bool							removeMessage(Message *iMessage);

	Message*						doMessage(Message *iMessage);
	Message*						doMessage0f(int iID, float iDelay, float iDuration=0, int iInterp=OF_LINEAR);
	Message*						doMessage1f(int iID, float iDelay, float iDuration, int iInterp, float iVal);
	Message*						doMessage3f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2);
	Message*						doMessage4f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2, float iVal3);
	Message*						doMessageNf(int iID, float iDelay, float iDuration, int iInterp, int iPath, std::vector<ci::vec4> iPathPoints);
	void							stopMessages(int iMessageType=-1);

	static void						Mul(float *source1, float *source2, float *dest);
	static void						LoadIdentity(float *dest);
	static void						Transpose(float *source, float *dest);



protected:
	std::vector <ObjectRef>			children;
	std::vector <Object*>				parents;
	std::vector <ObjectRef>      children_to_remove;

	bool							shown;
	bool							renderDirty;
	bool							matrixDirty;
	bool							localMatrixDirty;
	bool							isSortedObject;

	GLuint							displayList;
    bool                            displayListFlag;    //eg 070112

	ci::mat4			localMatrix;
	ci::mat4			matrix;
	//ObjectMaterial				*material,
		//							*drawMaterial;

	static int						numObjects;
	int								id;

	bool							isLit;
	static bool						prevLit;

	ci::vec3							xyzRot,
									xyz,
									scale;
	//ci::vec4						color;	//v4.0
	bool							hasSpecialTransparency;
	bool							renderOntop;

	float							timeElapsed,
									timePrev;

	std::vector<Message *>			messages;


public:
	float							sortedObjectsWindowZ;
	static bool						alwaysMatrixDirty;

	ObjectMaterial				*material,
									*drawMaterial;
    bool                            inheritColor;

	static float					curTime;

};

} // namespace soso
