#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
  
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
  
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
  
	font48 = new ofxSosoTrueTypeFont();
  font48->loadFont("National-Regular.otf", 48, true,  true, false, true);
  
  //memory test for lots of fonts
  for (int i=0; i<20; i++)
  {
    ofxSosoTrueTypeFont *f = new ofxSosoTrueTypeFont();
    //f->loadFont("National-Regular.otf", i*12+12, true, true);
    f->loadFont("National-Regular.otf", 96, true, true);
    memFonts.push_back(f);
  }
	
  
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
  unicodeText = new ofxTextObject(font24, "This block of \u2018text\u2019 demonstrates the use of extended characters in ofxTextObject\u2122 \u2014 initialized with \u00A9Unicode escape sequences.");
  unicodeText->setColor(240, 240, 240);
  unicodeText->setPointSize(24);
  unicodeText->setLeading(28);
  unicodeText->setSpaceWidth(0.25);
  unicodeText->setColumnWidth(ofGetWidth() - 1.0*unicodeText->getLeading());
  unicodeText->setTrans(trans + ofVec3f(0, -unicodeText->getLeading()*3.0, 0));
	scene->getRoot()->addChild(unicodeText);
  
  inlineText = new ofxTextObject(font24, "This block of ‘text’ demonstrates the use of extended characters in ofxTextObject™ — initialized with ©Inline escape sequences.");
  inlineText->setColor(240, 240, 240);
  inlineText->setPointSize(24);
  inlineText->setLeading(28);
  inlineText->setSpaceWidth(0.25);
  inlineText->setColumnWidth(ofGetWidth() - 1.0*inlineText->getLeading());
  inlineText->setTrans(trans + ofVec3f(0, -inlineText->getLeading()*6.0, 0));
	scene->getRoot()->addChild(inlineText);
  
  //Testing the character encodings with C
//  string s("This block of ‘text’ demonstrates the use of extended characters in ofxTextObject™ — initialized with ©Inline escape sequences.");
//  for (int i=0; i<s.length(); i++)
//  {
//    cout << s[i] << " ";
//  }
//  cout << endl;
  
//  //testing memory footprint
//  for (int i=0; i<10; i++)
//  {
//    string s;
//    for (int i=0; i<150; i++)
//    {
//      s += (unsigned char)ofRandom(33, 240);
//    }
//    int font_index = (int) ofRandom(memFonts.size());
//    ofxTextObject *t = new ofxTextObject(memFonts[font_index], s);
//    t->setColor(240, 240, 240);
//    t->setPointSize(12);
//    t->setTrans(trans + ofVec3f(0, -unicodeText->getLeading()*9.0 - i*1.0, 0));
//    scene->getRoot()->addChild(t);
//    memTexts.push_back(t);
//  }
  
  //Create a letter text object. This lets you treat each letter as a separate object to animate as you please.
  //See how the letters are animated below in keyPressed()
  //TODO: letterText doesn't have the mapped chars working yet &lsquo; fails
  letterText = new ofxLetterTextObject(font48, "This is an ofxLetterTextObject™ object using extended characters — initialized with ©Inline escape sequences. Press ‘a’ to animate.");
  letterText->setTrans(trans + ofVec3f(0, -unicodeText->getLeading()*10.0, 0));
  letterText->setColor(255, 255, 255);
  letterText->setPointSize(48);
  letterText->setLeading(54);
  letterText->setColumnWidth(ofGetWidth() - 1.0*inlineText->getLeading());
  letterText->setAlignment(OF_TEXT_ALIGN_LEFT);
  scene->getRoot()->addChild(letterText);
  
}

//--------------------------------------------------------------
void testApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
  
  static float current = ofGetElapsedTimef();
  static float past = ofGetElapsedTimef();
  static float timer = 0.0;
  float period = 2.0;
  
  current = ofGetElapsedTimef();
  timer += (current - past);
  past = current;
  
  
  //updating the strings on all the memory text objects
  if (timer > period)
  {
    timer = 0;
    
    //set string for all the text
    for (auto t : memTexts)
    {
      string s;
      for (int i=0; i<150; i++)
      {
        s += (unsigned char)ofRandom(33, 240);
      }
      t->setString(s);
    }
    
  }
  
  
  
}

//--------------------------------------------------------------
void testApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
  
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    //Animate the letter text with messages.
	if(key == 'a'){
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


