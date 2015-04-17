//
//  BaseMaterial.h
//  MaterialLibrary
//
//  Created by Adrià Navarro López on 3/24/15.
//
//

#pragma once

#include "BaseMaterial.h"

using namespace ci;

class Rim : public BaseMaterial {
public:
    void setup()
    {
        mShaderPath = "shaders/RimShader";
        BaseMaterial::setup();
    }
    
    virtual void setupParams(params::InterfaceGlRef &params)
    {
        params.reset();
        params = ci::params::InterfaceGl::create( "Basic Rim Shading", ci::ivec2(250, 150) );
        params->setOptions( "", "valueswidth=120 refresh=0.1" );
        params->setPosition(ci::ivec2(20, 150));
        
        params->addParam( "Rim Color", &mRimColor );
        params->addParam( "Object Color", &mObjectColor );
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::ScopedGlslProg shader(mShader);
        mShader->uniform("uRimColor", mRimColor);
        mShader->uniform("uObjectColor", mObjectColor);
        
        gl::draw(mesh);
    }
    
protected:
    Color mRimColor = Color(0.0f, 0.45f, 0.75f);
    Color mObjectColor = Color(0.18, 0.18, 0.2);
};