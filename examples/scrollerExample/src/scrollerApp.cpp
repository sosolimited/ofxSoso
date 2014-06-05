#include "scrollerApp.h"

// ofxScroller example
// Created by Alex Olivier
// May 29, 2014

//--------------------------------------------------------------
void scrollerApp::setup(){
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
  
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10,10,10);
  
  // Helper method to build instruction text
  buildInstructionText();
  
  //_________________________________________________________________________________________________________________
  
  // Make two scroller objects with an initial height of 2000
  horizontalScroller = new ofxScroller(2000);
  verticalScroller = new ofxScroller(2000);
  
  // Add the scrollers as a child of the scene
  scene->getRoot()->addChild(horizontalScroller);
  scene->getRoot()->addChild(verticalScroller);
  
  
  // In this example, we will use the ofxScroller objects to control images.
  // We will have two separate ofxScroller objects to control different
  // types of transforms.
  
  // We will update one scroller using the mouse's y-value when dragged
  // We will update the other scroller using the mouse's x-value when dragged
  
  // You can use ofxScroller to map any ofxSoso transforms
  // to any ofxSoso object
  
  // First, we will make some image objects and position them
  int numStars = 10;
  float yTrans = -300;
  
  // Let's make 10 stars
  for (int i= 0; i < numStars; i++){
    
    ofxImageObject *star = new ofxImageObject("star.png");
    star->setCentered(true);
    star->setColor(255,255,0);
    star->setTrans(0, yTrans, 0);
    star->setSpecialTransparency(true);
    
    yTrans+=70;
    stars.push_back(star);
  }
  
  
  // USING THE SCROLLERS
  
  // Scroller vars
  float offset = 5; // A scroller offset to offset our transforms
  float startScroll = 0; // The initial scroll value
  float scrollDistance = 40;  // The distance we have to scroll to finish a transform

  // Snap points allow us to jump to a specific scroller position
  // It's a good idea to add a snap point at the beginning scroll value
  horizontalScroller->addSnapPoint(startScroll);
  verticalScroller->addSnapPoint(startScroll);
  
  int i = 0;
  
  // For each star image, we will add different scroll transforms
  for (auto star : stars){
    
    
    // VERTICAL SCROLLER
    // We first need to add the object to our scroller
    // By default, the object is added as a child of the scroller
    verticalScroller->addObject(star);
    
    // Every other star will come in from a different direction
    if (i%2==0)
    // Star comes in from left to center
    verticalScroller->getScrollObject(star)->addTransform(OF_TRANSLATE, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(-450,OF_RELATIVE_VAL, OF_RELATIVE_VAL), ofVec3f(0,OF_RELATIVE_VAL,OF_RELATIVE_VAL));
    else
          // Star comes in from right to center
          verticalScroller->getScrollObject(star)->addTransform(OF_TRANSLATE, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(450,OF_RELATIVE_VAL, OF_RELATIVE_VAL), ofVec3f(0,OF_RELATIVE_VAL,OF_RELATIVE_VAL));
    
    
    // HORIZONTAL SCROLLER
    // We will use this scroller to animate the stars color and rotation
    
    // First add the object to the scroller
    // The star is already a child of the vertical scroller
    // - don't add it as a child to this scroller!
    horizontalScroller->addObject(star, false);
    
    // Add a transform to change the star's color from yellow to pink
    horizontalScroller->getScrollObject(star)->addTransform(OF_SETCOLOR, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(255,255,0), ofVec3f(255,100,255));
    
    // Add a transform to rotate the star
    horizontalScroller->getScrollObject(star)->addTransform(OF_ROTATE, startScroll + offset*i, startScroll + offset*i + scrollDistance, OF_LINEAR, ofVec3f(0,0,0), ofVec3f(0,360,0));
    
    // For each star's transform, add a snap point at the end
    horizontalScroller->addSnapPoint(startScroll + offset*i + scrollDistance);
    verticalScroller->addSnapPoint(startScroll + offset*i + scrollDistance);
    
    // IMPORTANT - Update the height of the scroller
    // We should fit the height of the scroller to our total scroll range
    horizontalScroller->setScrollHeight(startScroll + offset*i + scrollDistance);
    verticalScroller->setScrollHeight(startScroll + offset*i + scrollDistance);
    
    // The next star will start at this value
    startScroll+= scrollDistance;
    
    i++;
    
  }

  // Done building scroller
  // Look at MOUSE DRAGGED to see how we update the scroller velocity
  // Look at KEY PRESSED to see how we use snap points
  // We also call the scroller's update function in UPDATE
  
}


