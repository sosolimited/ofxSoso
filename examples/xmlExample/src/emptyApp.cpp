#include "emptyApp.h"
#include "ofxCircleObject.h"
#include "ofxTextObject.h"
#include "ofxSosoTrueTypeFont.h"

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
	scene->setBackgroundColor(180, 180, 180);
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // example 1: basic tree manipulation
  //
  
  // basic XPath syntax supported by Poco XML (from http://pocoproject.org/docs/Poco.XML.Node.html):
  // elem1/elem2/elem3
  // /elem1/elem2/elem3
  // /elem1/elem2[1]
  // /elem1/elem2[@attr1]
  // /elem1/elem2[@attr1='value']
  // //elem2[@attr1='value']
  // //[@attr1='value']
  
  
  ofXml tree;
  
  if(!tree.load("linebreaks.xml"))
    ofLogError() << "failed to open XML file" << endl;
  
  ofLogNotice() << "number of children under root node: " << tree.getNumChildren();
  
  // with ofXml, we have to set where we want to operate within the tree, similar to ofXmlSettings
  // setting to first child of current (root) node
  tree.setToChild(0);
  
  // iterate over all the children
  do {
    ofLogNotice() << "child ID: " << tree.getAttribute("id");
    ofLogNotice() << "child contents: " << tree.getValue();
  }
  while(tree.setToSibling()); // setToSibling() auto advances the target sibling element each call
  
  // switch back to root and add a 5th child
  tree.setToParent();
  
  if(tree.addChild("story")){
    ofLogNotice() << "Added an empty story. There are now " << tree.getNumChildren() << " nodes under root" << endl;
  }
  
  // set inner value of the newly added story
  tree.setValue("story[4]", "A final piece of the story");
  
  // give it an ID
  tree.setToChild(4);
  tree.setAttribute("id", "4");
  
  // add a new element inside of it
  //tree.addChild("author");
  //tree.setValue("author", "soso");
  
  // show new tree
  cout << tree.toString() << endl;
  
  // loop through tree and draw 3 stories to screen
  ofxSosoTrueTypeFont *font12;
  font12 = new ofxSosoTrueTypeFont();
  font12->loadFont("Arial.ttf", 12, true,  true, false, true);
  
  tree.setToParent();
  for(int i=0; i<5; i++){
    tree.setToChild(i);
    ofxTextObject* tobj = new ofxTextObject(font12, tree.getValue());
    tobj->setColumnWidth(200);
    tobj->setTrans(0,80+(i*50),0);
    scene->getRoot()->addChild(tobj);
    tree.setToParent();
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  // example 2: traversing and drawing from a simple SVG
  //
  
  ofXml circles;
  
  if(!circles.load("circles.svg")){
    ofLogError("Could not load circles.svg");
  }
  
  circles.setToChild(1);
  
  do {
    // first convert the element attribs to ints
    int x, y, r;
    istringstream(circles.getAttribute("cx")) >> x;
    istringstream(circles.getAttribute("cy")) >> y;
    istringstream(circles.getAttribute("r")) >> r;
    
    ofxCircleObject* cir = new ofxCircleObject(100,r,r-3);
    cir->setTrans(x,-200+y,0);
    scene->getRoot()->addChild(cir);
  }
  while( circles.setToSibling());
  
  ofxTextObject* ct = new ofxTextObject(font12, "example 2:\nsimple SVG traversal");
  ct->setTrans(-10,-240,0);
  ct->setColumnWidth(300);
  scene->getRoot()->addChild(ct);
  
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // example 3: more complex SVG traversal
  //
  
  ofxObject* wall = new ofxObject();
  
  ofXml wallxml;
  
  if(!wallxml.load("lights.svg")){
    ofLogError("Could not load lights.svg");
  }
  
  // make sure we're at the document root and not the first element (a comment)
  wallxml.setToParent();
  
  wallxml.setTo("/svg");
  ofLogNotice() << "There are " << wallxml.getNumChildren() << " wall panels.";
  
  // for every group
  for(int wp = 0; wp < wallxml.getNumChildren(); wp++){
    
    stringstream wall_id;
    wall_id << "/g[" << wp << "]";
    wallxml.setTo(wall_id.str()); // set to the wall panel
    
    int numLights = wallxml.getNumChildren(); // count # lights in panel
    string groupID = wallxml.getAttribute("id");
    
    // for every light
    for(int li=0; li < numLights; li++){
      float x,y,r;
      string color;
      
      //ofLogNotice() << "Building light " << li << " in light group" << groupID << " with position " << x << " " << y << " (radius: " << r << ")";
      
      wallxml.setToChild(li); // set to the light (<circle>) we are looking at
      
      istringstream(wallxml.getAttribute("cx")) >> x;
      istringstream(wallxml.getAttribute("cy")) >> y;
      istringstream(wallxml.getAttribute("r")) >> r;
      color = wallxml.getAttribute("fill");
      
      // grab the color from <circle>
      stringstream c;
      unsigned int i_color;
      c << hex << color.substr(1,color.length());
      c >> i_color;
    
      ofxCircleObject* cir = new ofxCircleObject(100,r,0);
      cir->setTrans(x,y,0);
      cir->setColor(ofColor::fromHex(i_color));
      wall->addChild(cir);
      
      wallxml.setToParent(); // set back to the parent group (<g>)
    }
    wallxml.setToParent(); // set back to <svg>
  }
  
  wall->setTrans(-400,-100,0);
  wall->setScale(2);
  scene->getRoot()->addChild(wall);
  
  ofxTextObject* t = new ofxTextObject(font12, "example 3:\nmore advanced SVG traversal");
  t->setTrans(-400,-130,0);
  t->setColumnWidth(300);
  scene->getRoot()->addChild(t);
  
             
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


