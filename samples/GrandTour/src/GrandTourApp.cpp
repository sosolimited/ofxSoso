#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GrandTourApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void GrandTourApp::setup()
{
}

void GrandTourApp::mouseDown( MouseEvent event )
{
}

void GrandTourApp::update()
{
}

void GrandTourApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( GrandTourApp, RendererGl )
