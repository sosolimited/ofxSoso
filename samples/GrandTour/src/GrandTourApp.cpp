#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "soso/Scene.h"
#include "soso/RectangleObject.h"
#include "soso/ImageObject.h"
#include "soso/CircleObject.h"
#include "soso/Animation.h"
#include "soso/LineSegmentObject.h"
#include "soso/TextureObject.h"
#include "soso/DynamicPolygonObject.h"
#include "soso/LetterTextObject.h"

#include "cinder/Text.h"
#include "cinder/Utilities.h"
//#include "ofxTextObject.h"
//#include "ofxLetterTextObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace soso;

class GrandTourApp : public AppNative {
public:
	void prepareSettings( Settings *settings ) override;
	void setup() override;

	// Handle simple user keyboard interaction.
	void keyDown( KeyEvent event ) override;

	void update() override;
	void draw() override;
private:
	shared_ptr<Scene>										scene;
	shared_ptr<ImageObject>							image;
	shared_ptr<DynamicPolygonObject>		dynamicPolygon;
	gl::TextureRef											polyTex;
	shared_ptr<LetterTextObject>				letterText;

	vector<shared_ptr<CircleObject>>			circles;
	vector<shared_ptr<LineSegmentObject>>	lines;

	shared_ptr<Animation>									animation;
};

void GrandTourApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1280, 720 );
}

