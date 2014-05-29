#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
  
  // Setup basic Soso world
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10,10,10);
  
  // Helper method to build instruction text
  buildInstructionText();
  
  //_________________________________________________________________________________________________________________
  
  // Make two scroller objects with an initial height of 2000
  horizontalScroller = new ofxScroller(2000);
  verticalScroller = new ofxScroller(2000);
  
  // Add the scrollers as a child
  scene->getRoot()->addChild(horizontalScroller);
  scene->getRoot()->addChild(verticalScroller);
  
  
  // In this example, we will use the scrollers to control images.
  // You can use ofxScroller to map any ofxSoso transforms
  // to any ofxSoso object
  
  // Make some stars and position them
  int numStars = 10;
  float yTrans = -300;
  
  
  for (int i= 0; i < numStars; i++){
    
    ofxImageObject *star = new ofxImageObject("star.png");
    star->setCentered(true);
    star->setColor(255,255,0);
    star->setTrans(0, yTrans, 0);
    star->setSpecialTransparency(true);
    
    yTrans+=70;
    stars.push_back(star);
  }
  
  // Now we'll
  
  
  // Format the scroll transforms
  
  float offset = 5;
  float startScroll = 0;
  float scrollDistance = 40;
  int i = 0;
  
  horizontalScroller->addSnapPoint(startScroll);
  verticalScroller->addSnapPoint(startScroll);
  
  //Build scroll transforms
  for (auto star : stars){
    
    verticalScroller->addObject(star);
    
    if (i%2==0)
    verticalScroller->getScrollObject(star)->addTransform(OF_TRANSLATE, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(-450,OF_RELATIVE_VAL, OF_RELATIVE_VAL), ofVec3f(0,OF_RELATIVE_VAL,OF_RELATIVE_VAL));
    else
          verticalScroller->getScrollObject(star)->addTransform(OF_TRANSLATE, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(450,OF_RELATIVE_VAL, OF_RELATIVE_VAL), ofVec3f(0,OF_RELATIVE_VAL,OF_RELATIVE_VAL));
    
    
    //make vertical
    horizontalScroller->addObject(star);
    horizontalScroller->getScrollObject(star)->addTransform(OF_SETCOLOR, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(255,255,0), ofVec3f(255,100,255));
    
    horizontalScroller->getScrollObject(star)->addTransform(OF_ROTATE, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(0,0,0), ofVec3f(0,360,0));
    
    horizontalScroller->addSnapPoint(startScroll + offset*i + scrollDistance);
    verticalScroller->addSnapPoint(startScroll + offset*i + scrollDistance);
    
    //set the height of scroller
    horizontalScroller->setScrollHeight(startScroll + offset*i + scrollDistance+10);
    verticalScroller->setScrollHeight(startScroll + offset*i + scrollDistance+10);
    
    startScroll+= scrollDistance;
    
    i++;
    
  }
  




}


//----Convenience class to build instruction text
void testApp::buildInstructionText(){
  
  //Load a font.
	//ofxSosoTrueTypeFont inherits from ofTrueTypeFont and adds some nice functionality, used by ofxTextObject.
	//Note: If the font has them, kerning pairs are loaded by default.
  font16 = new ofxSosoTrueTypeFont();
  font16->loadFont("Arial.ttf", 24.0f, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
  
  string instructionText[4] = { "Drag mouse horizontally to zip in stars.",
    "Drag mouse vertically to animate stars.",
    "Press 's' to snap stars to center.",
    "Press 'a' to snap stars to sides"};
  
  textBackground = new ofxRectangleObject(400,300);
  textBackground->setColor(200, 200, 200,240);
  textBackground->setTrans(-530,-450,1);
  scene->getRoot()->addChild(textBackground);
  
  float yPos = 100;
  for (int i=0; i < 4; i++){
    
    char *s = &instructionText[i][0]; //convert to char*
    ofxTextObject *instruction = new ofxTextObject(font16, s);
    instruction->setColumnWidth(350);
    instruction->setTrans(50,yPos,2);
    instruction->setColor(10,10,10);
    instruction->setPointSize(20.0f);
    
    instructions.push_back(instruction);
    textBackground->addChild(instruction);
    
    yPos+=50;
    
    
  }
}

//

//--------------------------------------------------------------
void testApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
  
  time+=ofGetElapsedTimef();
  //horizontalScroller->update(time);
  verticalScroller->update(time);
}

//--------------------------------------------------------------
void testApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

  if (key=='a'){
    
    verticalScroller->gotoSnapPoint(0, 1500);
    
  }else if (key == 's'){
    
    verticalScroller->gotoSnapPoint(10, 1500);
    
  }else if (key == 'z'){
    
    verticalScroller->gotoSnapPoint(0, 1000);
    
  }else if (key == 'x'){
    
    verticalScroller->gotoSnapPoint(10, 1000);
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
  
  //update scroller.
  
  float fMouseXDelta = x - prevMouseX;
  float fMouseYDelta = prevMouseY - y; //flip it
  
  if (fMouseXDelta < 0)
    horizontalDragDirection = OF_SCROLL_BACKWARD;
  else horizontalDragDirection = OF_SCROLL_FORWARD;
  
  if (fMouseYDelta < 0)
    verticalDragDirection = OF_SCROLL_BACKWARD;
  else verticalDragDirection = OF_SCROLL_FORWARD;

  if (abs(fMouseXDelta) > abs(fMouseYDelta)){
  //horizontalScroller->moveScroll(fMouseXDelta);
  }else{
  verticalScroller->moveScroll(fMouseYDelta);
  }
  
  prevMouseX = x;
  prevMouseY = y;

  
  
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
 
  prevMouseX = x;
  prevMouseY = y;
  
}

//------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  
  horizontalScroller->gotoNextSnapPoint(horizontalDragDirection);
  verticalScroller->gotoNextSnapPoint(verticalDragDirection);
  
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


