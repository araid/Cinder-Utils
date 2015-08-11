#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/CameraUi.h"
#include "cinder/ObjLoader.h"
#include "cinder/params/Params.h"
#include "Morph.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MorphTargetsApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
    
    TriMeshRef loadObj(const fs::path &objPath, const fs::path &mtlPath);

    CameraPersp             mCam;
    CameraUi                mCamUi;
    params::InterfaceGlRef  mParams;
    
    Morph mMorph;
    std::vector<float> mMorphWeights;
};

void MorphTargetsApp::setup()
{
    // create morphs
    // obj files from http://facegen.com/sampleExports.htm
    fs::path mtl = getAssetPath("obj/Jane_hi_neutral.mtl");
    mMorph.addMorphTarget(loadObj(getAssetPath("obj/Jane_hi_neutral.obj"), mtl));
    mMorph.addMorphTarget(loadObj(getAssetPath("obj/Jane_hi_all_ExpressionSmileClosed.obj"), mtl));
    mMorph.addMorphTarget(loadObj(getAssetPath("obj/Jane_hi_all_ExpressionSurprise.obj"), mtl));
    mMorph.addMorphTarget(loadObj(getAssetPath("obj/Jane_hi_all_ExpressionAnger.obj"), mtl));
    mMorph.addMorphTarget(loadObj(getAssetPath("obj/Jane_hi_all_ExpressionDisgust.obj"), mtl));
    mMorph.addMorphTarget(loadObj(getAssetPath("obj/Jane_hi_all_ExpressionFear.obj"), mtl));

    mMorph.createMorphMesh();
    mMorphWeights.resize(Morph::MAX_TARGETS, 0.0f);
    
    // setup camera
    mCam.setPerspective( 60.0f, getWindowAspectRatio(), 0.1, 10000 );
    mCam.setEyePoint(vec3(100, 100, 200));
    mCam.lookAt(vec3(0));
    mCamUi = CameraUi(&mCam, getWindow(), -1);
    
    // setup params
    mParams = params::InterfaceGl::create("Morph Targets", ivec2(200, 200));
    mParams->addParam("Smile",      &mMorphWeights[0]).min(0.0f).max(1.0f).step(0.05f);
    mParams->addParam("Surprise",   &mMorphWeights[1]).min(0.0f).max(1.0f).step(0.05f);
    mParams->addParam("Anger",      &mMorphWeights[2]).min(0.0f).max(1.0f).step(0.05f);
    mParams->addParam("Disgust",    &mMorphWeights[3]).min(0.0f).max(1.0f).step(0.05f);
    mParams->addParam("Fear",       &mMorphWeights[4]).min(0.0f).max(1.0f).step(0.05f);
}

void MorphTargetsApp::update()
{
    mMorph.setMorphWeights(mMorphWeights);
}

void MorphTargetsApp::draw()
{
	gl::clear(Color(0, 0, 0));
    gl::color(1, 1, 1);
    
    gl::ScopedDepth depth(true);
    gl::ScopedMatrices mat;
    gl::setMatrices(mCam);

    mMorph.draw();
    
    mParams->draw();
}

TriMeshRef MorphTargetsApp::loadObj(const fs::path &objPath, const fs::path &mtlPath)
{
    ObjLoader loader(loadFile(objPath), loadFile(mtlPath));
    TriMeshRef mesh = TriMesh::create(loader);
    
    if (! mesh->hasNormals()) {
        mesh->recalculateNormals();
    }
    
    app::console() << "loaded Obj with " << mesh->getNumVertices() << " vertices, "
                    << mesh->getNumTriangles() << " triangles and "
                    << mesh->getNormals().size() << " normals." << std::endl;
    return mesh;
}

CINDER_APP( MorphTargetsApp, RendererGl )
