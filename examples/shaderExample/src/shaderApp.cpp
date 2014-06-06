// An example showing how to use
// shaders with ofxSoso

// Written by Alex Olivier
// 6/5/2014

// Examples include:
// Shader with input
// Pass through shader for rect
// Pass through shader for image
// Multiple shader passes



#include "shaderApp.h"


//--------------------------------------------------------------
void shaderApp::setup(){
  
  //Replace the default ofGLRenderer with ofxSosoRenderer which has overriden setupScreen() and setupScreenPerspective().
	//This lets us set up the scene graph how we want to.
	//Warning: Up is up and down is down in this world.
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10, 10, 10);
  
  // Make an example rectangle object
  rect = new ofxColorRect(300,300);
  rect->setTrans(-ofGetWidth()/2 + 480,ofGetHeight()/2 - 380,0);
  scene->getRoot()->addChild(rect);
  
  rect->setColor(0,255,0);
  
  // Add a simple shader to the rect
  rect->loadShader("shaders/shader1/shader");
  
  //  Make a wavy plane object
  wavyPlane = new ofxWavyPlane(300,300, 10, 10);
  wavyPlane->setTrans(-ofGetWidth()/2 + 180,ofGetHeight()/2 - 250,0);
  scene->getRoot()->addChild(wavyPlane);
  
  wavyPlane->loadShader("shaders/shader2/shader");
  
  noisyPlane = new ofxNoisyPlane(300, 300, 10, 10);
  noisyPlane->setTrans(0, ofGetHeight()/2 - 250, 0);
  scene->getRoot()->addChild(noisyPlane);
  
  noisyPlane->loadShader("shaders/shader3/shader");
  
  imageMerge = new ofxImageMask("face2.png", "red_star.png");
  imageMerge->setTrans(-ofGetWidth()/2 + 20, -300, 0);
  scene->getRoot()->addChild(imageMerge);
  
  imageMerge->loadShader("shaders/shader4/shader");
  
  imageBlur = new ofxImageBlur("face2.png");
  scene->getRoot()->addChild(imageBlur);
  
}

//--------------------------------------------------------------
void shaderApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());

  rect->setMouseVals(mouseX, mouseY);
  //percentX = ofMap(percentX, 0, 1, 0, 255);
  //ofColor colorLeft = ofColor::magenta;
  //ofColor colorRight = ofColor::cyan;
  //ofColor colorMix = colorLeft.getLerped(colorRight, percentX);
  //rect->setColor(colorMix);
 // ofLogNotice("Sending " + ofToString(percentX));

  //rect->getShader().setUniform1f("percentX", percentX);
  
}

//--------------------------------------------------------------
void shaderApp::draw(){
  

  //rect->val = mouseX;
  //rect->val2 = mouseY;
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void shaderApp::keyPressed  (int key){
  
  ofLogNotice("Toggle shaders!");
  
  if(key == 's'){
    
    shaderEnable = !shaderEnable;
    rect->setEnableShaders(shaderEnable);
    
  }
}

//--------------------------------------------------------------
void shaderApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void shaderApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void shaderApp::mouseDragged(int x, int y, int button){
  
  
}

//--------------------------------------------------------------
void shaderApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void shaderApp::mouseReleased(int x, int y, int button){
  
  
}


//--------------------------------------------------------------
void shaderApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void shaderApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void shaderApp::dragEvent(ofDragInfo dragInfo){ 
  
}




