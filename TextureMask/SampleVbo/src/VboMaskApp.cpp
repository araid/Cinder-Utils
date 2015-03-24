#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"

#include "MaskedTexture.h"

using namespace ci;
using namespace ci::app;

class VboMaskApp : public App {
  public:
	void setup();
	void draw();
    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );

    bool  bDebug;
    vec2 mMousePos;
    Rectf mMaskRect;
    Rectf mImageRect;
    
    gl::TextureRef mImageTexture;
    gl::TextureRef mMaskTexture;
    gl::VboMeshRef mMesh;
};

void VboMaskApp::setup()
{
    bDebug = false;
    mMousePos = vec2(0);
    
    // load differently sized image and mask
    gl::Texture::Format format;
    format.enableMipmapping();
    mImageTexture = gl::Texture::create(loadImage(loadResource("image.jpg")), format);
    mMaskTexture  = gl::Texture::create(loadImage(loadResource("mask.png")), format);
    
    // create rectangles to define their size and position
    mImageRect = Rectf(mImageTexture->getBounds()).getCenteredFit(Rectf(-100, -100, 100, 100), true);
    mMaskRect = Rectf(mMaskTexture->getBounds()).getCenteredFit(mImageRect, true); // this will make the mask fit in the image

    // create vbo with the right mesh coordinates so we dont have to calculate them every frame
    mMesh = MaskedTexture::createMaskedTextureMesh(mImageRect, mMaskRect);
    
    // load default shader or assign your own
    MaskedTexture::loadShaders();
    
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
        float index = (float)i / (float)(numImages - 1);
        float rad = 150 * (sin(t * 5 + index * glm::two_pi<float>()) * 0.4 + 1.25);
        float rot = index * glm::two_pi<float>() + t;
        
        gl::pushMatrices();
        gl::translate(mMousePos);
        gl::rotate(rot);
        gl::translate(vec2(0, rad));
        
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


CINDER_APP( VboMaskApp, RendererGl )
