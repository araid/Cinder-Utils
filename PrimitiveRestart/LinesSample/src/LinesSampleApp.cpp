// This example shows an efficient way of drawing
// multiple primitives such as line strips
// all contained in a single vbo mesh
// by using primitive restart

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Batch.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/gl/Shader.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define NUM_PARTICLES 1000
#define TRAIL_LENGTH 15
#define RESTART_INDEX 0xFFFF

class Particle {
public:
    vec2 pos;
    vec2 vel;
    std::vector<vec2> trail;
    
    Particle() {
        pos = vec2(randFloat(getWindowWidth()), randFloat(getWindowHeight()));
        vel = randVec2f() * 5.0f;
        trail.resize(TRAIL_LENGTH, pos);
    }
    
    void update() {
        if (pos.x < 0 || pos.x > getWindowWidth()) {
            vel.x = -vel.x;
        }
        if (pos.y < 0 || pos.y > getWindowHeight()) {
            vel.y = -vel.y;
        }
        pos += vel;
        
        for (int i = 0; i < TRAIL_LENGTH ; i++) {
            trail[i] = trail[i+1];
        }
        trail[TRAIL_LENGTH - 1] = pos;
    }
};

class LinesTestApp : public App {
public:
    void setup() override;
    void update() override;
    void draw() override;
    
    void createVbo();
    
    vector<Particle*>   mParticles;
    gl::VboMeshRef      mTrailsVbo;
    gl::BatchRef        mTrailsBatch;
};

void LinesTestApp::setup()
{    
    // create new particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        mParticles.push_back(new Particle());
    }
    
    // create vbo mesh that will draw the trails
    createVbo();
}

void LinesTestApp::update()
{
    // update particles in cpu
    for (Particle * p : mParticles) p->update();
    
    // update vbo with particle trails
    auto vertPosIter = mTrailsVbo->mapAttrib2f( geom::POSITION );
    for (int i = 0; i < mTrailsVbo->getNumVertices(); i++) {
        int iParticle   = i / TRAIL_LENGTH;
        int iTrail      = i % TRAIL_LENGTH;
        
        *vertPosIter = mParticles[iParticle]->trail[iTrail];
        ++vertPosIter;
    }
    vertPosIter.unmap();
}

void LinesTestApp::draw()
{
    gl::clear( Color( 1, 1, 1 ) );
    gl::color(0, 0, 0);
    
    // Enable primitive restart so
    // OpenGL knows when to start a new line strip
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(RESTART_INDEX);
    
    mTrailsBatch->draw();
}

void LinesTestApp::createVbo()
{
    std::vector<vec2> vertices;
    std::vector<GLushort> indices;
    
    for (int i = 0; i < NUM_PARTICLES * TRAIL_LENGTH; i++) {
        // add vertex
        vertices.push_back(randVec2f() * 500.0f);
        
        // add restart index every TRAIL_LENGTH indices
        if(i % TRAIL_LENGTH == 0) indices.push_back(RESTART_INDEX);  // new line every TRAIL_LENGTH vertices
        
        // add normal index
        indices.push_back(i);
    }
    
    // vbo and batch
    gl::VboMesh::Layout layout;
    layout.usage(GL_DYNAMIC_DRAW).attrib(geom::POSITION, 2);
    
    mTrailsVbo = gl::VboMesh::create( vertices.size(), GL_LINE_STRIP, { layout }, indices.size() );
    mTrailsVbo->bufferAttrib( geom::POSITION, sizeof(vec2) * vertices.size(), vertices.data() );
    mTrailsVbo->bufferIndices( sizeof(GLushort) * indices.size(), indices.data() );
    
    mTrailsBatch = gl::Batch::create( mTrailsVbo, gl::getStockShader( gl::ShaderDef().color() ) );
}

CINDER_APP( LinesTestApp, RendererGl )
