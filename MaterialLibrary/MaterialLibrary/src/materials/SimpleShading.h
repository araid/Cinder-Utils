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
        loadShader("simpleshading");
    }
    
    void setupParams(params::InterfaceGlRef params)
    {
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::color(1, 1, 1);
        gl::ScopedGlslProg shader(mShader);
        gl::draw(mesh);
    }
    
protected:
};