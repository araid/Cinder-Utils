//
//  BaseMaterial.h
//  MaterialLibrary
//
//  Created by Adrià Navarro López on 3/24/15.
//
//

#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/VboMesh.h"
#include "cinder/params/Params.h"

using namespace ci;

class BaseMaterial {
public:
    virtual void draw(gl::VboMeshRef mesh) = 0;
    virtual void setupParams(params::InterfaceGlRef &params) {};
    
    virtual void setup()
    {
        if (!bLoaded) {
            loadShader();
            bLoaded = true;
        }
    }
    
    virtual void loadShader()
    {
        try {
            mShader = gl::GlslProg::create(app::loadAsset(mShaderPath + ".vert"), app::loadAsset(mShaderPath + ".frag"));
        }
        catch(Exception e) {
            app::console() << e.what() << std::endl;
        }
    }
    
protected:
    bool bLoaded = false;
    gl::GlslProgRef mShader;
    std::string mShaderPath;
};