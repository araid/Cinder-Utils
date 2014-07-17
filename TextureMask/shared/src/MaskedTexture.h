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

using namespace cinder;

class MaskedTexture {
public:

    // This function draws a masked image using the specified rectangles where we want to draw each of them
    // it draws in direct mode and does all the calculations every time
    static void drawMaskedTexture(gl::Texture texture, gl::Texture mask, Rectf textureRect, Rectf maskRect, bool ARB = false);
    
    // This one does the calculations only once and saves them in a Vbo with 2 sets of texture coordinates
    // useful if the rectangles aren't going to change every frame
    static gl::VboMeshRef createMaskedTextureMesh(Rectf textureRect, Rectf maskRect, Vec2f textureSize = Vec2f::zero(), Vec2f maskSize = Vec2f::zero(), bool ARB = false);
    
    // This can be used to draw the mesh created with the previous function, or just a reference if we want
    // to draw it ourselves (i.e. for multiple meshes where we don't want to bind textures/shaders individually)
    static void drawMaskedTextureMesh(gl::Texture texture, gl::Texture mask, gl::VboMeshRef mesh, bool ARB = false); // ARB not implemented here yet
    
    static void loadShader(bool ARB = false);
    
    static std::string shaderPath;
    static std::string shaderPathARB;
    static gl::GlslProg mMaskShader;
};