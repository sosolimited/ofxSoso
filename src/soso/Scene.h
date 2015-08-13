/*
Scene

Scene is a scene graph. It has a root object, accessed by getRoot(), to which objects are added.
In the standard configuration, a rectangle placed at z=0 of size ofGetWidth() x ofGetHeight() will exactly fill the window.
By default, the root is at the center of the screen and up is up and down is down.
Rather than using glPushMatrix() and glPopMatrix() calls, all matrix transformations are calculated in code
and glLoadMatrix() is used when each object is drawn.

The scene graph has three different render modes. Call setRenderMode() with:
RENDER_NORMAL - No fancy business. Just recurses through the render tree.
RENDER_ALPHA_DEPTH - Draws opaqaue objects. Disables depth-mask. Then draws transparent objects.
RENDER_ALPHA_DEPTH - Same as RENDER_ALPHA_DEPTH, but in addition sorts all transparent objects by their z position.

The owner of an Scene should call its update() and draw() methods once per frame. The update call propagates
calls to the idle() method of every object in the tree. The draw() call propagates calls to the render() method
of every object in the tree, among other things.

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
#include "cinder/Camera.h"

namespace soso
{

enum{
	RENDER_NORMAL,
	RENDER_ALPHA_DEPTH,
	RENDER_ALPHA_DEPTH_SORTED
};

class ViewportData
{
public:
	ViewportData(int iScreenNum, int iScreenX, int iScreenY, int iWorldX, int iWorldY, int iWidth, int iHeight);
	~ViewportData();

public:
	int						screenNum;
	int						screenOrigin[2];
	int						worldOrigin[2];
	int						dimensions[2];



};

class Scene
{
public:

	Scene(int w, int h);
	~Scene();

	void					update( float iTime );
	ObjectRef	getRoot();
	void					setRoot(ObjectRef iRoot);

	void					draw();
	void					drawNormal();
	void					drawAlphaDepth();
	void					drawAlphaDepthSorted();

	void					setClearBackground(bool iClear); //LM 061312
	void					setBackgroundColor(float iR, float iG, float iB, float iA=255);
	void					setCenterOffset(float iX, float iY, float iZ);
	void					setRenderMode(int iMode);
	void					setMaxSortedObjects(int iMax);

	void					enableScissor(bool iEnable);
	void					setScissorParameters(int iX, int iY, int iW, int iH);
	void					enableDepthTest(bool iEnable);
	void					setDepthFunc(GLenum iFunc);
	void					setDepthMask(bool iWrite);
	void					enableBlending(bool iEnable);

	int						getSceneWidth();
	int						getSceneHeight();
	ci::vec4					getBackgroundColor(); //LM 061312


	static int				depthCompareFunc(const void *iElement1, const void *iElement2);

private:
	int						sceneWidth;
	int						sceneHeight;
	ObjectRef	root;
	ci::vec4					backgroundColor;
	bool					isClearBackgroundOn;

	Object				**sortedObjects;
	int						maxSortedObjects;

	ci::vec3					centerOffset;

	int						renderMode;

	bool					isScissorOn;
	int						scissorOrigin[2];
	int						scissorDimensions[2];
	bool					isDepthTestOn,
							isDepthMaskOn;
	GLenum					depthFunc;
	bool					isBlendingOn;
	ci::CameraPersp	camera;

public:
	static ci::mat4				defaultMatrix;
	static ObjectMaterial *defaultMaterial;


};

} // namespace soso
