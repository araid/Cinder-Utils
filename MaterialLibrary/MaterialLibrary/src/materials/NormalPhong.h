//
//  BaseMaterial.h
//  MaterialLibrary
//
//  Created by Adrià Navarro López on 3/24/15.
//
//

#pragma once

#include "cinder/gl/Texture.h"
#include "BaseMaterial.h"

using namespace ci;

class NormalPhong : public BaseMaterial {
public:
    void setup()
    {
        if (!bLoaded) {
            mShaderPath = "shaders/NormalPhong";
            loadShader();
            mShader->uniform("uDiffuseMap", 0);
            mShader->uniform("uNormalMap", 1);
            
            gl::Texture::Format fmt;
            fmt.mipmap();
            mDiffuseTex = gl::Texture::create(loadImage(app::loadAsset("img/rock_layered.jpg")), fmt);
            mNormalTex  = gl::Texture::create(loadImage(app::loadAsset("img/rock_layered_NRM.png")), fmt);
            mDefaultDiffuseTex = gl::Texture::create(loadImage(app::loadAsset("img/white.png")));
            mDefaultNormalTex  = gl::Texture::create(loadImage(app::loadAsset("img/flatNormal.png")));
            
            bLoaded = true;
        }
    }
    
    virtual void setupParams(params::InterfaceGlRef &params)
    {
        params.reset();
        params = ci::params::InterfaceGl::create( "Phong + Normal Mapping", ci::ivec2(250, 170) );
        params->setOptions( "", "valueswidth=120 refresh=0.1" );
        params->setPosition(ci::ivec2(20, 150));
        
        params->addParam( "Textured", &bDiffuse );
        params->addParam( "Normal mapped", &bNormal );
        params->addParam( "Light Position", &mLightLoc );   // multiply by the camera matrix to make it rotate with it
        params->addParam( "Diffuse Color", &mDiffuseColor );
        params->addParam( "Specular Color", &mSpecularColor );
        params->addParam( "Shininess", &mSpecularPower ).min(0).max(200).step(0.25);
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::TextureRef diffuse = (bDiffuse && mDiffuseTex) ? mDiffuseTex : mDefaultDiffuseTex;
        gl::TextureRef normal  = (bNormal && mNormalTex) ? mNormalTex : mDefaultNormalTex;
        
        gl::ScopedTextureBind diffuseBind(diffuse, 0);
        gl::ScopedTextureBind normalBind(normal, 1);
        gl::ScopedGlslProg shader(mShader);

        mShader->uniform("uLightLocViewSpace", mLightLoc);
        mShader->uniform("uDiffuseColor", mDiffuseColor);
        mShader->uniform("uSpecularColor", mSpecularColor);
        mShader->uniform("uSpecularPower", mSpecularPower);
        
        gl::draw(mesh);
    }
    
protected:
    bool bDiffuse = true;
    bool bNormal = true;
    
    vec3 mLightLoc = vec3(-20, 20, 20);
    Color mDiffuseColor = Color(1, 1, 1);
    Color mSpecularColor = Color(0.5, 0.5, 0.5);
    float mSpecularPower = 8;
    
    gl::TextureRef mDiffuseTex;
    gl::TextureRef mNormalTex;
    gl::TextureRef mDefaultDiffuseTex;
    gl::TextureRef mDefaultNormalTex;
};