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
    virtual void setup() = 0;
    virtual void draw(gl::VboMeshRef mesh) = 0;
    virtual void setupParams(params::InterfaceGlRef &params) {};
    
protected:
    bool bLoaded = false;
    gl::GlslProgRef mShader;
    
    virtual void loadShader(std::string path)
    {
        try {
            mShader = gl::GlslProg::create(app::loadAsset(path + ".vert"), app::loadAsset(path + ".frag"));
        }
        catch(Exception e) {
            app::console() << e.what() << std::endl;
        }
    }
};