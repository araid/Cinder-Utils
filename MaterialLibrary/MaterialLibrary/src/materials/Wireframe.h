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

class Wireframe : public BaseMaterial {
public:
    void setup()
    {
        if (!bLoaded) {
            auto format = gl::GlslProg::Format()
                .vertex( app::loadAsset( "shaders/Wireframe.vert" ) )
                .geometry( app::loadAsset( "shaders/Wireframe.geom" ) )
                .fragment( app::loadAsset( "shaders/Wireframe.frag" ) );
            mShader = gl::GlslProg::create( format );
            mShader->uniform( "uViewportSize", vec2( app::getWindowSize() ) );
            bLoaded = true;
        }
    }
    
    void draw(gl::VboMeshRef mesh)
    {
        gl::ScopedGlslProg shader(mShader);
        
        // We're using alpha blending, so render the back side first.
        gl::ScopedBlendAlpha blendScope;
        gl::ScopedFaceCulling cullScope( true, GL_FRONT );
        gl::ScopedColor colorScope( Color( 0.7f, 0.5f, 0.3f ) );

        mShader->uniform( "uBrightness", 0.5f );
        gl::draw(mesh);
        
        // Now render the front side.
        gl::cullFace( GL_BACK );
        mShader->uniform( "uBrightness", 1.0f );
        gl::draw(mesh);
    }
    
protected:
};