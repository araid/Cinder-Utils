//
//  MaskedTexture.h
//  MaskTest
//
//  Created by Adria Navarro on 4/26/14.
//
//

#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/VboMesh.h"

using namespace cinder;

class MaskedTexture {
public:

    // This function draws a masked image using the specified rectangles where we want to draw each of them
    // it draws in direct mode and does all the calculations every time
    static void drawMaskedTexture(gl::TextureRef texture, gl::TextureRef mask, Rectf textureRect, Rectf maskRect, bool ARB = false);
    
    // This one does the calculations only once and saves them in a Vbo with 2 sets of texture coordinates
    // useful if the rectangles aren't going to change every frame
    static gl::VboMeshRef createMaskedTextureMesh(Rectf textureRect, Rectf maskRect, vec2 textureSize = vec2(0), vec2 maskSize = vec2(0), bool ARB = false);
    
    // This can be used to draw the mesh created with the previous function, or just a reference if we want
    // to draw it ourselves (i.e. for multiple meshes where we don't want to bind textures/shaders individually)
    static void drawMaskedTextureMesh(gl::TextureRef texture, gl::TextureRef mask, gl::VboMeshRef mesh);
    
    static void loadShaders();
    
    static gl::GlslProgRef mMaskShader;
    static gl::GlslProgRef mMaskPrecalcShader;
};