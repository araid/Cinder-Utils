#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "MaskedTexture.h"

using namespace ci;
using namespace ci::app;

class AnimatedMaskApp : public App {
  public:
	void setup();
	void draw();
    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );

    bool bDebug;
    vec2 mMousePos;
    Rectf mMaskRect;
    Rectf mImageRect;
    
    gl::TextureRef mImageTexture;
    gl::TextureRef mMaskTexture;
};

void AnimatedMaskApp::setup()
{
    bDebug = true;
    mMousePos = vec2(0);
    
    // load differently sized image and mask
    mImageTexture = gl::Texture::create(loadImage(loadResource("image.jpg")));
    mMaskTexture  = gl::Texture::create(loadImage(loadResource("mask.png")));
    
    // create rectangles to define their size and position
    mImageRect = Rectf(mImageTexture->getBounds()).getCenteredFit(getWindowBounds(), true);
    mMaskRect = Rectf(mMaskTexture->getBounds()).getCenteredFit(mImageRect, true); // this will make the mask fit in the image
    
    gl::enableAlphaBlending();
}


void AnimatedMaskApp::mouseMove( MouseEvent event )
{
    mMousePos = event.getPos();
}

void AnimatedMaskApp::mouseDown(MouseEvent event)
{
    bDebug = !bDebug;
}

void AnimatedMaskApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    // animate mask rectangle
    Rectf newMaskRect =  mMaskRect.scaledCentered(sin(getElapsedSeconds())) + mMousePos - getWindowCenter();
    
    // draw in direct mode passing both textures and rectangles (new texture coords will be calculated every frame)
    MaskedTexture::drawMaskedTexture(mImageTexture, mMaskTexture, mImageRect, newMaskRect);
    
    // draw rectangles for debug
    if (bDebug) {
        gl::color(0, 1, 1);
        gl::drawStrokedRect(mImageRect);
    
        gl::color(1, 1, 0);
        gl::drawStrokedRect(newMaskRect);
    }
}

CINDER_APP( AnimatedMaskApp, RendererGl )
