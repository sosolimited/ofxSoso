#include "iostream.h"
#include "emptyApp.h"


//--------------------------------------------------------------
void emptyApp::setup(){
  ofSetVerticalSync(true);
  
    // Disable the of setupScreen because now each scene has a custom renderer.
    ofDisableSetupScreen();
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
  scene->setBackgroundColor(80, 80, 80, 0);
    
//    font16 = new ofxSosoTrueTypeFont();
//    font16->loadFont("Arial.ttf", 16, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
//	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
//    
//    readOut = new ofxTextObject(font16, "Your R,G,B and X,Y,Z read out.");
//    readOut->setColor(255, 255, 255);										//Set the color of the text.
//    readOut->setPointSize(16);												//Set the point size of the text, independent of the font resolution.
//    readOut->setLeading(19);												//Set the leading (line-spacing) of the text.
//    //text->setScale(0.5);												//Text can be scaled like any other object. So the final scale is the pointSize x scale.
//    readOut->setColumnWidth(360);											//Set the column width, which determines how the words wrap.
//	readOut->setTrans(-300,300,0);			//Position the text, relative to the image object above.
//	scene->getRoot()->addChild(readOut);
    
    
    
    //Create Circle Object with Container
    circle = new ofxCircleObject(100,50);
    
    container = new ofxObject();
    
    container->addChild(circle);
    //container->setTrans(100,-200,0);
    
    scene->getRoot()->addChild(container);
    
    controls = new ofxGuiObject();
    //controls->setTrans(0, 0, -3);
    scene->getRoot()->addChild(controls);
    
    
    
    
  
}

//--------------------------------------------------------------
void ofxGuiObject::circleResolutionChanged(int & circleResolution){
	ofSetCircleResolution(circleResolution);
}


//--------------------------------------------------------------
void emptyApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
    //string text = "Your values" + ofToString(container->getX());
   
    ofSetCircleResolution(controls->circleResolution);

    
    circle->outerRadius = controls->radius*0.5;
    circle->setTrans(controls->center->x-ofGetWindowWidth()/2, (controls->center->y*-1)+ofGetWindowHeight()/2, 0);
    
}


//--------------------------------------------------------------
void emptyApp::draw(){
    
    scene->draw();
    
    ofPushMatrix();
    ofScale(1,-1,-1);
    ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
    
    ofBackgroundGradient(ofColor::white, ofColor::gray);
	//Call draw on scene, which initiates the drawing of the root object.

    
    if( controls->filled ){
		ofFill();
	}else{
		ofNoFill();
	}
    
	ofSetColor(controls->color);
	if(controls->twoCircles){
		ofCircle(controls->center->x-controls->radius*.5, controls->center->y, controls->radius );
		ofCircle(controls->center->x+controls->radius*.5, controls->center->y, controls->radius );
	}else{
		ofCircle((ofVec2f)controls->center, controls->radius );
    }
	
	// auto draw?
	// should the gui control hiding?
	if( controls->bHide ){


		controls->gui.draw();

	}
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void emptyApp::keyPressed  (int key){
    if( key == 'h' ){
		controls->bHide = !controls->bHide;
	}
	if(key == 's') {
		controls->gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		controls->gui.loadFromFile("settings.xml");
	}
	if(key == ' '){
		controls->color = ofColor(255);
	}

}

//--------------------------------------------------------------
void emptyApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void emptyApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void emptyApp::mouseDragged(int x, int y, int button){
  
  
}

//--------------------------------------------------------------
void emptyApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void emptyApp::mouseReleased(int x, int y, int button){
  
  
}


//--------------------------------------------------------------
void emptyApp::windowResized(int w, int h){
  controls->screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void emptyApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void emptyApp::dragEvent(ofDragInfo dragInfo){ 
  
}


