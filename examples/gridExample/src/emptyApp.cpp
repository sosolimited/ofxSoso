#include "emptyApp.h"


//--------------------------------------------------------------
void emptyApp::setup(){
  
  // ofxScene setup
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10, 10, 10);
  // Enable onTop drawing for the sake of the grid lines, which are set up to always render on top.
  scene->enableOnTopDrawing(true);

  
  root = new ofxObject();
  root->setTrans(-ofGetWidth()/2, -ofGetHeight()/2, 0);
  scene->getRoot()->addChild(root);
  
  
  // Add the grid to the scene to be able to see the grid lines.
  // It is not necessary to be able to use the grid.
  scene->getRoot()->addChild(&grid);
  // The grid references a lower left origin.
  // Align the root of your objects with the origin of the grid.
  grid.setTrans(root->getTrans());
  
  // Show the grid lines.
  // You can show the lines and base lines independently.
  grid.showLines(true);
  grid.showBaselines(true);
  // Press 'g' to hide lines.
  // Press 'G' to show them

  buildLayout(&grid, root);
}

// Create and layout objects using the grid
void emptyApp::buildLayout(ofxGridSystem* grid, ofxObject* iRoot)
{
  // Load fonts.
  ofxSosoTrueTypeFont *helveticaBig = new ofxSosoTrueTypeFont();
  helveticaBig->loadFont("HelveNeuLig.otf", 60, true, true, false, true);
  ofxTextObject::addFont("HelveticaHeading", helveticaBig);

  ofxSosoTrueTypeFont *helveticaBody = new ofxSosoTrueTypeFont();
  helveticaBody->loadFont("HelveNeuLig.otf", 12, true, true, false, true);
  ofxTextObject::addFont("HelveticaBody", helveticaBody);
  
  ofxTextObject* p1 = new ofxTextObject(helveticaBody, "Although this detail has no connection whatever with the real substance of what we are about to relate, it will not be superfluous, if merely for the sake of exactness in all points, to mention here the various rumors and remarks which had been in circulation about him from the very moment when he arrived in the diocese. True or false, that which is said of men often occupies as important a place in their lives, and above all in their destinies, as that which they do. M. Myriel was the son of a councillor of the Parliament of Aix; hence he belonged to the nobility of the bar.");
  
  p1->setColumnWidth(grid->colW); // set text body to have width of grid column
  p1->setTrans((*grid)[0][1]); // move the text to the first column, 2nd line
  p1->setLeading(grid->leading); // set the text body leading to match the leading of the grid system.
  iRoot->addChild(p1);
  
  ofxTextObject* p2 = new ofxTextObject(helveticaBig, "Chapter III: Two Misfortunes Make One Piece of Good Fortune");
  p2->setColumnWidth(grid->col(3)); // set column width of text object to the width of 3 grid system columns
  p2->setTrans((*grid)[1][3]);
  p2->setLeading(grid->leading*4); // set leading of this text object to 4x leading of the grid system
  iRoot->addChild(p2);
  
  ofxImageObject* man = new ofxImageObject("plasticman.jpg", true);
  float scaleFact = grid->col(2) / man->width; // scale image to fit the width of 2 columns
  man->setScale(scaleFact);
  man->setTrans((*grid)[6][0] + ofVec3f(0,-man->height*scaleFact,0)); // align the image to the top of the grid system, at 7th column
  iRoot->addChild(man);
  
  ofxTextObject* p3 = new ofxTextObject(helveticaBody, " On examining it he found that the door was not a door; it had neither hinges, cross-bars, lock, nor fissure in the middle; the iron bands traversed it from side to side without any break. Through the crevices in the planks he caught a view of unhewn slabs and blocks of stone roughly cemented together, which passers-by might still have seen there ten years ago. He was forced to acknowledge with consternation that this apparent door was simply the wooden decoration of a building against which it was placed. It was easy to tear off a plank; but then, one found one's self face to face with a wall.");
  
  p3->setColumnWidth(grid->col(2));
  p3->setLeading(grid->leading*2); // set leading twice a single line
  p3->setTrans((*grid)[4][1]);
  iRoot->addChild(p3);
  
  ofxRectangleObject* rect = new ofxRectangleObject(grid->col(2), grid->leading/4);
  rect->setTrans(grid->colX(4),grid->bottomY,0); // use colX(n) to get the X position of a specific column
  iRoot->addChild(rect);
}


//--------------------------------------------------------------
void emptyApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void emptyApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void emptyApp::keyPressed  (int key){
  // Press 'g' to hide lines.
  // Press 'G' to show them.
  if(key == 'g'){
    grid.showLines(false);
    grid.showBaselines(false);
  }else if(key == 'G'){
    grid.showLines(true);
    grid.showBaselines(true);
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
  
}

//--------------------------------------------------------------
void emptyApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void emptyApp::dragEvent(ofDragInfo dragInfo){ 
  
}


