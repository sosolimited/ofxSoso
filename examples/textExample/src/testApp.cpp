#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
  
  //Replace the default ofGLRenderer with ofxSosoRenderer which has overriden setupScreen() and setupScreenPerspective().
    //This lets us set up the scene graph how we want to.
	//Warning: Up is up and down is down in this world.
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10, 10, 10);
	
	//_________________________________________________________________________________________________________________
  
	//Load a font.
	//ofxSosoTrueTypeFont inherits from ofTrueTypeFont and adds some nice functionality, used by ofxTextObject.
	//Note: If the font has them, kerning pairs are loaded by default.
  font16 = new ofxSosoTrueTypeFont();
  font16->loadFont("Arial.ttf", 16, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
  
	font64 = new ofxSosoTrueTypeFont();
  font64->loadFont("Arial.ttf", 64, true,  true, false, true);
	
  
	//Create a text object.
  text = new ofxTextObject(font16, "An OpenFrameworks Addon by Sosolimited. Tyson. é"
                           );
  text->setColor(255, 255, 255);                  //Set the color of the text.
  text->setPointSize(16);                         //Set the point size of the text, independent of the font resolution.
  text->setLeading(19);                           //Set the leading (line-spacing) of the text.
  //text->setScale(0.5);                          //Text can be scaled like any other object. So the final scale is the pointSize x scale.
  text->setColumnWidth(360);											//Set the column width, which determines how the words wrap.
	text->setTrans(-200,-200,0);			//Position the text, relative to the image object above.
	scene->getRoot()->addChild(text);								//Add the text object to the scene.
  
	//Create a letter text object. This lets you treat each letter as a separate object to animate as you please.
  //See how the letters are animated below in keyPressed()
  letterText = new ofxLetterTextObject(font64, "This is a letter text object. Press &times;&times;&times; &amp; g to animate the letters. &frac12; ");
  letterText->setTrans(-350, 200,0);
  letterText->setColor(255, 255, 255);
  letterText->setPointSize(48);
  letterText->setLeading(52);
  letterText->setColumnWidth(700);
  letterText->setAlignment(OF_TEXT_ALIGN_LEFT);
  scene->getRoot()->addChild(letterText);
    
	//Create a label for the polygon.
	polygonLabel = new ofxTextObject(font16, "This body of &lsquo;text&rsquo; will demonstrate the column width feature of ofxTextObject by automatically wrapping if the line gets too long. ");
	polygonLabel->setColor(255, 255, 255);
	polygonLabel->setPointSize(16);
	polygonLabel->setLeading(19);
	polygonLabel->setColumnWidth(150);
	polygonLabel->setTrans(ofVec3f(0, polygonLabel->getLeading() + 5, -1.0));	//Position the text relative to the position of the polygon created above.
	scene->getRoot()->addChild(polygonLabel);
}

//--------------------------------------------------------------
void testApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void testApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    //Animate the letter text with messages.
	if(key == 'g'){
		float travel = 100;
		float leaveDuration = 1.0;
		float returnDuration = 0.5;
      
		for(int i=0; i < letterText->letters.size(); i++){
			ofVec3f letterHome = letterText->letters[i]->home;	//Grab letter home position.
			
			//Stop any previous messages.
			letterText->letters[i]->stopMessages();
            
			//Leave home.
			letterText->letters[i]->doMessage3f(OF_TRANSLATE, i*0.03, leaveDuration, OF_EASE_OUT, letterHome.x + ofRandom(-travel, travel), letterHome.y + ofRandom(-travel, travel), 0);
			letterText->letters[i]->doMessage1f(OF_SCALE, i*0.03, leaveDuration, OF_EASE_OUT, ofRandom(0.5, 1.8));
			letterText->letters[i]->doMessage3f(OF_ROTATE, i*0.03, leaveDuration, OF_EASE_OUT, ofRandom(-30,30), ofRandom(-30,30), 0);
			letterText->letters[i]->doMessage3f(OF_SETCOLOR, i*0.03, leaveDuration, OF_EASE_OUT, 255, 100, 200);
            
			//Return home.
			letterText->letters[i]->doMessage3f(OF_TRANSLATE, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, letterHome.x, letterHome.y, letterHome.z);
			letterText->letters[i]->doMessage1f(OF_SCALE, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, 1.0);
			letterText->letters[i]->doMessage3f(OF_ROTATE, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, 0, 0, 0);
			letterText->letters[i]->doMessage3f(OF_SETCOLOR, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, 255, 255, 255);
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  
  
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  
  
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
  
}


