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
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
private:
	shared_ptr<ofxScene>								scene;
	shared_ptr<ofxImageObject>					image;
	shared_ptr<ofxDynamicPolygonObject> dynamicPolygon;

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
	scene = make_shared<ofxScene>( getWindowWidth(), getWindowHeight() );
	scene->setBackgroundColor( 50, 50, 50 );

	image = make_shared<ofxImageObject>( getAssetPath("soso_LOGO.png").string() );
	image->setSpecialTransparency( true );
	image->setTrans( getWindowWidth() - image->width - 40, getWindowHeight() - image->height - 40, 5.0f );
	image->setCentered( false );
	scene->getRoot()->addChild( image.get() );

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
		line->setRot( 0, 0, gray );
		lines.push_back( line );
		lineRoot->addChild( line.get() );
	}
}

void GrandTourApp::mouseDown( MouseEvent event )
{
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
/*
	gl::setMatricesWindowPersp( getWindowSize() );
	gl::translate( Vec2f( 0.0f, 200.0f ) );
	gl::draw( mTexture );
*/
}

CINDER_APP_NATIVE( GrandTourApp, RendererGl )
