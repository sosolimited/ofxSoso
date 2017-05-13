#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

  // Disable the of setupScreen because now each scene has a custom renderer.
  ofDisableSetupScreen();
  
  //Create a scene.
	//The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	//When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
  scene = new ofxScene(ofGetWidth(), ofGetHeight());
	scene->setBackgroundColor(10, 10, 10);
  
  
	//_________________________________________________________________________________________________________________
  
	//Create an image with an alpha channel.
  image = new ofxImageObject("soso_LOGO.png");
	image->setSpecialTransparency(true);																//Call this on objects that have textures with an alpha channel, so the scene knows how to render and sort them properly.
  image->setTrans(-ofGetWidth()/2 + 70, ofGetHeight()/2 - image->height - 40, 5.0);					//Set the translation of the object.
	image->setCentered(false);																			//Call this with true to center it.	Defaults to lower left origin.
  scene->getRoot()->addChild(image);																	//Add the object to the scene's root.
  
  
	//Create a video object.
	//movie = new ofxVideoPlayerObject("fingers.mov");
	//movie->player->setLoopState(OF_LOOP_NORMAL);
	//movie->setCentered(true);																			//Call this with true to center it. Defaults to lower left origin.
	//movie->setTrans(0, 0, 10);
	//movie->start();
	//scene->getRoot()->addChild(movie);
  
	
	//_________________________________________________________________________________________________________________
  
	//Load a font.
	//ofxSosoTrueTypeFont inherits from ofTrueTypeFont and adds some nice functionality, used by ofxTextObject.
	//Note: If the font has them, kerning pairs are loaded by default.
  font16 = new ofxSosoTrueTypeFont();
  font16->loadFont("Arial.ttf", 16, true,  true, false, true);		//The last bool argument enables mipmapping on the letter textures.
