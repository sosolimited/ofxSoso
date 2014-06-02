//
//  videoUniverse.h
//  baccaratApp
//
//  Created by Sam Kronick on 5/7/13.
//
//  All scenegraph objects that will be shown on the chandlier "display" should be
//  added as children to this object. Everything else (GUI elements, status indicators,
//  etc) should be added directly to the baccaratApp scene.



#pragma once

#include "ofxObject.h"
//#include "ofxScene.h"
#include "tempScene.h"
#include "ofxRectangleObject.h"
#include "ofxFboObject.h"
#include "ofxImageObject.h"

#include "ofMain.h"


class videoUniverse : public ofxObject {
  public:
    videoUniverse(int width, int height);
    ~videoUniverse();
    
    void idle(float time);
    void drawFBOScene();
    ofxFboObject* getFbo();
    
    void addToRoot(ofxObject *o) { root->addChild(o); }
    void removeFromRoot(ofxObject *o) { root->removeChild(o); }
    
    int getHeight() { return height; }
    int getWidth()  { return width; }
    
    ofxObject* getRoot() { return root; }
    
  private:
    int                 width,height;
    ofxFboObject*       fbo;
//    ofxScene*           scene;
    tempScene*          scene;
    ofxObject*          root;
    
    int screenFov;
    
    vector<tempViewportData*>    viewports;
    
    tempViewportData    *addViewport(int iScreenNum, int iScreenX, int iScreenY, int iWorldX, int iWorldY, int iWidth, int iHeight);
    void                setupViewport(tempViewportData *iView);
    void                setupMultiScreen(int iScreenIndex);
};