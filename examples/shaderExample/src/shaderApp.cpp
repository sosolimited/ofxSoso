// An example showing how to use
// shaders with ofxSoso

// Written by Alex Olivier
// 6/5/2014

// Examples include:
// * Shader with mouse input
// *


// GENERAL TECHNIQUE FOR USING SHADERS WITH OFXSOSO:
//
// ofxObjects now have a built-in ofShader object.  If you
// want to load a shader without parameters, simply call
// object->loadShader(string filename)

// If you need to pass parameters to a shader (mouse value, textures, etc.),
// create your own class and override the ofxObject::setShaderParams() method.
// Within this method, you can call methods like shader->setUniform1f(string varName, float varValue);
// to set the value of a variable of your choice.
//

// If you want to use a more complicated shader that requires multiple
// passes (like blur), you will have to implement


#include "shaderApp.h"


//--------------------------------------------------------------
void shaderApp::setup(){

  //Replace the default ofGLRenderer with ofxSosoRenderer which has overriden setupScreen() and setupScreenPerspective().
	//This lets us set up the scene graph how we want to.
	//Warning: Up is up and down is down in this world.
  //ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofxSosoRenderer(false)));
  
  ofDisableSetupScreen();
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10, 10, 10);

  
  
  // ---------------------------COLORFUL RECT-----------------------------------
  //
  // A rect that uses a shader to control it's color based on mouse value

  // Make an example rectangle object
  colorRect = new shaderColorRect(300,300);
  colorRect->setTrans(-ofGetWidth()/2 + 50, ofGetHeight()/2 - 380,0);
  scene->getRoot()->addChild(colorRect);
  colorRect->setColor(25,25,250);
  shaderObjects.push_back(colorRect);

  // Add a shader to control rects
  colorRect->loadShader("shaders/mouse_to_color/mouse_to_color");
  
  // --------------------------------------------------------------------------
  
  

  // ---------------------------SINE WAVE PLANE---------------------------------
  //
  //  Offset a plane's vertices' y-position by a time-based sine wave
  
  wavyPlane = new shaderWavyPlane(300,300, 10, 10);
  wavyPlane->setTrans(-ofGetWidth()/2 + 700, ofGetHeight()/2 - 250,0);
  scene->getRoot()->addChild(wavyPlane);
  shaderObjects.push_back(wavyPlane);

  wavyPlane->loadShader("shaders/sine_wave_plane/sine_wave_plane");
  // --------------------------------------------------------------------------
  
  
  
  // -----------------------------NOISY PLANE----------------------------------
  //
  // Pass a black and white texture to a shader and map color vals to vertex y-offset
  // Texture is algorithmically generated
  // Noise amount is mapped to mouse x value
  
  noisyPlane = new shaderNoisyPlane(300, 300, 10, 10);
  noisyPlane->setTrans(-ofGetWidth()/2 + 1250, ofGetHeight()/2 - 250, 0);
  scene->getRoot()->addChild(noisyPlane);
  shaderObjects.push_back(noisyPlane);
  
  noisyPlane->loadShader("shaders/noise_texture_plane/noise_texture_plane");
  noisyPlane->setColor(255,255,255,255);
  // --------------------------------------------------------------------------
  
  
  // -----------------------------MASKED IMAGE----------------------------------
  //
  // Masks an image using the red-channel of another image
  
  imageMask = new shaderImageMask("face2.png", "red_star.png");
  imageMask->setTrans(-400, -350, 0);
  scene->getRoot()->addChild(imageMask);
  shaderObjects.push_back(imageMask);
  
  imageMask->loadShader("shaders/image_mask/image_mask");
  // --------------------------------------------------------------------------
  
  
  
  // -----------------------------BLURRED IMAGE--------------------------------
  //
  // Blurs an image based on mouse x-value
  // Uses a double pass shader
  // ADVANCED USE CASE
  
  imageBlur = new shaderImageBlur("face1.png");
  imageBlur->setTrans(150,-350,0);
  scene->getRoot()->addChild(imageBlur);
  shaderObjects.push_back(imageBlur);
  // --------------------------------------------------------------------------
  

  // Make instruction text
  buildInstructionText();
  
}

//--------------------------------------------------------------
void shaderApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.

  // For shaderObjects with mouse values, update the mouse values
  colorRect->setMouseVals(mouseX, mouseY);
  imageBlur->setMouseVals(mouseX, mouseY);
  noisyPlane->setMouseVals(mouseX, mouseY);

  scene->update(ofGetElapsedTimef());

}

//--------------------------------------------------------------
void shaderApp::draw(){

	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
  
}

//--------------------------------------------------------------
void shaderApp::keyPressed  (int key){
  
  ofLogNotice("Toggle shaders!");
  
  if(key == 's'){
    
    shaderEnable = !shaderEnable;

    for (auto obj : shaderObjects)
      obj->setEnableShaders(shaderEnable);
    
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

//----Convenience method to build instruction text
void shaderApp::buildInstructionText(){
  
  //Load a font.
	//ofxSosoTrueTypeFont inherits from ofTrueTypeFont and adds some nice functionality, used by ofxTextObject.
	//Note: If the font has them, kerning pairs are loaded by default.
  font16 = new ofxSosoTrueTypeFont();
  font16->loadFont("Arial.ttf", 16.0f, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
  
  string instructionText[6] = { "Shader maps mouse x/y to rectangle color",
    "Shader maps vertex offset to sine.",
    "Shader generates noise texture .",
    "Shader uses star image as a mask",
    "Shader blurs image based on mouse x.",
    "Press 's' to toggle shaders."};
  
  
  float yPos = 100;
  for (int i=0; i < 6; i++){
    
    char *s = &instructionText[i][0]; //convert to char*
    ofxTextObject *instruction = new ofxTextObject(font16, s);
    instruction->setColumnWidth(350);
    instruction->setTrans(50,yPos,2);
    instruction->setPointSize(16.0f);
    
    // Color instructions for ease of reading
    //if (i<2)
      instruction->setColor(255,255,255);
    //else if (i<4)
    //  instruction->setColor(255,10,10);
    //else
    // instruction->setColor(10,10,10);
    
    instructions.push_back(instruction);
    scene->getRoot()->addChild(instruction);
    
    yPos+=30;
    
    
  }
  
  instructions[0]->setTrans(-700, 50, 0);
  instructions[1]->setTrans(-150, 50, 0);
  instructions[2]->setTrans(350, 50, 0);
  instructions[3]->setTrans(-400, -400, 0);
  instructions[4]->setTrans(150, -400, 0);
  //instructions[5]->setTrans(-700, 150, 0);
  
  instructions[5]->setColor(255,0,0);
  
}

//

//--------------------------------------------------------------


