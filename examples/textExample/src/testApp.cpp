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
	scene->setBackgroundColor(40, 40, 40);
	
	//_________________________________________________________________________________________________________________
  
	//Load a font.
	//ofxSosoTrueTypeFont inherits from ofTrueTypeFont and adds some nice functionality, used by ofxTextObject.
	//Note: If the font has them, kerning pairs are loaded by default.
  font24 = new ofxSosoTrueTypeFont();
  font24->loadFont("TiemposText-Medium.otf", 24, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
	font24->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
  
	font64 = new ofxSosoTrueTypeFont();
  font64->loadFont("National-Regular.otf", 64, true,  true, false, true);
	
  
	//Create a text object.
  escapeText = new ofxTextObject(font24, "This block of &lsquo;text&rsquo; demonstrates the use of extended characters in ofxTextObject&trade; &mdash; initialized with &copy;HTML escape sequences.");
  escapeText->setColor(240, 240, 240);                  //Set the color of the text.
  escapeText->setPointSize(24);                         //Set the point size of the text, independent of the font resolution.
  escapeText->setLeading(28);                           //Set the leading (line-spacing) of the text.
  escapeText->setSpaceWidth(0.25);
  //text->setScale(0.5);                                //Text can be scaled like any other object. So the final scale is the pointSize x scale.
  escapeText->setColumnWidth(ofGetWidth() - 1.0*escapeText->getLeading());											//Set the column width, which determines how the words wrap.
	ofVec3f trans(-ofGetWidth()/2. + escapeText->getLeading(), ofGetHeight()/2. - escapeText->getLeading()*2.0, 0);
  escapeText->setTrans(trans);                          //Position the text, relative to the image object above.
	scene->getRoot()->addChild(escapeText);								//Add the text object to the scene.
  
  //verifying the results of getWidth() and getHeight()
  ofxRectangleObject *measureRect = new ofxRectangleObject(escapeText->getWidth(), escapeText->getHeight());
  measureRect->setTrans(trans + ofVec3f(0, -escapeText->getHeight() + escapeText->getPointSize(), 0));
  measureRect->setAlpha(50);
  scene->getRoot()->addChild(measureRect);
  
  //checking pointSize
  ofxRectangleObject *pointRect = new ofxRectangleObject(24, 24);
  pointRect->setAlpha(200);
  pointRect->setTrans(trans + ofVec3f(0, 0, 0));
  scene->getRoot()->addChild(pointRect);
  
  //testing Illustrator text for layout reference
  ofxImageObject *guide = new ofxImageObject("Illustrator-Text.png");
  guide->setSpecialTransparency(true);
  guide->setTrans(trans + ofVec3f(0, -escapeText->getLeading()*4.0, 0));
  //scene->getRoot()->addChild(guide);
  
  
  //Unicode and Hex escape sequences - note that HEX is required for chars below 255
  unicodeText = new ofxTextObject(font24, "This block of \u2018text\u2019 demonstrates the use of extended characters in ofxTextObject\u2122 \u2014 initialized with \u00A9Unicode escape sequences. \u0152");
  unicodeText->setColor(240, 240, 240);
  unicodeText->setPointSize(24);
  unicodeText->setLeading(28);
  unicodeText->setSpaceWidth(0.25);
  unicodeText->setColumnWidth(ofGetWidth() - 1.0*unicodeText->getLeading());
  unicodeText->setTrans(trans + ofVec3f(0, -unicodeText->getLeading()*3.0, 0));
	scene->getRoot()->addChild(unicodeText);
  
  string s("This block of ‘text’ demonstrates the use of extended characters in ofxTextObject™ — initialized with ©Inline éêscape sequences.");
  for (int i=0; i<s.length(); i++)
  {
    cout << s[i] << " " << ofToString(ofToHex(s[i])) << endl;
  }
  
  inlineText = new ofxTextObject(font24, "This block of ‘text’ demonstrates the use of extended characters in ofxTextObject™ — initialized with ©Inline éêscape sequences.");
  inlineText->setColor(240, 240, 240);
  inlineText->setPointSize(24);
  inlineText->setLeading(28);
  inlineText->setSpaceWidth(0.25);
  inlineText->setColumnWidth(ofGetWidth() - 1.0*inlineText->getLeading());
  inlineText->setTrans(trans + ofVec3f(0, -inlineText->getLeading()*6.0, 0));
	scene->getRoot()->addChild(inlineText);
  
  
  
  //Create a letter text object. This lets you treat each letter as a separate object to animate as you please.
  //See how the letters are animated below in keyPressed()
  //TODO: letterText doesn't have the mapped chars working yet &lsquo; fails
  letterText = new ofxLetterTextObject(font64, "This is a letter text object. Press &times;&times;&times; &amp; g to animate the letters. &frac12;");
  letterText->setTrans(-350, 200,0);
  letterText->setColor(255, 255, 255);
  letterText->setPointSize(48);
  letterText->setLeading(52);
  letterText->setColumnWidth(700);
  letterText->setAlignment(OF_TEXT_ALIGN_LEFT);
  //scene->getRoot()->addChild(letterText);
  
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


