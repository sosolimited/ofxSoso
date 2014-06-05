#include "iostream.h"
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
    
    font16 = new ofxSosoTrueTypeFont();
    font16->loadFont("Arial.ttf", 16, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
    
    readOut = new ofxTextObject(font16, "Your R,G,B and X,Y,Z read out.");
    readOut->setColor(255, 255, 255);										//Set the color of the text.
    readOut->setPointSize(16);												//Set the point size of the text, independent of the font resolution.
    readOut->setLeading(19);												//Set the leading (line-spacing) of the text.
    //text->setScale(0.5);												//Text can be scaled like any other object. So the final scale is the pointSize x scale.
    readOut->setColumnWidth(360);											//Set the column width, which determines how the words wrap.
	readOut->setTrans(-300,300,0);			//Position the text, relative to the image object above.
	scene->getRoot()->addChild(readOut);
    
    //--------------------------------------------------------------
    
    //Create Circle Object with Container
    circle = new ofxCircleObject(100,50);
    
    container = new ofxObject();
    
    container->addChild(circle);
    container->setTrans(100,-200,0);
    
    scene->getRoot()->addChild(container);
    
    
  
}

//--------------------------------------------------------------
void emptyApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
    string text = "Your values" + ofToString(container->getX());
    
    readOut->setString(text);
    
    
}


//--------------------------------------------------------------
void emptyApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void emptyApp::keyPressed  (int key){
    if (key == 'a') {
        container->doMessage3f(OF_TRANSLATE, 0, 2, OF_LINEAR, 235, -45, 50);
        
    }
    else if (key == 'A') {
        
        container->doMessage3f(OF_TRANSLATE, 0, 2, OF_LINEAR, 100, -200, 0);
        
    }
    else if (key == 's') {
        container->doMessage3f(OF_SETCOLOR, 0, 1, OF_LINEAR, 30, 100, 234);
    }
    else if (key == 'S') {
        container->doMessage3f(OF_SETCOLOR, 0, 1, OF_LINEAR, 255, 255, 255);
    }
    else if (key == 'r') {
        std::cout << "X:";
        std::cout << container->getX();
        std::cout << " Y:";
        std::cout << container->getY();
        std::cout << " Z:";
        std::cout << container->getZ() << std::endl;
        std::cout << "R:";
        std::cout << container->getRed();
        std::cout << " G:";
        std::cout << container->getGreen();
        std::cout << " B:";
        std::cout << container->getBlue() << std::endl;
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


