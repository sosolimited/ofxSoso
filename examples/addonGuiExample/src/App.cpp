#include "App.h"


//--------------------------------------------------------------
void App::setup(){
  
  //Replace the default ofGLRenderer with ofxSosoRenderer which has overriden setupScreen() and setupScreenPerspective().
	//This lets us set up the scene graph how we want to.
	//Warning: Up is up and down is down in this world.
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(100, 100, 100);
  
  // Build ofxGuiObject
  guiObject = new exampleGuiObject();
  guiObject->setup();
  scene->getRoot()->addChild(guiObject);
  guiObject->setVisible(true);
  
  int gui_x = -ofGetWindowWidth()/2;
  int gui_y = ofGetWindowHeight()/2;
  guiObject->setTrans(gui_x, gui_y, 0);
  
  // Build ofxUIObject
  UIObject = new exampleUIObject();
  UIObject->setup();
  scene->getRoot()->addChild(UIObject);
  UIObject->setVisible(true);
  
  int UI_x = 0;
  int UI_y = ofGetWindowHeight()/2;
  UIObject->setTrans(UI_x, UI_y, 0);
  
}

//--------------------------------------------------------------
void App::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
  
}

//--------------------------------------------------------------
void App::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
  
}

//--------------------------------------------------------------
void App::keyPressed  (int key){
  
}

//--------------------------------------------------------------
void App::keyReleased(int key){
  
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){
  
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){
  
}

//--------------------------------------------------------------
void App::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void App::dragEvent(ofDragInfo dragInfo){ 
  
}

//--------------------------------------------------------------
void App::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void App::UIevent(ofxUIEventArgs &e){
  //TODO: Does this go here? If so, fill in.
}

