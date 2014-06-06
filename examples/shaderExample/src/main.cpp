#include "ofMain.h"
#include "shaderApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
  
  ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			// <-------- setup the GL context
  ofSetupOpenGL(&window, 2048,1024, OF_WINDOW);			// <-------- setup the GL context
  
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new shaderApp());
  
}
