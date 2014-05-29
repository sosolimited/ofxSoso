#include "emptyApp.h"


//--------------------------------------------------------------
void emptyApp::setup(){
  
  //Replace the default ofGLRenderer with ofxSosoRenderer which has overriden setupScreen() and setupScreenPerspective().
	//This lets us set up the scene graph how we want to.
	//Warning: Up is up and down is down in this world.
  ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10, 10, 10);
  
  // Create object counter that shows the number of created objects for Destructor testing.
  font16 = new ofxSosoTrueTypeFont();
  font16->loadFont("Arial.ttf", 16, true,  true, false, true);
	font16->setKerningPair('T', 'y', -2);
  counterDisplay = new ofxTextObject(font16, "# of objects: 0");
  counterDisplay->setColor(0, 255, 255);
  counterDisplay->setPointSize(16);     //DEV: why doesnt work?
  counterDisplay->setLeading(19);
  counterDisplay->setColumnWidth(360);
  counterDisplay->setTrans(0, 0, 5.0);
  scene->getRoot()->addChild(counterDisplay);
  
  // Init counters for each destructor test.
  numObjectsToCreate = 1000;
  numCreatedTextObjects = 0;
  numCreatedGridSystems = 0;
  numCreatedCircleObjects = 0;
  numCreatedArcObjects = 0;
  numCreatedLineSegments = 0;
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
  
  if(key == 'a'){
    
    // Create the objects.
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxTextObject* text = new ofxTextObject(font16, "Life is thickly sown with thorns, and I know no other remedy than to pass quickly through them. The longer we dwell on our misfortunes, the greater is their power to harm us. -Voltaire");
      delete text;
    }
    
    // Update counter to display # of created objs.
    numCreatedTextObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxTextObjects: "+ ofToString(numCreatedTextObjects));

  } else if(key == 'b'){
   
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxGridSystem* grid = new ofxGridSystem(1080, 1920,
                    5, 14,
                    108, 28,     // left = menu + gutter; right = gutter
                    108, 108,    // top = bottom = left
                    2);
      delete grid;
    }
    numCreatedGridSystems += numObjectsToCreate;
    counterDisplay->setString("# of ofxGridSystems: "+ ofToString(numCreatedGridSystems));
  } else if(key == 'c'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxCircleObject* circle = new ofxCircleObject(120, 100.0);
      delete circle;
    }
    numCreatedCircleObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxCircleObjects: "+ ofToString(numCreatedCircleObjects));
  } else if(key == 'd'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxArcObject* arc = new ofxArcObject(100,80,90,270);
      delete arc;
    }
    numCreatedArcObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxArcObjects: "+ ofToString(numCreatedArcObjects));
  } else if(key == 'e'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxLineSegmentObject* lineSegment = new ofxLineSegmentObject(1000);
      delete lineSegment;
    }
    numCreatedLineSegments += numObjectsToCreate;
    counterDisplay->setString("# of ofxLineSegmentObjects: "+ ofToString(numCreatedLineSegments));
  } else if(key == 'f'){
    // Testing proper destruction of messages in ofxObject & ofxMessage destructors.
    counterDisplay->doMessage3f(OF_TRANSLATE, 0, 1, OF_LINEAR, 0, 100, 200);
    counterDisplay->doMessage3f(OF_ROTATE, 0, 1, OF_LINEAR, 0, 100, 200);
    counterDisplay->doMessage1f(OF_SCALE, 0, 1, OF_LINEAR, 200);
    counterDisplay->doMessage3f(OF_SCALE3, 0, 1, OF_LINEAR, 100, 200, 200);
    counterDisplay->doMessage3f(OF_SETCOLOR, 0, 1, OF_LINEAR, 150, 10, 200);
    counterDisplay->doMessage1f(OF_SETALPHA, 0, 1, OF_LINEAR, 0.5);
    counterDisplay->addChild( new ofxTextObject(font16, "") );                //test destruction of children.
    counterDisplay->addChild( new ofxTextObject(font16, "") );
    delete counterDisplay;
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


