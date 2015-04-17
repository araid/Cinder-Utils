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

class Phong : public BaseMaterial {
public:
    void setup()
    {
        mShaderPath = "shaders/Phong";
        BaseMaterial::setup();
    }
    
    virtual void setupParams(params::InterfaceGlRef &params)
    {
        params.reset();
        params = ci::params::InterfaceGl::create( "Phong", ci::ivec2(250, 150) );
        params->setOptions( "", "valueswidth=120 refresh=0.1" );
        params->setPosition(ci::ivec2(20, 150));
        
        params->addParam( "Light Position", &mLightLoc );   // multiply by the camera matrix to make it rotate with it
        params->addParam( "Diffuse Color", &mDiffuseColor );
        params->addParam( "Specular Color", &mSpecularColor );
        params->addParam( "Shininess", &mSpecularPower ).min(0).max(200).step(0.25);
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::ScopedGlslProg shader(mShader);
        
        mShader->uniform("uLightLocViewSpace", mLightLoc);
        mShader->uniform("uDiffuseColor", mDiffuseColor);
        mShader->uniform("uSpecularColor", mSpecularColor);
        mShader->uniform("uSpecularPower", mSpecularPower);
        
        gl::draw(mesh);
    }
    
protected:
    vec3 mLightLoc = vec3(-20, 20, 20);
    Color mDiffuseColor = Color(1, 1, 1);
    Color mSpecularColor = Color(0.5, 0.5, 0.5);
    float mSpecularPower = 8;
};