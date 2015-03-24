// Port from OpenGL Programming Guide, 8th edition
// Example 2-15 Using glPrimitiveRestartIndex() to Render Multiple Triangle Strips

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/Shader.h"
#include "cinder/MayaCamUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CubeSampleApp : public App {
  public:
	void setup() override;
	void draw() override;
    void mouseDown( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;

    uint32_t mRestartIndex = 0xFFFF;
    MayaCamUI mCam;
    gl::BatchRef mCubeBatch;
};

void CubeSampleApp::setup()
{
    // setup camera
    CameraPersp cam(getWindowWidth(), getWindowHeight(), 60, 0.001, 1000);
    cam.setEyePoint(vec3(5));
    cam.setCenterOfInterestPoint(vec3(0));
    mCam.setCurrentCam(cam);
    
    // 8 corners of a cube, side length 2, centered on the origin
    static const GLfloat cube_positions[] =
    {
        -1.0f, -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f, 1.0f,
        1.0f, -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f, -1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    
    // Color for each vertex
    static const GLfloat cube_colors[] =
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f
    };
    
    // Indices for the triangle strips
    static const GLushort cube_indices[] =
    {
        0, 1, 2, 3, 6, 7, 4, 5,         // First strip
        0xFFFF,                         // <<-- This is the restart index
        2, 6, 0, 4, 1, 5, 3, 7          // Second strip
    };
    
    // Create a Vbo mesh and batch
    int numIndices = 17;
    int numVertices = 8;
    
    gl::VboMesh::Layout cubeLayout;
    cubeLayout.usage(GL_STATIC_DRAW).attrib(geom::POSITION, 4).attrib(geom::COLOR, 4);
    
    gl::VboMeshRef cubeVboMesh = gl::VboMesh::create( numVertices, GL_TRIANGLE_STRIP, { cubeLayout }, numIndices );
    cubeVboMesh->bufferAttrib( geom::POSITION, sizeof(cube_positions), cube_positions );
    cubeVboMesh->bufferAttrib( geom::COLOR, sizeof(cube_colors), cube_colors );
    cubeVboMesh->bufferIndices(sizeof(cube_indices), cube_indices);
    
    mCubeBatch = gl::Batch::create( cubeVboMesh, gl::getStockShader( gl::ShaderDef().color() ) );
}

void CubeSampleApp::mouseDown( MouseEvent event )
{
    mCam.mouseDown( event.getPos() );
}

void CubeSampleApp::mouseDrag( MouseEvent event )
{
    mCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void CubeSampleApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    gl::color(1, 1, 1);
    gl::setMatrices(mCam.getCamera());
    
    // Enable primitive restart and tell openGL the restart index
    // so we know when to start a new triangle strip
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(mRestartIndex);
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    mCubeBatch->draw();
}

CINDER_APP( CubeSampleApp, RendererGl )
