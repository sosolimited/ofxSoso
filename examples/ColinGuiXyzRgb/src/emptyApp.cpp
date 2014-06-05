#include "iostream.h"
#include "emptyApp.h"


//--------------------------------------------------------------
void emptyApp::setup(){
  ofSetVerticalSync(true);
  //Replace the default ofGLRenderer with ofxSosoRenderer which has overriden setupScreen() and setupScreenPerspective().
	//This lets us set up the scene graph how we want to.
	//Warning: Up is up and down is down in this world.
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth()/2, ofGetHeight()/2);
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
    container->setTrans(100,-200,0);
    
    scene->getRoot()->addChild(container);
    
    
    // we add this listener before setting up so the initial circle resolution is correct
	circleResolution.addListener(this, &emptyApp::circleResolutionChanged);
	
    fbo = new ofFbo();
    fbo->allocate(ofGetWidth()/2,ofGetHeight()/2);
    
    //--------------------------------------------------------------
    
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

//--------------------------------------------------------------
void emptyApp::circleResolutionChanged(int & circleResolution){
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void emptyApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
    //string text = "Your values" + ofToString(container->getX());
   ofSetCircleResolution(circleResolution);

    
}


//--------------------------------------------------------------
void emptyApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
	//Call draw on scene, which initiates the drawing of the root object.

    
    if( filled ){
		ofFill();
	}else{
		ofNoFill();
	}
    
	ofSetColor(color);
	if(twoCircles){
		ofCircle(center->x-radius*.5, center->y, radius );
		ofCircle(center->x+radius*.5, center->y, radius );
	}else{
		ofCircle((ofVec2f)center, radius );
    }
	
	// auto draw?
	// should the gui control hiding?
	if( bHide ){
        ofPushMatrix();
        
        ofScale(1,-1,-1);
        ofTranslate(-ofGetWindowWidth()/2, -ofGetWindowHeight()/2);
		gui.draw();
        ofPopMatrix();
	}
    fbo->begin();
    scene->draw();
    fbo->end();
    
    fbo->draw(200,200);
}

//--------------------------------------------------------------
void emptyApp::keyPressed  (int key){
    if( key == 'h' ){
		bHide = !bHide;
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
	if(key == ' '){
		color = ofColor(255);
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
  screenSize = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void emptyApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void emptyApp::dragEvent(ofDragInfo dragInfo){ 
  
}


