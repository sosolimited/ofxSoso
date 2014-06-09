//
//  ofxGuiObject.cpp
//  testAppColin
//
//  Created by Colin Wageman on 6/9/14.
//
//

#include "ofxGuiObject.h"

ofxGuiObject::ofxGuiObject()
{
	// we add this listener before setting up so the initial circle resolution is correct
	circleResolution.addListener(this, &ofxGuiObject::circleResolutionChanged);
	
    
	gui.setup(); // most of the time you don't need a name
	gui.add(filled.setup("fill", true));
	gui.add(radius.setup( "radius", 140, 10, 300 ));
	gui.add(center.setup("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
	gui.add(color.setup("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
	gui.add(circleResolution.setup("circle res", 5, 3, 90));
	gui.add(twoCircles.setup("two circles"));
    
	gui.add(screenSize.setup("screen size", ""));
    
	bHide = true;
    
	
}



ofxGuiObject::~ofxGuiObject(){
    
}