//	font16->setKerningPair('T', 'y', -2);								//After you've loaded a font, you can also manually adjust kerning pairs.
  
	font64 = new ofxSosoTrueTypeFont();
  font64->loadFont("Arial.ttf", 64, true,  true, false, true);
	
  
	//Create a text object.
  text = new ofxTextObject(font16, "An OpenFrameworks Addon by Sosolimited");
  text->setColor(255, 255, 255);										//Set the color of the text.
  text->setPointSize(16);												//Set the point size of the text, independent of the font resolution.
  text->setLeading(19);												//Set the leading (line-spacing) of the text.
  //text->setScale(0.5);												//Text can be scaled like any other object. So the final scale is the pointSize x scale.
  text->setColumnWidth(360);											//Set the column width, which determines how the words wrap.
	text->setTrans(image->getTrans() + ofVec3f(10, -text->getLeading(), 0));			//Position the text, relative to the image object above.
	scene->getRoot()->addChild(text);									//Add the text object to the scene.
  
	//Create a letter text object. This lets you treat each letter as a separate object to animate as you please.
  //See how the letters are animated below in keyPressed()
  letterText = new ofxLetterTextObject(font64, "This is a letter text object. Press 'g' to animate the letters. ");
  letterText->setTrans(image->getTrans().x, -230, 0);
  letterText->setColor(255, 255, 255);
  letterText->setPointSize(48);
  letterText->setLeading(52);
  letterText->setColumnWidth(700);
  letterText->setAlignment(OF_TEXT_ALIGN_LEFT);
  scene->getRoot()->addChild(letterText);
  
  
	//_________________________________________________________________________________________________________________
  
	//Create a textured dynamic polygon object.
	//See how it is animated below in keyPressed().
	polyTex = new ofImage();												//Create an ofImage to be used to texture the polygon.
	polyTex->loadImage("plasticman.jpg");
	
	dynamicPolygon = new ofxDynamicPolygonObject(4);						//Create a dynamic polygon with 4 vertices.
	//dynamicPolygon->setSpecialTransparency(true);							//Call this if your texture has an alpha channel.
	dynamicPolygon->enableVertexColoring(false);							//Shut off vertex coloring.
	dynamicPolygon->setTrans(250, -70, 0);
	dynamicPolygon->setScale(0.5);											//Scale the polygon.
	dynamicPolygon->setTexture(polyTex);									//Set the texture of your polygon.
  
	//Set the home positions and tex coords of the vertices.
	//Note: We start at the lower left corner and move around counter clockwise, as a general practice.
	dynamicPolygon->setVertexPos(0, ofVec3f(0, 0.3*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(0, 0, 0.7*polyTex->getHeight());
	//
	dynamicPolygon->setVertexPos(1, ofVec3f(polyTex->getWidth(), 0.3*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(1, polyTex->getWidth(), 0.7*polyTex->getHeight());
	//
	dynamicPolygon->setVertexPos(2, ofVec3f(polyTex->getWidth(), 0.7*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(2, polyTex->getWidth(), 0.3*polyTex->getHeight());
	//
	dynamicPolygon->setVertexPos(3, ofVec3f(0, 0.7*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(3, 0, 0.3*polyTex->getHeight());
  
	scene->getRoot()->addChild(dynamicPolygon);								//Add the polygon to the scene.
  
	
  
	//Create a label for the polygon.
	polygonLabel = new ofxTextObject(font16, "Here is a dynamic polygon. Press UP and DOWN to animate it.");
	polygonLabel->setColor(255, 255, 255);
	polygonLabel->setPointSize(16);
	polygonLabel->setLeading(19);
	polygonLabel->setColumnWidth(250);
	polygonLabel->setTrans(dynamicPolygon->getTrans() + ofVec3f(0, polygonLabel->getLeading() + 5, -1.0));	//Position the text relative to the position of the polygon created above.
	scene->getRoot()->addChild(polygonLabel);
  
  
	//_________________________________________________________________________________________________________________
  
	//Make a root for the circle objects created below and position it.
	circleRoot = new ofxObject();
	circleRoot->setTrans(-ofGetWidth()/2 + 180, 20, 0);
	scene->getRoot()->addChild(circleRoot);
  
	//Make some circle objects, position them, and add them to the circleRoot object.
	int numCircles = 12;
	for(int i=0; i < numCircles; i++){
		ofxCircleObject *circ = new ofxCircleObject(120, 100.0);
		circ->setColor(255, 200-i*10, 200);										//Set an object's color.
		circ->setAlpha(100);													//Set an object's transparency independently.
		circ->setTrans(70*cos(TWO_PI*(float)i/(float)numCircles),				//Set an object's translation.
                   70*sin(TWO_PI*(float)i/(float)numCircles),
                   -2.0);
		circleRoot->addChild(circ);												//Add the object to the scene.
		circles.push_back(circ);
	}
  
	//Create a label for the circles. Notice the use of named entities for left and right single quotes.
	circlesLabel = new ofxTextObject(font16, "Here are some circles.\nPress &lsquo;a&rsquo; to animate them with messages. Press &lsquo;A&rsquo; to animate them with an animation.");
	circlesLabel->setColor(0,0,0);
	circlesLabel->setPointSize(16);
	circlesLabel->setLeading(19);
	circlesLabel->setColumnWidth(200);
	circlesLabel->setTrans(-circlesLabel->getColumnWidth()/2.0, 2.0*circlesLabel->getLeading(), 2.0);
	circleRoot->addChild(circlesLabel);
	
	//Create an animation with the circles.
	//Call tween on the animation to tell it to do something (OF_TRANSLATE, OF_ROTATE, OF_SCALE, OF_SETCOLOR, OF_SETALPHA)
	//from a start to end time, with a specific interpolation (OF_EASE_OUT, OF_EASE_IN, OF_OF_EASE_INOUT).
	//Note: See ofxAnimation for more advanced tweening features.
	animation = new ofxAnimation();
	float offset = 0.1;
	for(int i=0; i < circles.size(); i++){
		//Grab translation and color for circles, as set above.
		ofVec3f curTrans = circles[i]->getTrans();
		ofColor curColor = circles[i]->getColor();
    
		animation->tween(circles[i], OF_SCALE, i*offset, i*offset + 0.5, OF_EASE_OUT, OF_RELATIVE_VAL, 0.5);		//You can pass OF_RELATIVE_VAL as the first animation value to animate from wherever the object is at the time the animation is called
		animation->tween(circles[i], OF_SETCOLOR, i*offset, i*offset + 0.5, OF_EASE_OUT,							//Animate the color over this timeframe with this interpolation,
                     curColor.r, curColor.g, curColor.b, curColor.a,												//starting with this color,
                     0, 255, 0, 255);																			//and ending at this color.
		animation->tween(circles[i], OF_SETCOLOR, i*offset + 0.5, i*offset + 1.0, OF_EASE_OUT,						//Animate the color back...
                     0, 255, 0, 255,
                     curColor.r, curColor.g, curColor.b, curColor.a);
	}
  
  
	//_________________________________________________________________________________________________________________
  
	//Make and lay out some lines. See how they are animated below in keyPressed().
	lineRoot = new ofxObject();
	lineRoot->setTrans(30, 20, 0);
	scene->getRoot()->addChild(lineRoot);
  
	int numLines = 120;
	for(int i=0; i < numLines; i++){
		ofxLineSegmentObject *line = new ofxLineSegmentObject(2);
		line->setVertexPos(0, 0, 100, 0);
		line->setVertexPos(1, 0, 180, 0);
		float gray = 255 - i/(float)numLines * 200;
		line->setColor(gray, gray, gray);
		line->setTrans(0, 0, 0);
		line->setLineWidth(2.0);
		line->setRot(0, 0, 0);
		lines.push_back(line);
		lineRoot->addChild(line);
	}
  
	//Create a label for the lines.
	lineLabel = new ofxTextObject(font16, "Press &lsquo;s&rsquo; to animate the lines.");
	lineLabel->setColor(255, 255, 255);
	lineLabel->setAlignment(OF_TEXT_ALIGN_CENTER);
	lineLabel->setPointSize(16);
	lineLabel->setLeading(19);
	lineLabel->setTrans(0, lineLabel->getLeading(), 2.0);
	lineLabel->setColumnWidth(100);
	lineRoot->addChild(lineLabel);
  
  
	
  
}

//--------------------------------------------------------------
void testApp::update(){
  
	//Update the scene with the current time. This call propagates the idle() call to all objects as well.
	//Note: If you are capturing frames to create a movie, simply replace ofGetElapsedTimef() with a float variable that you increment by a fixed time interval each frame.
  scene->update(ofGetElapsedTimef());
}

//--------------------------------------------------------------
void testApp::draw(){
  
	//Call draw on scene, which initiates the drawing of the root object.
  scene->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
  
	//Animate the circles using messages.
  if(key == 'a'){
		//Stop the animation just in case, since it is moving the same circles that you're about to animate with messages.
		animation->stop();
    
		//Animate the circles using messages. These methods allow you to directly tell the object to do things.
		float offset = 0.1;
		for(int i=0; i < circles.size(); i++){
			float start = offset*i;
			float dur = 0.5;
			circles[i]->stopMessages();														//Stop (and remove) all other previously running messages.
			circles[i]->doMessage1f(OF_SCALE, start, dur, OF_EASE_OUT, 1.6);				//Animate the scale to 1.6.
			circles[i]->doMessage1f(OF_SCALE, start + dur, dur, OF_EASE_IN, 1.0);			//Animate the scale back to 1.0.
		}
  }
	//Animate the circles using the animation we made above.
	else if(key == 'A'){
		animation->start();
	}
	//Animate the lines using messages.
	else if(key == 's'){
		if(lines[lines.size()-1]->getRot().z > 10){
			for(int i=0; i < lines.size(); i++){
				lines[i]->stopMessages();
				lines[i]->doMessage3f(OF_ROTATE, 0, 2.0, OF_EASE_OUT, 0, 0, 0);
			}
		}else{
			for(int i=0; i < lines.size(); i++){
				lines[i]->stopMessages();
				lines[i]->doMessage3f(OF_ROTATE, 0, 2.0, OF_EASE_OUT, 0, 0, i/(float)lines.size() * 360);
			}
		}
	}
	//Animate the cropping of the dynamic polygon.
	else if(key == OF_KEY_DOWN){
		float dur = 1.0;
		
		dynamicPolygon->gotoVertexPos(0, ofVec3f(0, 0.3*polyTex->getHeight(), 0), dur);
		dynamicPolygon->gotoVertexTexCoords(0, 0, 0.7*polyTex->getHeight(), dur);
		
		dynamicPolygon->gotoVertexPos(1, ofVec3f(polyTex->getWidth(), 0.3*polyTex->getHeight(), 0), dur);
		dynamicPolygon->gotoVertexTexCoords(1, polyTex->getWidth(), 0.7*polyTex->getHeight(), dur);
		
		dynamicPolygon->gotoVertexPos(2, ofVec3f(polyTex->getWidth(), 0.7*polyTex->getHeight(), 0), dur);
		dynamicPolygon->gotoVertexTexCoords(2, polyTex->getWidth(), 0.3*polyTex->getHeight(), dur);
		
		dynamicPolygon->gotoVertexPos(3, ofVec3f(0, 0.7*polyTex->getHeight(), 0), dur);
		dynamicPolygon->gotoVertexTexCoords(3, 0, 0.3*polyTex->getHeight(), dur);
	}
	else if(key == OF_KEY_UP){
		float dur = 1.0;
		
		dynamicPolygon->gotoVertexPos(0, ofVec3f(0, 0, 0), dur);
		dynamicPolygon->gotoVertexTexCoords(0, 0, polyTex->getHeight(), dur);
		
		dynamicPolygon->gotoVertexPos(1, ofVec3f(polyTex->getWidth(), 0, 0), dur);
		dynamicPolygon->gotoVertexTexCoords(1, polyTex->getWidth(), polyTex->getHeight(), dur);
		
		dynamicPolygon->gotoVertexPos(2, ofVec3f(polyTex->getWidth(), polyTex->getHeight(), 0), dur);
		dynamicPolygon->gotoVertexTexCoords(2, polyTex->getWidth(), 0, dur);
		
		dynamicPolygon->gotoVertexPos(3, ofVec3f(0, polyTex->getHeight(), 0), dur);
		dynamicPolygon->gotoVertexTexCoords(3, 0, 0, dur);
	}
	//Animate the letter text with messages.
	else if(key == 'g'){
		float travel = 100;
		float leaveDuration = 1.0;
		float returnDuration = 0.5;
		for(int i=0; i < letterText->letters.size(); i++){
			ofVec3f letterHome = letterText->letters[i]->home;	//Grab letter home position.
			
			//Stop any previous messages.
			letterText->letters[i]->stopMessages();
			//Leave home.
			letterText->letters[i]->doMessage3f(OF_TRANSLATE, i*0.03, leaveDuration, OF_EASE_OUT, letterHome.x + ofRandom(-travel, travel), letterHome.y + ofRandom(-travel, travel), 0);
			letterText->letters[i]->doMessage1f(OF_SCALE, i*0.03, leaveDuration, OF_EASE_OUT, ofRandom(0.5, 1.8));
			letterText->letters[i]->doMessage3f(OF_ROTATE, i*0.03, leaveDuration, OF_EASE_OUT, ofRandom(-30,30), ofRandom(-30,30), 0);
			letterText->letters[i]->doMessage3f(OF_SETCOLOR, i*0.03, leaveDuration, OF_EASE_OUT, 255, 100, 200);
			//Return home.
			letterText->letters[i]->doMessage3f(OF_TRANSLATE, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, letterHome.x, letterHome.y, letterHome.z);
			letterText->letters[i]->doMessage1f(OF_SCALE, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, 1.0);
			letterText->letters[i]->doMessage3f(OF_ROTATE, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, 0, 0, 0);
			letterText->letters[i]->doMessage3f(OF_SETCOLOR, i*0.03 + leaveDuration, returnDuration, OF_EASE_IN, 255, 255, 255);
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
  
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
  
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  
  
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  
  
}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
  
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
  
}


