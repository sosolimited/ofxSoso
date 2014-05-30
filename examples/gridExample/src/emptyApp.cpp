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

  //buildLayout(grid);

  
  
}

// Create and layout objects using the grid
void emptyApp::buildLayout(ofxGridSystem grid)
{
  // Load fonts.
  ofxSosoTrueTypeFont *helveticaBig = new ofxSosoTrueTypeFont();
  helveticaBig->loadFont("HelveNeuLig.otf", 60, true, true, false, true);
  ofxTextObject::addFont("HelveticaHeading", helveticaBig);

  ofxSosoTrueTypeFont *helveticaBody = new ofxSosoTrueTypeFont();
  helveticaBody->loadFont("HelveNeuLig.otf", 12, true, true, false, true);
  ofxTextObject::addFont("HelveticaBody", helveticaBody);


  //ofxTextObject *text = new ofxTextObject(ofxTextObject::getFont("SEOptimistBold320"), buff);
  //number->setColor(200,200,200);
  //number->setPointSize(320);
  //number->setAlignment(OF_TEXT_ALIGN_CENTER);
  //number->setTrans(rC->getTrans() + ofVec3f(0, 100, 0));
  
  
  
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


