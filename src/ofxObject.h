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

#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofMath.h"
#include "ofxMessage.h"
#include "ofShader.h"
//#include "ofxVectorMath.h" //OF7

enum{
	OF_RENDER_ALL = 0x00,
	OF_RENDER_OPAQUE = 0x01,
	OF_RENDER_TRANSPARENT = 0x02,
	OF_RENDER_ONTOP = 0x03
};

class ofxObjectMaterial{
  
public:
	ofxObjectMaterial();
	~ofxObjectMaterial();
  ofVec4f getColorVec4f(); //AO: for using ofvec4f with messages

public:

  ofColor           color;
	bool							inheritAlphaFlag;	//firebrand

};



class ofxObject{
  
  
  friend class ofxScene; //Baccarat
  
public :
  
	ofxObject();
	virtual ~ofxObject();
  
	int								addChild(ofxObject *child);
	void 							removeChild(ofxObject *child);
  //! variant of removeChild that is safe to call within idle()
  void              removeChildSafe(ofxObject *child);
	int 							isDescendant(ofxObject *iObject);

	virtual void			predraw();
	virtual void			postdraw();
	void							draw(ofxObjectMaterial *iMaterial, float *iMatrix, int iSelect=OF_RENDER_ALL, bool iDrawAlone=false);		//v4.0 added material arg
	virtual void			render();
	
  void							idleBase(float iTime);
	virtual void			idle(float iTime){};
	
  void   						hide();
	void   						show();
	bool   						isShown();
  
  void							setTrans(ofVec3f vec);
	void							setTrans(float x, float y, float z);
	ofVec3f           getTrans();
  
  void            setX(float iX);
  void            setY(float iY);
  void            setZ(float iZ);
  
  float           getX();
  float           getY();
  float           getZ();
	
  void							setRot(ofVec3f r);
	void							setRot(float x, float y, float z);
	ofVec3f           getRot();
	
	virtual void     	setScale(float s);
	virtual void     	setScale(float x, float y, float z);
	virtual void     	setScale(ofVec3f vec);
  virtual ofVec3f   getScale();
  
 	virtual void			setAlpha(float iA);
  float             getAlpha();

	virtual void			setColor(float r, float g, float b, float a);
  virtual void      setColor(float r, float g, float b); // Applies object's current alpha value
	virtual void			setColor(ofVec4f c);
  virtual void      setColor(ofColor c);
  virtual void      setColor(string iHex);
	ofVec4f						getColorVec4f();
  ofColor           getColor();
    
  void              setRed(float iRed);
  void              setGreen(float iGreen);
  void              setBlue(float iBlue);
  
  float             getRed();
  float             getGreen();
  float             getBlue();

	void							setSpecialTransparency(bool iFlag);
	bool							hasTransparency();
  void              setRenderOnTop(bool iSet) { renderOntop = iSet; }
  
	float*						getMatrix();
	float*						getLocalMatrix();
	virtual float*		updateMatrix(float *iParentMatrix);
	void							updateMatrices(float *iParentMatrix = NULL);
	void							updateLocalMatrix();

  ofxObjectMaterial*				updateMaterial(ofxObjectMaterial *iMat);
  
  GLuint            getDisplayList(){ return displayList; }
  void              setDisplayList(GLuint iList);
	
 	void							enableAlphaInheritance(bool iEnable);
	int								collectNodes(int iSelect, ofxObject *iNodes[], int iNumber, int iMax);
	ofVec3f						getWindowCoords();

  void							setLighting(bool iOnOff);
  
  // shaders
  void  setShader(ofShader *iShader);
  void  loadShader(string iShaderName);
  void  loadShader(string iFragName, string iVertName);
  virtual void  setShaderParams();
  void  setEnableShaders(bool iSet);
  ofShader* getShader(){ return shader; }
  
	int								getID();
  bool              isObjectID(vector<GLuint> iIDs);

  bool							isAnimating();
	void							updateMessages();
	void							deleteMessage(ofxMessage *iMessage);
	bool							hasMessage(ofxMessage *iMessage);
	bool							removeMessage(ofxMessage *iMessage);
  
	ofxMessage*				doMessage(ofxMessage *iMessage);
	ofxMessage*				doMessage0f(int iID, float iDelay, float iDuration=0, int iInterp=OF_LINEAR);
	ofxMessage*				doMessage1f(int iID, float iDelay, float iDuration, int iInterp, float iVal);
	ofxMessage*				doMessage3f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2);
	ofxMessage*				doMessage4f(int iID, float iDelay, float iDuration, int iInterp, float iVal0, float iVal1, float iVal2, float iVal3);
	ofxMessage*				doMessageNf(int iID, float iDelay, float iDuration, int iInterp, int iPath, vector<ofVec4f> iPathPoints);
	void							stopMessages(int iMessageType=-1);
  
	static void				Mul(float *source1, float *source2, float *dest);
	static void				LoadIdentity(float *dest);
	static void				Transpose(float *source, float *dest);
  
protected:
	bool							shown;
	bool							renderDirty;
	bool							matrixDirty;
	bool							localMatrixDirty;
	bool							isSortedObject;
  
	GLuint						displayList;
  bool              displayListFlag;    //eg 070112
  
	static int				numObjects;
	int								id;
  
	bool							isLit;
	static bool				prevLit;
  
	ofVec3f						xyzRot,
                    xyz,
                    scale;
  
  bool              hasSpecialTransparency;
	bool							renderOntop;

	float							timeElapsed,
                    timePrev;

	float							*localMatrix;
	float							*matrix;
  
  // shader
  ofShader  *shader;
  bool  shaderEnabled;
  

  vector <ofxObject *>			children;
	vector <ofxObject *>			parents;
  vector <ofxObject *>      children_to_remove;
	vector<ofxMessage *>			messages;
  
  void setTime(float iTime);
	
  //ofxObjectMaterial				*material,
  //							*drawMaterial;
  //ofVec4f						color;	//v4.0

public:
	float							sortedObjectsWindowZ;
	static bool				alwaysMatrixDirty;
  bool              inheritColor;
	//static float			curTime;
  
  float curTime = 0;
  
	ofxObjectMaterial				*material,
                          *drawMaterial;
  
};

