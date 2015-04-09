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

class ToonAA : public BaseMaterial {
public:
    void setup()
    {
        if (!bLoaded) {
            loadShader("shaders/ToonAA");
            bLoaded = true;
        }
    }
    
    virtual void setupParams(params::InterfaceGlRef &params)
    {
        params.reset();
        params = ci::params::InterfaceGl::create( "Antialiased Toon", ci::ivec2(250, 150) );
        params->setOptions( "", "valueswidth=120 refresh=0.1" );
        params->setPosition(ci::ivec2(20, 150));
        
        params->addParam( "Light Position", &mLightLoc );   // multiply by the camera matrix to make it rotate with it
        params->addParam( "Diffuse Color", &mDiffuseColor );
        params->addParam( "Ambient Color", &mAmbientColor );
        params->addParam( "Specular Color", &mSpecularColor );
        params->addParam( "Shininess", &mShininess ).min(0).max(200).step(0.25);
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::ScopedGlslProg shader(mShader);
        
        mShader->uniform("LightPosition", mLightLoc);
        mShader->uniform("DiffuseMaterial", mDiffuseColor);
        mShader->uniform("AmbientMaterial", mAmbientColor);
        mShader->uniform("SpecularMaterial", mSpecularColor);
        mShader->uniform("Shininess", mShininess);

        
        gl::draw(mesh);
    }
    
protected:
    
    vec3 mLightLoc = vec3(-20, 20, 20);
    Color mDiffuseColor = Color(0, 0.75, 0.75);
    Color mAmbientColor = Color(0.04f, 0.04f, 0.04f);
    Color mSpecularColor = Color(0.5, 0.5, 0.5);
    float mShininess = 50;
};