void GrandTourApp::setup()
{
	// Create a scene.
	// The scene is a scene graph that renders objects added to its root and their children and their children's children and so on.
	// When the render mode of the scene is set to RENDER_ALPHA_DEPTH_SORTED, it handles sorting of both transparent and opaque objects in the z-axis.
	scene = make_shared<Scene>( getWindowWidth(), getWindowHeight() );
	scene->setBackgroundColor( 10, 10, 10 );

	//_________________________________________________________________________________________________________________

	// Create an image with an alpha channel.
	image = make_shared<ImageObject>( getAssetPath("soso_LOGO.png").string() );
	image->setSpecialTransparency( true );
	image->setTrans( getWindowWidth() - image->width - 40, getWindowHeight() - image->height - 40, 5.0f );
	image->setCentered( false );
	scene->getRoot()->addChild( image );

	//_________________________________________________________________________________________________________________

	// Load our fonts.
	Font font16( loadAsset("Arial.ttf"), 16 );
	Font font64( loadAsset("Arial.ttf"), 64 );
	auto textureFont64 = gl::TextureFont::create( font64 );

	// Create a texture object to display some text.
	{
		TextBox box;
		box.setFont( font16 );
		box.setSize( Vec2i( 360, TextBox::GROW ) );
		box.setColor( Color( "white" ) );
		box.setText( "A CinderBlock by Sosolimited" );

		auto texture = gl::Texture::create( box.render() );
		auto text = make_shared<TextureObject>( texture );
		text->setSpecialTransparency( true );
		text->setTrans( image->getTrans() + Vec3f( 10.0f, -texture->getHeight(), 0.0f ) );
		scene->getRoot()->addChild( text );
	}

	{ // Display some non-western scripts.
		TextBox box;
		box.setFont( font16 );
		box.setLigate( true );
		box.setSize( Vec2i( 300, TextBox::GROW ) );
		box.setColor( ColorA( "white" ) );
		string non_western_text = loadString( loadAsset( "non-roman-scripts-utf8.txt" ) );
		box.setText( "Some non-roman text:\n" );
		box.appendText( non_western_text );

		auto label = make_shared<TextureObject>( gl::Texture::create( box.render() ) );
		label->setTrans( Vec3f( getWindowWidth() - 320.0f, 40.0f, 0.0f ) );
		label->setSpecialTransparency( true );
		scene->getRoot()->addChild( label );
	}

	// TODO: Create a letter text object.
	letterText = make_shared<LetterTextObject>( textureFont64, u8"This is a letter text object. Press ‘g’ to animate the letters." );
	letterText->setTrans( 0.0f, 230.0f, 0.0f );
	letterText->setColor( Color::white() );
	scene->getRoot()->addChild( letterText );

	//_________________________________________________________________________________________________________________

	// Create a textured dynamic polygon object.
	// See how it is animated below in keyPressed().
	polyTex = gl::Texture::create( loadImage( loadAsset( "plasticman.jpg" ) ) );

	dynamicPolygon = make_shared<DynamicPolygonObject>(4);
	dynamicPolygon->enableVertexColoring( false );
	dynamicPolygon->setTrans( image->getTrans().x + 10.0f, 320.0f, 0 );
	dynamicPolygon->setScale( 0.5f );
	dynamicPolygon->setTexture( polyTex );

	//Set the home positions and tex coords of the vertices.
	//Note: We start at the lower left corner and move around counter clockwise, as a general practice.
	dynamicPolygon->setVertexPos(0, Vec3f(0, 0.3*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(0, 0, 0.7f );
	//
	dynamicPolygon->setVertexPos(1, Vec3f(polyTex->getWidth(), 0.3*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(1, 1.0f, 0.7f );
	//
	dynamicPolygon->setVertexPos(2, Vec3f(polyTex->getWidth(), 0.7*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(2, 1.0f, 0.3f );
	//
	dynamicPolygon->setVertexPos(3, Vec3f(0, 0.7*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(3, 0, 0.3f );

	scene->getRoot()->addChild( dynamicPolygon );								//Add the polygon to the scene.

	// DW: Do we want to make a wrapper around this text functionality?
	{ // Create a label for the polygon.
		TextBox box;
		box.setColor( Color::white() );
		box.setFont( font16 );
		box.setLigate( true );
		box.setSize( Vec2i( polyTex->getWidth() / 2, TextBox::GROW ) );
		box.setText( "Here is a dynamic polygon. Press UP and DOWN to animate it." );
		auto label = make_shared<TextureObject>( gl::Texture::create( box.render() ) );
		label->setTrans( dynamicPolygon->getTrans() + Vec3f( 0, polyTex->getHeight() * 0.35f + 5, -1.0f ) );
		label->setSpecialTransparency( true );
		scene->getRoot()->addChild( label );
	}

	//_________________________________________________________________________________________________________________

	//Make a root for the circle objects created below and position it.
	auto circle_root = make_shared<Object>();
	circle_root->setTrans( getWindowWidth() / 2, 360.0f, 0);
	scene->getRoot()->addChild(circle_root);

	//Make some circle objects, position them, and add them to the circleRoot object.
	int numCircles = 12;
	for(int i=0; i < numCircles; i++){
		auto circ = make_shared<CircleObject>(120, 100.0);
		circ->setColor(255, 200-i*10, 200);										//Set an object's color.
		circ->setAlpha(100);													//Set an object's transparency independently.
		circ->setTrans(70*cos(2 * M_PI*(float)i/(float)numCircles),				//Set an object's translation.
									 70*sin(2 * M_PI*(float)i/(float)numCircles),
									 -2.0);
		circle_root->addChild(circ);												//Add the object to the scene.
		circles.push_back(circ);
	}

	{ //Create a label for the circles. Notice the use of u8 to denote utf-8 encoding for the string.
		//
		string text = u8"Here are some circles.\nPress ‘a’ to animate them with messages. Press ‘A’ to animate them with an animation.";
		TextBox box;
		box.setColor( Color::white() );
		box.setFont( font16 );
		box.setSize( Vec2i( 200, TextBox::GROW ) );
		box.setText( text );

		auto label = make_shared<TextureObject>( gl::Texture::create( box.render() ) );
		label->setTrans( -100.0f, 38.0f, 2.0f );
		label->setSpecialTransparency( true );
		label->setColor( Color::black() );
		circle_root->addChild( label );
	}

	//Create an animation with the circles.
	//Call tween on the animation to tell it to do something (OF_TRANSLATE, OF_ROTATE, OF_SCALE, OF_SETCOLOR, OF_SETALPHA)
	//from a start to end time, with a specific interpolation (OF_EASE_OUT, OF_EASE_IN, OF_OF_EASE_INOUT).
	//Note: See ofxAnimation for more advanced tweening features.
	animation = make_shared<soso::Animation>();
	float offset = 0.1;
	for(int i=0; i < circles.size(); i++){
		//Grab translation and color for circles, as set above.
		Vec4f		curColor( circles[i]->getColor().r, circles[i]->getColor().g, circles[i]->getColor().b, circles[i]->getColor().a );

		animation->tween(circles[i].get(), OF_SCALE, i*offset, i*offset + 0.5, OF_EASE_OUT, OF_RELATIVE_VAL, 0.5);		//You can pass OF_RELATIVE_VAL as the first animation value to animate from wherever the object is at the time the animation is called
		animation->tween(circles[i].get(), OF_SETCOLOR, i*offset, i*offset + 0.5, OF_EASE_OUT,							//Animate the color over this timeframe with this interpolation,
										 curColor.x, curColor.y, curColor.z, curColor.w,												//starting with this color,
										 0, 255, 0, 255);																			//and ending at this color.
		animation->tween(circles[i].get(), OF_SETCOLOR, i*offset + 0.5, i*offset + 1.0, OF_EASE_OUT,						//Animate the color back...
										 0, 255, 0, 255,
										 curColor.x, curColor.y, curColor.z, curColor.w);
	}

	//_________________________________________________________________________________________________________________

	// Make and lay out some lines. See how they are animated below in keyPressed().
	auto line_root = make_shared<Object>();
	line_root->setTrans( 200, 360, 0 );
	scene->getRoot()->addChild( line_root );

	int numLines = 120;
	for( int i = 0; i < numLines; ++i )
	{
		auto line = make_shared<LineSegmentObject>( 2 );
		line->setVertexPos( 0, 0, 100, 0 );
		line->setVertexPos( 1, 0, 180, 0 );
		float gray = 255 - i/(float)numLines * 200;
		line->setColor( ColorA8u::gray( gray ) );
		line->setTrans( 0, 0, 0 );
		line->setLineWidth( 2.0 );
		line->setRot( 0, 0, 0 );
		lines.push_back( line );
		line_root->addChild( line );
	}

	{ // Create a label for the lines.
		//
		string text = u8"Press ‘s’ to animate the lines.";
		TextBox box;
		box.setColor( Color::white() );
		box.setFont( font16 );
		box.setSize( Vec2i( 100, TextBox::GROW ) );
		box.setText( text );

		auto label = make_shared<TextureObject>( gl::Texture::create( box.render() ) );
		label->setTrans( 0.0f, 20.0f, 2.0f );
		label->setSpecialTransparency( true );
		line_root->addChild( label );
	}
}

void GrandTourApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'a' )
	{ // Animate circles using messages
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
	else if( event.getChar() == 'A' )
	{ // Animate circles using animation we made above.
		animation->start();
	}

	switch ( event.getCode() )
	{
		case KeyEvent::KEY_s:
			if( lines.back()->getRot().z > 10 )
			{
				for( auto &line : lines )
				{
					line->stopMessages();
					line->doMessage3f( OF_ROTATE, 0, 2.0, OF_EASE_OUT, 0, 0, 0 );
				}
			}
			else
			{
				int i = 0;
				for( auto &line : lines )
				{
					line->stopMessages();
					line->doMessage3f( OF_ROTATE, 0, 2.0, OF_EASE_OUT, 0, 0, i/(float)lines.size() * 360 );
					++i;
				}
			}
		break;
		case KeyEvent::KEY_DOWN:
		{
			float dur = 1.0;

			dynamicPolygon->gotoVertexPos(0, Vec3f(0, 0.3*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(0, 0, 0.7, dur);

			dynamicPolygon->gotoVertexPos(1, Vec3f(polyTex->getWidth(), 0.3*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(1, 1.0, 0.7, dur);

			dynamicPolygon->gotoVertexPos(2, Vec3f(polyTex->getWidth(), 0.7*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(2, 1.0, 0.3, dur);

			dynamicPolygon->gotoVertexPos(3, Vec3f(0, 0.7*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(3, 0, 0.3, dur);
		}
		break;
		case KeyEvent::KEY_UP:
		{
			float dur = 1.0;

			dynamicPolygon->gotoVertexPos(0, Vec3f(0, 0, 0), dur);
			dynamicPolygon->gotoVertexTexCoords(0, 0, 1.0, dur);

			dynamicPolygon->gotoVertexPos(1, Vec3f(polyTex->getWidth(), 0, 0), dur);
			dynamicPolygon->gotoVertexTexCoords(1, 1.0, 1.0, dur);

			dynamicPolygon->gotoVertexPos(2, Vec3f(polyTex->getWidth(), polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(2, 1.0, 0.0, dur);

			dynamicPolygon->gotoVertexPos(3, Vec3f(0, polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(3, 0, 0.0, dur);
		}
		break;
		default:
		break;
	}
}

void GrandTourApp::update()
{
	scene->update( getElapsedSeconds() );
}

void GrandTourApp::draw()
{
	// Set origin lower-left, with positive-y up by using final false flage.
	scene->draw();

}

CINDER_APP_NATIVE( GrandTourApp, RendererGl )
