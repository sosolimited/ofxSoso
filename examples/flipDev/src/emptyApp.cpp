#include "emptyApp.h"


//--------------------------------------------------------------
void emptyApp::setup(){
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
  
  //Create a scene.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(245, 0, 184);
  
  image = new ofxImageObject("plasticman.jpg");
  image->isCentered = true;
  scene->getRoot()->addChild(image);
  
  //ofIm.loadImage("plasticman.jpg");
  
  int fboW = 300;
  int fboH = 300;
  
  fboScene = new ofxScene(fboW, fboH);
  fboScene->setBackgroundColor(255, 255, 0);
  
  fbo = new ofxFboObject(fboW, fboH);
  scene->getRoot()->addChild(fbo);
  
}

//--------------------------------------------------------------
void emptyApp::update(){
  
  scene->update(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void emptyApp::draw(){
  
  scene->draw();
  
  //ofIm.draw(0,400);

}

//--------------------------------------------------------------
void emptyApp::keyPressed  (int key){
  
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


