#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"

#include "MaskedTexture.h"

using namespace ci;
using namespace ci::app;

class VboMaskApp : public AppNative {
  public:
	void setup();
	void draw();
    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );

    bool  bDebug;
    Vec2f mMousePos;
    Rectf mMaskRect;
    Rectf mImageRect;
    
    gl::Texture mImageTexture;
    gl::Texture mMaskTexture;
    gl::VboMeshRef mMesh;
};

void VboMaskApp::setup()
{
    bDebug = false;
    mMousePos = Vec2f::zero();
    
    // load differently sized image and mask
    gl::Texture::Format format;
    format.enableMipmapping();
    mImageTexture = gl::Texture(loadImage(loadResource("image.jpg")), format);
    mMaskTexture  = gl::Texture(loadImage(loadResource("mask.png")), format);
    
    // create rectangles to define their size and position
    mImageRect = Rectf(mImageTexture.getBounds()).getCenteredFit(Rectf(-100, -100, 100, 100), true);
    mMaskRect = Rectf(mMaskTexture.getBounds()).getCenteredFit(mImageRect, true); // this will make the mask fit in the image

    // create vbo with the right mesh coordinates so we dont have to calculate them every frame
    mMesh = MaskedTexture::createMaskedTextureMesh(mImageRect, mMaskRect);
    
    // load default shader or assign your own
    MaskedTexture::loadShader();
    
    gl::enableAlphaBlending();
}


void VboMaskApp::mouseMove( MouseEvent event )
{
    mMousePos = event.getPos();
}

void VboMaskApp::mouseDown(MouseEvent event)
{
    bDebug = !bDebug;
}

void VboMaskApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    // draw the masked Vbo with the saved texcoords a bunch of times
    int numImages = 10;
    float t = getElapsedSeconds();
    
    for (int i=0; i<numImages; i++) {
        float index = (float)i / (float)numImages;
        float rad = 150 * (sin(t * 5 + index * M_PI * 2) * 0.4 + 1.25);
        float rot = index * 360 + t * 30;
        
        gl::pushMatrices();
        gl::translate(mMousePos);
        gl::rotate(rot);
        gl::translate(Vec2f(0, rad));
        
        // this is the important stuff, all the previous was just to get some nice movement
        MaskedTexture::drawMaskedTextureMesh(mImageTexture, mMaskTexture, mMesh);
        
        if (bDebug) {
            gl::color(0, 1, 1);
            gl::drawStrokedRect(mImageRect);
            gl::color(1, 1, 0);
            gl::drawStrokedRect(mMaskRect);
        }
        gl::popMatrices();
    }
}



CINDER_APP_NATIVE( VboMaskApp, RendererGl )
