#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/MayaCamUI.h"
#include "cinder/GeomIo.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"

#include "SimpleShading.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void prepareSettings( App::Settings* settings );

class MaterialLibraryApp : public App {
  public:
    enum PrimitiveDef  { SPHERE, CUBE, TEAPOT };
    enum MaterialDef   { WIREFRAME, SIMPLE, PHONG };

	void setup() override;
	void update() override;
	void draw() override;
    void mouseDown( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    
    void setupParams();
    void loadGeometry();
    void loadMaterial();
    
    int mPrimitiveSelected, mPrimitiveCurrent;
    int mMaterialSelected, mMaterialCurrent;
    int mSubdivisions;
    
    gl::VboMeshRef          mMesh;
    MayaCamUI               mMayaCam;
    params::InterfaceGlRef	mParams;
    params::InterfaceGlRef	mMaterialParams;
    
    BaseMaterial*           mMaterial;
    std::map<MaterialDef, BaseMaterial*> mMaterials;
};

void prepareSettings( App::Settings* settings )
{
    settings->setWindowSize(1280, 920);
    settings->setHighDensityDisplayEnabled();
}

void MaterialLibraryApp::setup()
{
    // setup geometry
    mSubdivisions = 20;
    mPrimitiveSelected = mPrimitiveCurrent = SPHERE;
    mMaterialSelected = mMaterialCurrent = SIMPLE;
    loadGeometry();
    
    // setup materials
    mMaterials = { { SIMPLE, new SimpleShading() } };
    loadMaterial();
    
    // setup camera
    CameraPersp initialCam;
    initialCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.1, 1000 );
    initialCam.setEyePoint(vec3(0, 0, 5));
    initialCam.setCenterOfInterestPoint(vec3(0));
    mMayaCam.setCurrentCam( initialCam );
    
    // setup gui
    setupParams();
    
    // Enable the depth buffer.
    gl::enableDepthRead();
    gl::enableDepthWrite();
}

void MaterialLibraryApp::setupParams()
{
    vector<string> primitives = { "Sphere", "Cube", "Teapot" };
    vector<string> materials  = { "Wireframe", "Shaded", "Phong" };
    
    mParams = ci::params::InterfaceGl::create( "Material Library", ci::ivec2(200, 200) );
    mParams->setPosition(ci::ivec2(20, 20));
    
    mParams->addParam( "Material", materials, (int*) &mMaterialSelected );
    mParams->addParam( "Primitive", primitives, (int*) &mPrimitiveSelected );
    mParams->addParam( "Subdivisions", &mSubdivisions ).min(1).max(50).step(1)
            .updateFn( [this] { loadGeometry(); } );
}

void MaterialLibraryApp::loadGeometry()
{
    switch( mPrimitiveCurrent ) {
        case SPHERE:    mMesh = gl::VboMesh::create(geom::Sphere().subdivisions(mSubdivisions)); break;
        case CUBE:      mMesh = gl::VboMesh::create(geom::Cube().subdivisions(mSubdivisions)); break;
        case TEAPOT:    mMesh = gl::VboMesh::create(geom::Teapot().subdivisions(mSubdivisions)); break;
    }
}

void MaterialLibraryApp::loadMaterial()
{
    mMaterial = mMaterials.at((MaterialDef) mMaterialCurrent);
    mMaterial->setup();
    mMaterial->setupParams(mMaterialParams);
}

void MaterialLibraryApp::mouseDown( MouseEvent event )
{
    mMayaCam.mouseDown( event.getPos() );
}

void MaterialLibraryApp::mouseDrag( MouseEvent event )
{
    mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void MaterialLibraryApp::update()
{
    if(mPrimitiveCurrent != mPrimitiveSelected) {
        mPrimitiveCurrent = mPrimitiveSelected;
        loadGeometry();
    }
    
    if(mMaterialCurrent != mMaterialSelected) {
        mMaterialCurrent = mMaterialSelected;
        loadMaterial();
    }
}

void MaterialLibraryApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::ScopedMatrices cameraMat;
    gl::setMatrices(mMayaCam.getCamera());
    
    mMaterial->draw(mMesh);
    
    mParams->draw();
}

CINDER_APP( MaterialLibraryApp, RendererGl )
