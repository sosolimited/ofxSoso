#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "ofxScene.h"
#include "ofxRectangleObject.h"
#include "ofxImageObject.h"
#include "ofxCircleObject.h"
#include "ofxAnimation.h"
#include "ofxLineSegmentObject.h"
#include "ofxDynamicPolygonObject.h"
//#include "ofxTextObject.h"
//#include "ofxLetterTextObject.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GrandTourApp : public AppNative {
public:
	void prepareSettings( Settings *settings ) override;
	void setup() override;

	// Handle simple user keyboard interaction.
	void keyDown( KeyEvent event ) override;

	void update() override;
	void draw() override;
private:
	shared_ptr<ofxScene>								scene;
	shared_ptr<ofxImageObject>					image;
	shared_ptr<ofxDynamicPolygonObject> dynamicPolygon;
	gl::TextureRef											polyTex;

	shared_ptr<ofxObject>											circleRoot, lineRoot;
	vector<shared_ptr<ofxCircleObject>>				circles;
	vector<shared_ptr<ofxLineSegmentObject>>	lines;

	shared_ptr<ofxAnimation>									animation;
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
	scene = make_shared<ofxScene>( getWindowWidth(), getWindowHeight() );
	scene->setBackgroundColor( 50, 50, 50 );

	//_________________________________________________________________________________________________________________

	// Create an image with an alpha channel.
	image = make_shared<ofxImageObject>( getAssetPath("soso_LOGO.png").string() );
	image->setSpecialTransparency( true );
	image->setTrans( getWindowWidth() - image->width - 40, getWindowHeight() - image->height - 40, 5.0f );
	image->setCentered( false );
	scene->getRoot()->addChild( image.get() );

	//_________________________________________________________________________________________________________________

	// Text will go here

	//_________________________________________________________________________________________________________________

	// Create a textured dynamic polygon object.
	// See how it is animated below in keyPressed().
	polyTex = gl::Texture::create( loadImage( loadAsset( "plasticman.jpg" ) ) );

	dynamicPolygon = make_shared<ofxDynamicPolygonObject>(4);
	dynamicPolygon->enableVertexColoring( false );
	dynamicPolygon->setTrans( 250, 70, 0 );
	dynamicPolygon->setScale( 0.5f );
	dynamicPolygon->setTexture( polyTex );

	//Set the home positions and tex coords of the vertices.
	//Note: We start at the lower left corner and move around counter clockwise, as a general practice.
	dynamicPolygon->setVertexPos(0, Vec3f(0, 0.3*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(0, 0, 0.3f );
	//
	dynamicPolygon->setVertexPos(1, Vec3f(polyTex->getWidth(), 0.3*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(1, 1.0f, 0.3f );
	//
	dynamicPolygon->setVertexPos(2, Vec3f(polyTex->getWidth(), 0.7*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(2, 1.0f, 0.7f );
	//
	dynamicPolygon->setVertexPos(3, Vec3f(0, 0.7*polyTex->getHeight(), 0));
	dynamicPolygon->setVertexTexCoords(3, 0, 0.7f );

	scene->getRoot()->addChild( dynamicPolygon.get() );								//Add the polygon to the scene.

	//_________________________________________________________________________________________________________________

	// Make and lay out some lines. See how they are animated below in keyPressed().
	lineRoot = make_shared<ofxObject>();
	lineRoot->setTrans( 200, 200, 0 );
	scene->getRoot()->addChild( lineRoot.get() );

	int numLines = 120;
	for( int i = 0; i < numLines; ++i )
	{
		auto line = make_shared<ofxLineSegmentObject>( 2 );
		line->setVertexPos( 0, 0, 100, 0 );
		line->setVertexPos( 1, 0, 180, 0 );
		float gray = 255 - i/(float)numLines * 200;
		line->setColor( ColorA8u::gray( gray ) );
		line->setTrans( 0, 0, 0 );
		line->setLineWidth( 2.0 );
		line->setRot( 0, 0, 0 );
		lines.push_back( line );
		lineRoot->addChild( line.get() );
	}

	// Create a label for the lines.
}

void GrandTourApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'a' )
	{
		// animate circles using messages
	}
	else if( event.getChar() == 'A' )
	{
		// animate circles using animation
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
			dynamicPolygon->gotoVertexTexCoords(0, 0, 0.3, dur);

			dynamicPolygon->gotoVertexPos(1, Vec3f(polyTex->getWidth(), 0.3*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(1, 1.0, 0.3, dur);

			dynamicPolygon->gotoVertexPos(2, Vec3f(polyTex->getWidth(), 0.7*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(2, 1.0, 0.7, dur);

			dynamicPolygon->gotoVertexPos(3, Vec3f(0, 0.7*polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(3, 0, 0.7, dur);
		}
		break;
		case KeyEvent::KEY_UP:
		{
			float dur = 1.0;

			dynamicPolygon->gotoVertexPos(0, Vec3f(0, 0, 0), dur);
			dynamicPolygon->gotoVertexTexCoords(0, 0, 0.0, dur);

			dynamicPolygon->gotoVertexPos(1, Vec3f(polyTex->getWidth(), 0, 0), dur);
			dynamicPolygon->gotoVertexTexCoords(1, 1.0, 0.0, dur);

			dynamicPolygon->gotoVertexPos(2, Vec3f(polyTex->getWidth(), polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(2, 1.0, 1.0, dur);

			dynamicPolygon->gotoVertexPos(3, Vec3f(0, polyTex->getHeight(), 0), dur);
			dynamicPolygon->gotoVertexTexCoords(3, 0, 1.0, dur);
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
	// clear out the window with black
	gl::setMatricesWindowPersp( getWindowSize() );
	gl::setViewport( getWindowBounds() );

	scene->draw();

}

CINDER_APP_NATIVE( GrandTourApp, RendererGl )
