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
  numVideosToCreate = 100;
  numCreatedTextObjects = 0;
  numCreatedGridSystems = 0;
  numCreatedCircleObjects = 0;
  numCreatedArcObjects = 0;
  numCreatedLineSegments = 0;
  numCreatedPolygonObjects = 0;
  numCreatedQuadStripObjects = 0;
  numCreatedLineStripObjects = 0;
  numCreatedDynPolygonObjects = 0;
  numCreatedVideoPlayerObjects = 0;
  numCreatedFboObjects = 0;
  numCreatedImageObjects = 0;
  numCreatedRectangleObjects = 0;
  numCreatedRoundedArcObjects = 0;
  numCreatedSceneObjects = 0;
  numCreatedRendererObjects = 0;
  numCreatedFonts = 0;
  numCreatedStateMachines = 0;
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
    // Testing ofxObject & ofxMessage destructors.
    ofxTextObject *testText = new ofxTextObject(font16, "Testing ofxObject and ofxMessage.");
    testText->setColor(0, 255, 255);
    testText->doMessage3f(OF_TRANSLATE, 0, 1, OF_LINEAR, 0, 100, 200);
    testText->doMessage3f(OF_ROTATE, 0, 1, OF_LINEAR, 0, 100, 200);
    testText->doMessage1f(OF_SCALE, 0, 1, OF_LINEAR, 200);
    testText->doMessage3f(OF_SCALE3, 0, 1, OF_LINEAR, 100, 200, 200);
    testText->doMessage3f(OF_SETCOLOR, 0, 1, OF_LINEAR, 150, 10, 200);
    testText->doMessage1f(OF_SETALPHA, 0, 1, OF_LINEAR, 0.5);
    scene->getRoot()->addChild(testText);
    testText->addChild( new ofxTextObject(font16, "") );                //test destruction of children.
    testText->addChild( new ofxTextObject(font16, "") );
    delete testText;
  } else if(key == 'g'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxPolygonObject* polygon = new ofxPolygonObject(8);
      delete polygon;
    }
    numCreatedPolygonObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxPolygonObjects: "+ ofToString(numCreatedPolygonObjects));
  } else if(key == 'h'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxQuadStripObject* quadStrip = new ofxQuadStripObject(4);
      delete quadStrip;
    }
    numCreatedQuadStripObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxQuadStripObjects: "+ ofToString(numCreatedQuadStripObjects));
  } else if(key == 'i'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxLineStripObject* lineStrip = new ofxLineStripObject(4);
      delete lineStrip;
    }
    numCreatedLineStripObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxLinetripObjects: "+ ofToString(numCreatedLineStripObjects));
  } else if(key == 'j'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxDynamicPolygonObject* dynPoly = new ofxDynamicPolygonObject(8);
      delete dynPoly;
    }
    numCreatedDynPolygonObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxDynamicPolygonObjects: "+ ofToString(numCreatedDynPolygonObjects));
  } else if(key == 'k'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxDynamicPolygonObject* dynPoly = new ofxDynamicPolygonObject(8);
      delete dynPoly;
    }
    numCreatedDynPolygonObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxDynamicPolygonObjects: "+ ofToString(numCreatedDynPolygonObjects));
  } else if(key == 'l'){
    
    for(int i=0; i < numVideosToCreate; i++) {
      ofxVideoPlayerObject* videoPlayer = new ofxVideoPlayerObject("RF1308-Superbowl_Closing-Ver02.mp4");
      videoPlayer->start();
      videoPlayerCollection.push_back(videoPlayer);
    }
    numCreatedVideoPlayerObjects += numVideosToCreate;
    counterDisplay->setString("# of ofxVideoPlayerObjects: "+ ofToString(numCreatedVideoPlayerObjects));
  } else if(key == 'L'){
    for(auto videoPlayer : videoPlayerCollection) {
      delete videoPlayer;
    }
    videoPlayerCollection.clear();
    numCreatedVideoPlayerObjects = videoPlayerCollection.size();
    counterDisplay->setString("# of ofxVideoPlayerObjects *: "+ ofToString(numCreatedVideoPlayerObjects));
  } else if(key == 'm'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxFboObject* fbo = new ofxFboObject(400, 400);
      delete fbo;
    }
    numCreatedFboObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxFboObjects: "+ ofToString(numCreatedFboObjects));
  } else if(key == 'n'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxImageObject* image = new ofxImageObject("plasticman.jpg");
      delete image;
    }
    numCreatedImageObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxImageObjects: "+ ofToString(numCreatedImageObjects));
  } else if(key == 'o'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxRectangleObject* rect = new ofxRectangleObject(1000, 800);
      delete rect;
    }
    numCreatedRectangleObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxRectangleObjects: "+ ofToString(numCreatedRectangleObjects));
  } else if(key == 'p'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxRoundedArcObject* rArc = new ofxRoundedArcObject(100,80,90,270);
      delete rArc;
    }
    numCreatedRoundedArcObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxRoundedArcObjects: "+ ofToString(numCreatedRoundedArcObjects));
  } else if(key == 'q'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxScene* scene = new ofxScene(ofGetWidth(), ofGetHeight());
      delete scene;
    }
    numCreatedSceneObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxSceneObjects: "+ ofToString(numCreatedSceneObjects));
  } else if(key == 'r'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxSosoRenderer* renderer = new ofxSosoRenderer();
      delete renderer;
    }
    numCreatedRendererObjects += numObjectsToCreate;
    counterDisplay->setString("# of ofxSosoRenderer: "+ ofToString(numCreatedRendererObjects));
  } else if(key == 's'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxSosoTrueTypeFont* font = new ofxSosoTrueTypeFont();
      font->loadFont("Arial.ttf", 16, true,  true, false, true);
      delete font;
    }
    numCreatedFonts += numObjectsToCreate;
    counterDisplay->setString("# of ofxSosoTrueTypeFont: "+ ofToString(numCreatedFonts));
  } else if(key == 't'){
    
    for(int i=0; i < numObjectsToCreate; i++) {
      ofxStateMachine* state = new ofxStateMachine();
      state->setVerbose(true);
      state->enableTransitionBlocking(true);
      state->addTransition("MAP", "INTRO", 2.01);
      state->addTransition("INTRO", "MAP", 2.01);
      state->addTransition("INTRO", "LIST", 2.01);
      state->addTransition("LIST", "MAP", 2.01);
      
      delete state;
    }
    numCreatedStateMachines += numObjectsToCreate;
    counterDisplay->setString("# of ofxStateMachine: "+ ofToString(numCreatedStateMachines));
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


