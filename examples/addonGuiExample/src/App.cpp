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
  
  //Create Circle Object
  circle = new ofxCircleObject(defaultRes, 50);
  circle->setTrans(xPosition, yPosition, 0);
  circle->setScale(sliderScale/100);
  scene->getRoot()->addChild(circle);
  
  //Smile
  eye1 = new ofxCircleObject(30, 6);
  eye2 = new ofxCircleObject(30, 6);
  arcSmile = new ofxArcObject(28,26, 200, 340);
  eye1->setColor(255, 255, 255, 0);
  eye1->setTrans(-15, 15, 1);
  eye2->setColor(255, 255, 255, 0);
  eye2->setTrans(15, 15, 1);
  arcSmile->setColor(255, 255, 255, 0);
  arcSmile->setTrans(0, 0, 1);
  circle->addChild(eye1);
  circle->addChild(eye2);
  circle->addChild(arcSmile);
  
  
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
  
  //Background Color Control Toggle
  if (bgControl) {
    if (!bgInvert) {
      scene->setBackgroundColor(sliderR, sliderG, sliderB);
    } else {
      scene->setBackgroundColor(255-circle->getRed(), 255-circle->getGreen(), 255-circle->getBlue());
    }
  } else {
    scene->setBackgroundColor(defaultR, defaultG, defaultB);
  }
  
  //Smile Visibility Control
  if (toggleSmile) {
    eye1->setAlpha(255);
    eye2->setAlpha(255);
    arcSmile->setAlpha(255);
  } else {
    eye1->setAlpha(0);
    eye2->setAlpha(0);
    arcSmile->setAlpha(0);
  }
  
  //Circle Controls
  circle->setTrans(xPosition-ofGetWidth()/2, (yPosition-ofGetHeight()/2)*-1, 0);
  circle->setRed(sliderR);
  circle->setGreen(sliderG);
  circle->setBlue(sliderB);
  circle->setResolution(sliderRes);
  circle->setScale(sliderScale/100);
  
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
  } else if (name == "position") {
    ofxUI2DPad *padPos = (ofxUI2DPad *) e.widget;
    xPosition = (float) padPos->getValue().x*ofGetWidth();
    yPosition = (float) padPos->getValue().y*ofGetHeight();
  } else if (name == "resolution") {
    ofxUIMinimalSlider *slider = (ofxUIMinimalSlider *) e.widget;
    sliderRes = (int) slider->getValue();
  } else if (name == "scale") {
    ofxUIMinimalSlider *slider = (ofxUIMinimalSlider *) e.widget;
    sliderScale = (float) slider->getValue();
  } else if (name == "Smile") {
    ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
    toggleSmile = toggle->getValue();
  }


}

