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

class SimpleShading : public BaseMaterial {
public:
    void setup()
    {
        if (!bLoaded) {
            loadShader("shaders/SimpleShading");
            bLoaded = true;
        }
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::ScopedColor colorScope( Color( 1, 1, 1 ) );
        gl::ScopedGlslProg shader(mShader);
        gl::draw(mesh);
    }
    
protected:
};