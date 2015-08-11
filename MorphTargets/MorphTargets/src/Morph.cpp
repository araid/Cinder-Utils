//
//  Morph.cpp
//  MorphTargets
//
//  Created by Adrià Navarro López on 8/11/15.
//
//

#include "Morph.h"

void Morph::clear()
{
    mBatch.reset();
    mShader.reset();
}

void Morph::draw()
{
    if (mBatch) {
        mShader->uniform("uWeights", mWeights, MAX_TARGETS);
        mBatch->draw();
    }
}

void Morph::addMorphTarget(TriMeshRef mesh)
{
    mMeshes.push_back(mesh);
}

void Morph::removeMorphTarget(int i)
{
    if (!mMeshes.empty()) {
        mMeshes.erase(mMeshes.begin() + i);
    }
}

void Morph::setMorphWeights(const std::vector<float> &newWeights)
{
    if (!mShader) loadShader();
    
    // calculate total weight
    int numWeights = glm::min(mMeshes.size(), newWeights.size());
    float totalWeight = 0.0f;
    
    for (int i = 0; i < numWeights; i++) {
        totalWeight += newWeights[i];
    }
    
    if (totalWeight < 1.0f) totalWeight = 1.0f;
    
    // save normalized weights
    for (int i = 0; i < MAX_TARGETS; i++) {
        if (i < numWeights) {
            mWeights[i] = newWeights[i] / totalWeight;
        }
        else {
            mWeights[i] = 0;
        }
    }
}

void Morph::loadShader()
{
    // load main shader
    gl::GlslProg::Format shaderFmt;
    shaderFmt.vertex(app::loadAsset("glsl/morph.vert"));
    shaderFmt.fragment(app::loadAsset("glsl/morph.frag"));
    shaderFmt.attrib(geom::CUSTOM_0, "vPositionMorph0");
    shaderFmt.attrib(geom::CUSTOM_1, "vNormalMorph0");
    shaderFmt.attrib(geom::CUSTOM_2, "vPositionMorph1");
    shaderFmt.attrib(geom::CUSTOM_3, "vNormalMorph1");
    shaderFmt.attrib(geom::CUSTOM_4, "vPositionMorph2");
    shaderFmt.attrib(geom::CUSTOM_5, "vNormalMorph2");
    shaderFmt.attrib(geom::CUSTOM_6, "vPositionMorph3");
    shaderFmt.attrib(geom::CUSTOM_7, "vNormalMorph3");
    shaderFmt.attrib(geom::CUSTOM_8, "vPositionMorph4");
    shaderFmt.attrib(geom::CUSTOM_9, "vNormalMorph4");
    
    mShader = gl::GlslProg::create(shaderFmt);
}

bool Morph::createMorphMesh()
{
    if (mMeshes.empty()) return false;
    
    bool vertexCountOK = true;
    size_t numMeshes = mMeshes.size();
    size_t numVertices = mMeshes.front()->getNumVertices();
    size_t numIndices = mMeshes.front()->getNumIndices();
    size_t numNormals = mMeshes.front()->getNormals().size();
    
    // create mesh layout
    gl::VboMesh::Layout layout;
    layout.usage(GL_STATIC_DRAW);
    layout.attrib(geom::Attrib::POSITION, 3);   // position of base mesh
    layout.attrib(geom::Attrib::NORMAL, 3);     // normals of base mesh
    
    if (mMeshes.front()->hasTexCoords()) {
        layout.attrib(geom::Attrib::TEX_COORD_0, 2);    // normally there's no need to morph tex coords
    }
    
    for (int i = 0; i < (numMeshes - 1); i++) {
        if (mMeshes[i + 1]->getNumVertices() == numVertices &&
            mMeshes[i + 1]->getNormals().size() == numNormals) {
            layout.attrib((geom::Attrib)(geom::Attrib::CUSTOM_0 + i * 2), 3);       // positions of morph target #i
            layout.attrib((geom::Attrib)(geom::Attrib::CUSTOM_0 + i * 2 + 1), 3);   // normals of morph target #i
        }
        else {
            vertexCountOK = false;
        }
    }
    
    if (!vertexCountOK) {
        app::console() << "Morph Error: meshes need the same number of vertices and normals" << std::endl;
    }
    
    // create vbo mesh
    gl::VboMeshRef vbo = gl::VboMesh::create((uint32_t)numVertices,
                                             GL_TRIANGLES,
                                             {layout},
                                             (uint32_t)numIndices,
                                             GL_UNSIGNED_INT);
    
    // buffer attributes for first mesh
    vbo->bufferAttrib(geom::Attrib::POSITION, sizeof(vec3) * numVertices, mMeshes[0]->getPositions<3>());
    vbo->bufferAttrib(geom::Attrib::NORMAL, sizeof(vec3) * numNormals, mMeshes[0]->getNormals().data());
    vbo->bufferIndices(sizeof(uint32_t) * numIndices, mMeshes[0]->getIndices().data());
    
    if (mMeshes[0]->hasTexCoords()) {
        vbo->bufferAttrib(geom::Attrib::TEX_COORD_0, sizeof(vec2) * numVertices, mMeshes[0]->getTexCoords0<2>());
    }
    
    // buffer attributes for morph targets
    for (int i = 1; i < numMeshes; i++) {
        if (mMeshes[i]->getNumVertices() == numVertices &&
            mMeshes[i]->getNormals().size() == numNormals) {
            geom::Attrib attrPos = (geom::Attrib)(geom::Attrib::CUSTOM_0 + (i - 1) * 2);
            geom::Attrib attrNrm = (geom::Attrib)(geom::Attrib::CUSTOM_0 + (i - 1) * 2 + 1);
            vbo->bufferAttrib(attrPos, sizeof(vec3) * numVertices, mMeshes[i]->getPositions<3>());
            vbo->bufferAttrib(attrNrm, sizeof(vec3) * numNormals, mMeshes[i]->getNormals().data());
        }
    }
    
    // create batch
    if (!mShader) loadShader();
    mBatch = gl::Batch::create( vbo, mShader );
    
    return vertexCountOK;
}
