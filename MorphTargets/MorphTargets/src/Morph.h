//
//  Morph.h
//  MorphTargets
//
//  Created by Adrià Navarro López on 8/11/15.
//
//

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Shader.h"

#pragma once

using namespace ci;

class Morph {
public:
    const static int MAX_TARGETS = 5;

    void clear();
    void draw();
    
    bool createMorphMesh();
    void addMorphTarget(TriMeshRef mesh);
    void removeMorphTarget(int i);
    void setMorphWeights(const std::vector<float> &weights);
    
    int getNumTargets()     { return mMeshes.size(); }
    
protected:
    void loadShader();
   
    gl::BatchRef            mBatch;
    gl::GlslProgRef         mShader;
    std::vector<TriMeshRef> mMeshes;
    
    float mWeights[MAX_TARGETS] = { 0.0 };
};