//
//  MaskedTexture.cpp
//  MaskTest
//
//  Created by Adria Navarro on 4/26/14.
//
//

#include "MaskedTexture.h"

gl::GlslProgRef MaskedTexture::mMaskShader;
gl::GlslProgRef MaskedTexture::mMaskPrecalcShader;

void MaskedTexture::loadShaders()
{
    try {
        mMaskShader = gl::GlslProg::create(app::loadResource("/shaders/mask.vert"), app::loadResource("/shaders/mask.frag"));
        mMaskPrecalcShader = gl::GlslProg::create(app::loadResource("/shaders/mask_precalc.vert"), app::loadResource("/shaders/mask.frag"));
        std::cout << "MaskedTexture: shaders loaded successfully" << std::endl;
    }
    catch( ci::gl::GlslProgCompileExc &exc ) {
        std::cout << "MaskedTexture: shader compile error: " << std::endl;
        std::cout << exc.what();
    }
    catch( ... ) {
        std::cout << "MaskedTexture: unable to load shaders" << std::endl;
    }
}

void MaskedTexture::drawMaskedTexture(gl::TextureRef texture, gl::TextureRef mask, Rectf textureRect, Rectf maskRect, bool ARB)
{
    // load shader if needed
    if(!mMaskShader) {
        loadShaders();
    }
    
    // calculate relationship between mask and image to align them - all normalized [0, 1]
    vec2 maskScale = textureRect.getSize() / maskRect.getSize();
    vec2 maskOffset = vec2(maskRect.getCenter() - textureRect.getCenter()) / textureRect.getSize();
    vec4 maskToImage(maskOffset, maskScale);
    
    // bind textures and shader
    gl::ScopedTextureBind maskBind(mask, 0);
    gl::ScopedTextureBind imageBind(texture, 1);
    gl::ScopedGlslProg prog(mMaskShader);
    
    mMaskShader->uniform( "uMaskTex", 0 );
    mMaskShader->uniform( "uImageTex", 1 );
    mMaskShader->uniform( "uImageSize", ARB ? vec2(texture->getSize()) : vec2(1.0f) );
    mMaskShader->uniform( "uMaskToImage", maskToImage);    // supply offset and relative size of the mask
    
    gl::drawSolidRect(textureRect);
}


gl::VboMeshRef MaskedTexture::createMaskedTextureMesh(Rectf textureRect, Rectf maskRect, vec2 textureSize, vec2 maskSize, bool ARB)
{
    // load shader if needed
    if(!mMaskPrecalcShader) {
        loadShaders();
    }
    
    // calculate mask uv coordinates in the texture rectangle
    float maxX = ARB? maskSize.x : 1.0f;
    float maxY = ARB? maskSize.y : 1.0f;
    
    vec2 maskTopLeft;
    maskTopLeft.x = lmap(textureRect.x1, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskTopLeft.y = lmap(textureRect.y1, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    vec2 maskTopRight;
    maskTopRight.x = lmap(textureRect.x2, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskTopRight.y = lmap(textureRect.y1, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    vec2 maskBotRight;
    maskBotRight.x = lmap(textureRect.x2, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskBotRight.y = lmap(textureRect.y2, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    vec2 maskBotLeft;
    maskBotLeft.x = lmap(textureRect.x1, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskBotLeft.y = lmap(textureRect.y2, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    // create Vbo mesh that will hold the multiple texture coordinates
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> texCoords;
    std::vector<vec2> maskTexCoords;
    
    vertices.push_back(vec3(textureRect.getUpperLeft(), 0.0f));
    vertices.push_back(vec3(textureRect.getUpperRight(), 0.0f));
    vertices.push_back(vec3(textureRect.getLowerRight(), 0.0f));
    vertices.push_back(vec3(textureRect.getLowerLeft(), 0.0f));
    
    normals.push_back(vec3(0, 0, 1));
    normals.push_back(vec3(0, 0, 1));
    normals.push_back(vec3(0, 0, 1));
    normals.push_back(vec3(0, 0, 1));
    
    if (ARB) {
        texCoords.push_back(vec2(0.0f, 0.0f));
        texCoords.push_back(vec2(textureSize.x, 0.0f));
        texCoords.push_back(vec2(textureSize.x, textureSize.y));
        texCoords.push_back(vec2(0.0f, textureSize.y));
    }
    else {
        texCoords.push_back(vec2(0.0f, 0.0f));
        texCoords.push_back(vec2(1.0f, 0.0f));
        texCoords.push_back(vec2(1.0f, 1.0f));
        texCoords.push_back(vec2(0.0f, 1.0f));
    }
    maskTexCoords.push_back(maskTopLeft);
    maskTexCoords.push_back(maskTopRight);
    maskTexCoords.push_back(maskBotRight);
    maskTexCoords.push_back(maskBotLeft);
    
    gl::VboMesh::Layout layout;
    layout.usage(GL_STATIC_DRAW).attrib(geom::POSITION, 3).attrib(geom::NORMAL, 2).attrib(geom::TEX_COORD_0, 2).attrib(geom::TEX_COORD_1, 2);
    
    gl::VboMeshRef mesh = gl::VboMesh::create( 4, GL_TRIANGLE_FAN, { layout }, 0 );

    mesh->bufferAttrib( geom::POSITION, sizeof(vec3) * vertices.size(), vertices.data() );
    mesh->bufferAttrib( geom::NORMAL, sizeof(vec2) * normals.size(), normals.data() );
    mesh->bufferAttrib( geom::TEX_COORD_0, sizeof(vec2) * texCoords.size(), texCoords.data() );
    mesh->bufferAttrib( geom::TEX_COORD_1, sizeof(vec2) * maskTexCoords.size(), maskTexCoords.data() );

    return mesh;
}

void MaskedTexture::drawMaskedTextureMesh(gl::TextureRef texture, gl::TextureRef mask, gl::VboMeshRef mesh)
{
    // load shader if needed
    if(!mMaskPrecalcShader) {
        loadShaders();
    }
    
    // bind textures and mask shader
    gl::ScopedTextureBind maskBind(mask, 0);
    gl::ScopedTextureBind imageBind(texture, 1);
    gl::ScopedGlslProg prog(mMaskPrecalcShader);

    mMaskPrecalcShader->uniform( "uMaskTex", 0 );
    mMaskPrecalcShader->uniform( "uImageTex", 1 );
 
    // draw mesh
    gl::draw(mesh);
}