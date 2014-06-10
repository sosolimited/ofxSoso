#include "App.h"


//--------------------------------------------------------------
void App::setup(){
  
  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(defaultR, defaultG, defaultB);
  
  // Build UIObject
  UIObject = new exampleUIObject();
  UIObject->setup();
  scene->getRoot()->addChild(UIObject);
  UIObject->setVisible(true);
  
  int cornerOffset = 20;
  
  int UI_x = -ofGetWindowWidth()/2 + cornerOffset;
  int UI_y = ofGetWindowHeight()/2 - cornerOffset;
  UIObject->setTrans(UI_x, UI_y, 0);
  
  // Add Event Listeners
  ofAddListener(UIObject->UI->newGUIEvent, this, &App::UIEvent);
  
}

//--------------------------------------------------------------
void App::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
  
  UIObject->update(); // This could also be done with an idle() method in UIObject.
  
  if (bgControl) {
    if (!bgInvert) {
      scene->setBackgroundColor(sliderR, sliderG, sliderB);
    } else {
      scene->setBackgroundColor(255-sliderR, 255-sliderG, 255-sliderB);
    }
  } else {
    scene->setBackgroundColor(defaultR, defaultG, defaultB);
  }
  
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
// Monitor UI events to update the appropriate values based on input.
void App::UIEvent(ofxUIEventArgs &e){
  
  string name = e.widget->getName();
  
  if (name == "toggle bg control") {
    ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
    bgControl = toggle->getValue();
  } else if (name == "red"){
    ofxUIMinimalSlider *slider = (ofxUIMinimalSlider *) e.widget;
    sliderR = (int) slider->getValue();
  } else if (name == "green"){
    ofxUIMinimalSlider *slider = (ofxUIMinimalSlider *) e.widget;
    sliderG = (int) slider->getValue();
  } else if (name == "blue"){
    ofxUIMinimalSlider *slider = (ofxUIMinimalSlider *) e.widget;
    sliderB = (int) slider->getValue();
  } else if (name == "invert bg color"){
    ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
    bgInvert = button->getValue();
  }


}