//----Convenience class to build instruction text
void scrollerApp::buildInstructionText(){
  
  //Load a font.
	//ofxSosoTrueTypeFont inherits from ofTrueTypeFont and adds some nice functionality, used by ofxTextObject.
	//Note: If the font has them, kerning pairs are loaded by default.
  font16 = new ofxSosoTrueTypeFont();
  font16->loadFont("Arial.ttf", 16.0f, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
  
  string instructionText[6] = { "Drag mouse horizontally to zip in stars.",
                                "Drag mouse vertically to animate stars.",
                                "Press 's' to go to previous position snap.",
                                "Press 'a' to go to next position snap.",
                                "Press 'x' to go to previous animation snap.",
                                "Press 'z' to go to next animation snap."};
  
  textBackground = new ofxRectangleObject(330,300);
  textBackground->setColor(200, 200, 200,240);
  textBackground->setTrans(-530,-450,1);
  scene->getRoot()->addChild(textBackground);
  
  float yPos = 100;
  for (int i=0; i < 6; i++){
    
    char *s = &instructionText[i][0]; //convert to char*
    ofxTextObject *instruction = new ofxTextObject(font16, s);
    instruction->setColumnWidth(350);
    instruction->setTrans(50,yPos,2);
    instruction->setPointSize(16.0f);
    
    // Color instructions for ease of reading
    if (i<2)
      instruction->setColor(10,10,255);
    else if (i<4)
      instruction->setColor(255,10,10);
    else
      instruction->setColor(10,10,10);
    
    instructions.push_back(instruction);
    textBackground->addChild(instruction);
    
    yPos+=30;
    
    
  }
}

//

//--------------------------------------------------------------
void scrollerApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
  time+=ofGetElapsedTimef();
  
  // We must call update on the scrollers
  horizontalScroller->update(time);
  verticalScroller->update(time);
}

//--------------------------------------------------------------
void scrollerApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void scrollerApp::keyPressed  (int key){

  // Key presses navigate the scrollers
  // to the next or previous snap point
  if (key=='a'){
    
    verticalScroller->gotoNextSnapPoint(500,OF_SCROLL_FORWARD);
    
  }else if (key == 's'){
    
    verticalScroller->gotoNextSnapPoint(500,OF_SCROLL_BACKWARD);
    
  }else if (key == 'z'){
    
    horizontalScroller->gotoNextSnapPoint(500, OF_SCROLL_FORWARD);
    
  }else if (key == 'x'){
    
    horizontalScroller->gotoNextSnapPoint(500, OF_SCROLL_BACKWARD);
  }
}

//--------------------------------------------------------------
void scrollerApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void scrollerApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void scrollerApp::mouseDragged(int x, int y, int button){
  
  // Calculate the difference in mouse position
  float fMouseXDelta = x - prevMouseX;
  float fMouseYDelta = prevMouseY - y; //flip sine...Mouse Y is opposite world Y
  
  // Calculate x/y mouse drag velocity
  mouseXVelocity = fMouseXDelta/(ofGetElapsedTimef() - prevDragTime);
  mouseYVelocity = fMouseYDelta/(ofGetElapsedTimef() - prevDragTime);

  // In this example, we are only updating
  // one scroll velocity at a time, whichever is bigger
  if (abs(fMouseXDelta) > abs(fMouseYDelta)){
    horizontalScroller->setScrollVelocity(mouseXVelocity);
  }else{
    verticalScroller->setScrollVelocity(mouseYVelocity);
  }

  prevMouseX = x;
  prevMouseY = y;
  prevDragTime = ofGetElapsedTimef();
  
}

//--------------------------------------------------------------
void scrollerApp::mousePressed(int x, int y, int button){
 
  prevMouseX = x;
  prevMouseY = y;
  

}

//------------------------------------------------------------
void scrollerApp::mouseReleased(int x, int y, int button){

}


//--------------------------------------------------------------
void scrollerApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void scrollerApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void scrollerApp::dragEvent(ofDragInfo dragInfo){ 
  
}


