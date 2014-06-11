#include "emptyApp.h"


//--------------------------------------------------------------
void emptyApp::setup(){
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();

  // ----------------------------------------
  //Create a scene.
  mainScene = new ofxScene(ofGetWidth(), ofGetHeight());
	mainScene->setBackgroundColor(245, 0, 184);  // The main scene is pink.
  
  image = new ofxImageObject("plasticman.jpg");
  //image = new ofxImageObject("maxresdefault.jpg");  // 1280x720 image proves that perspective and modelview are setup correctly!
  image->isCentered = true;
  image->setTrans(-100, 0, 0);
  mainScene->getRoot()->addChild(image);
  
  
  // ----------------------------------------
  // Set up a scene in an FBO.
  int fboW = 300;
  int fboH = 300;
  
  fboScene = new ofxScene(fboW, fboH);
  fboScene->setBackgroundColor(255, 255, 0);  // The fbo scene is yellow.
  // We're using the default params here, but if you wanted to
  // you could set the PROJECTION / LOOKAT params for your fbo scene
  // indpendently from the scene above.
  fboScene->setScreenParams(false, false, 60);
  
  // Create and position the fbo object.
  fbo = new ofxFboObject(fboW, fboH);
  fbo->setTrans(250, -fboH/2, 0);
  mainScene->getRoot()->addChild(fbo); // The fbo object gets added to the main scene.
  
  // Create an image to add to the fbo scene.
  fboImage = new ofxImageObject("plasticman2.jpg");
  fboImage->isCentered = true;
  fboImage->setScale(0.25);
  fboScene->getRoot()->addChild(fboImage);
  
}

//--------------------------------------------------------------
void emptyApp::update(){
  
  mainScene->update(ofGetElapsedTimef());
  fboScene->update(ofGetElapsedTimef());
  
}

//--------------------------------------------------------------
void emptyApp::draw(){
  
  // Draw the main scene.
  mainScene->draw();
  
  // Draw the fbo scene inside of the fbo begin/end block.

  // Note: Call begin with false to disable fbo setupscreen.
  // ofxScene now handles screen setup.
  fbo->fbo->begin(false);
  fboScene->draw();
  fbo->fbo->end();

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


