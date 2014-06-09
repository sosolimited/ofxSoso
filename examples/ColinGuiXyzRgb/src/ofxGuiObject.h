//
//  ofxGuiObject.h
//  testAppColin
//
//  Created by Colin Wageman on 6/9/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxObject.h"
#include "ofxGui.h"

class ofxGuiObject : public ofxObject{
    
public:
    ofxGuiObject();
	~ofxGuiObject();
	    
	//void						render();
    
public:
    
    //GUI Assets
    void circleResolutionChanged(int & circleResolution);
    
	bool bHide;
    
	ofxFloatSlider radius;
	ofxColorSlider color;
	ofxVec2Slider center;
	ofxIntSlider circleResolution;
	ofxToggle filled;
	ofxButton twoCircles;
	ofxLabel screenSize;
    
	ofxPanel gui;
	
    
};