//
//  MaskedTexture.cpp
//  MaskTest
//
//  Created by Adria Navarro on 4/26/14.
//
//

#include "MaskedTexture.h"

std::string MaskedTexture::shaderPath = "/shaders/maskShader/";
std::string MaskedTexture::shaderPathARB = "/shaders/maskShaderARB/";
gl::GlslProg MaskedTexture::mMaskShader;

void MaskedTexture::loadShader(bool ARB)
{
    std::string path = ARB ? shaderPathARB : shaderPath;

    try {
        mMaskShader = gl::GlslProg(app::loadResource(path + "mask_vert.glsl"), app::loadResource(path + "mask_frag.glsl"));
        std::cout << "MaskedTexture shader successfully loaded from " << path << std::endl;
    }
    catch( ci::gl::GlslProgCompileExc &exc ) {
        std::cout << "MaskedTexture: shader compile error: " << std::endl;
        std::cout << exc.what();
    }
    catch( ... ) {
        std::cout << "MaskedTexture: unable to load shader from " << path << std::endl;
    }
}

void MaskedTexture::drawMaskedTexture(gl::Texture texture, gl::Texture mask, Rectf textureRect, Rectf maskRect, bool ARB)
{
    // load shader if needed
    if(!MaskedTexture::mMaskShader) {
        loadShader(ARB);
    }
    
    // calculate mask uv coordinates in the texture rectangle
    float maxX = ARB? (float)mask.getWidth() : 1.0f;
    float maxY = ARB? (float)mask.getHeight() : 1.0f;
    
    Vec2f maskTopLeft;
    maskTopLeft.x = lmap(textureRect.x1, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskTopLeft.y = lmap(textureRect.y1, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    Vec2f maskTopRight;
    maskTopRight.x = lmap(textureRect.x2, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskTopRight.y = lmap(textureRect.y1, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    Vec2f maskBotRight;
    maskBotRight.x = lmap(textureRect.x2, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskBotRight.y = lmap(textureRect.y2, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    Vec2f maskBotLeft;
    maskBotLeft.x = lmap(textureRect.x1, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskBotLeft.y = lmap(textureRect.y2, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    // bin textures and mask shader
    texture.bind(0);
    mask.bind(1);
    
    mMaskShader.bind();
    mMaskShader.uniform("tImage", 0);
    mMaskShader.uniform("tMask", 1);
    
    // draw texture rectangle in direct mode
    glBegin(GL_QUADS);

    if (ARB) {
        glMultiTexCoord2d(GL_TEXTURE0_ARB, 0.0f, 0.0f);
        glMultiTexCoord2d(GL_TEXTURE1_ARB, maskTopLeft.x, maskTopLeft.y);
        glVertex2f(textureRect.getUpperLeft());
        
        glMultiTexCoord2d(GL_TEXTURE0_ARB, texture.getWidth(), 0.0f);
        glMultiTexCoord2d(GL_TEXTURE1_ARB, maskTopRight.x, maskTopRight.y);
        glVertex2f(textureRect.getUpperRight());
        
        glMultiTexCoord2d(GL_TEXTURE0_ARB, texture.getWidth(), texture.getHeight());
        glMultiTexCoord2d(GL_TEXTURE1_ARB, maskBotRight.x, maskBotRight.y);
        glVertex2f(textureRect.getLowerRight());
        
        glMultiTexCoord2d(GL_TEXTURE0_ARB, 0.0f, texture.getHeight());
        glMultiTexCoord2d(GL_TEXTURE1_ARB, maskBotLeft.x, maskBotLeft.y);
        glVertex2f(textureRect.getLowerLeft());

    }
    else {
        glMultiTexCoord2d(GL_TEXTURE0, 0.0f, 0.0f);
        glMultiTexCoord2d(GL_TEXTURE1, maskTopLeft.x, maskTopLeft.y);
        glVertex2f(textureRect.getUpperLeft());
        
        glMultiTexCoord2d(GL_TEXTURE0, 1.0f, 0.0f);
        glMultiTexCoord2d(GL_TEXTURE1, maskTopRight.x, maskTopRight.y);
        glVertex2f(textureRect.getUpperRight());
        
        glMultiTexCoord2d(GL_TEXTURE0, 1.0f, 1.0f);
        glMultiTexCoord2d(GL_TEXTURE1, maskBotRight.x, maskBotRight.y);
        glVertex2f(textureRect.getLowerRight());
        
        glMultiTexCoord2d(GL_TEXTURE0, 0.0f, 1.0f);
        glMultiTexCoord2d(GL_TEXTURE1, maskBotLeft.x, maskBotLeft.y);
        glVertex2f(textureRect.getLowerLeft());
    }
    glEnd();

    // unbind everything
    mMaskShader.unbind();
    mask.unbind();
    texture.unbind();
}


gl::VboMeshRef MaskedTexture::createMaskedTextureMesh(Rectf textureRect, Rectf maskRect, Vec2f textureSize, Vec2f maskSize, bool ARB)
{
    // load shader if needed
    if(!MaskedTexture::mMaskShader) {
        loadShader(ARB);
    }
    
    // calculate mask uv coordinates in the texture rectangle
    float maxX = ARB? maskSize.x : 1.0f;
    float maxY = ARB? maskSize.y : 1.0f;
    
    Vec2f maskTopLeft;
    maskTopLeft.x = lmap(textureRect.x1, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskTopLeft.y = lmap(textureRect.y1, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    Vec2f maskTopRight;
    maskTopRight.x = lmap(textureRect.x2, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskTopRight.y = lmap(textureRect.y1, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    Vec2f maskBotRight;
    maskBotRight.x = lmap(textureRect.x2, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskBotRight.y = lmap(textureRect.y2, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    Vec2f maskBotLeft;
    maskBotLeft.x = lmap(textureRect.x1, maskRect.x1, maskRect.x2, 0.0f, maxX);
    maskBotLeft.y = lmap(textureRect.y2, maskRect.y1, maskRect.y2, 0.0f, maxY);
    
    // create Vbo mesh that will hold the multiple texture coordinates
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
    layout.setStaticNormals();
	layout.setStaticTexCoords2d(0); // should be dynamic in the future
    layout.setStaticTexCoords2d(1); // having a second set of coords causes problems
    
    // save vertex data
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> texCoords;
    std::vector<Vec2f> maskTexCoords;
	std::vector<uint32_t> indices;
    
    vertices.push_back(Vec3f(textureRect.getUpperLeft(), 0.0f));
    vertices.push_back(Vec3f(textureRect.getUpperRight(), 0.0f));
    vertices.push_back(Vec3f(textureRect.getLowerRight(), 0.0f));
    vertices.push_back(Vec3f(textureRect.getLowerLeft(), 0.0f));
    
    normals.push_back(Vec3f::zAxis());
    normals.push_back(Vec3f::zAxis());
    normals.push_back(Vec3f::zAxis());
    normals.push_back(Vec3f::zAxis());
    
    if (ARB) {
        texCoords.push_back(Vec2f(0.0f, 0.0f));
        texCoords.push_back(Vec2f(textureSize.x, 0.0f));
        texCoords.push_back(Vec2f(textureSize.x, textureSize.y));
        texCoords.push_back(Vec2f(0.0f, textureSize.y));
    }
    else {
        texCoords.push_back(Vec2f(0.0f, 0.0f));
        texCoords.push_back(Vec2f(1.0f, 0.0f));
        texCoords.push_back(Vec2f(1.0f, 1.0f));
        texCoords.push_back(Vec2f(0.0f, 1.0f));
    }
    maskTexCoords.push_back(maskTopLeft);
    maskTexCoords.push_back(maskTopRight);
    maskTexCoords.push_back(maskBotRight);
    maskTexCoords.push_back(maskBotLeft);
    
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    
    // create mesh and buffer data
    gl::VboMeshRef mesh = gl::VboMesh::create(4, 4, layout, GL_QUADS);
    
    mesh->bufferPositions(vertices);
    mesh->bufferNormals(normals);
    mesh->bufferTexCoords2d(0, texCoords);
    mesh->bufferTexCoords2d(1, maskTexCoords);
    mesh->bufferIndices(indices);
    
    mesh->unbindBuffers(); // https://forum.libcinder.org/topic/bizarre-vbo-issue
	
    return mesh;
}

void MaskedTexture::drawMaskedTextureMesh(gl::Texture texture, gl::Texture mask, gl::VboMeshRef mesh, bool ARB)
{
    // load shader if needed
    if(!MaskedTexture::mMaskShader) {
        loadShader(ARB);
    }
    
    // bind textures and mask shader
    texture.bind(0);
    mask.bind(1);
    
    mMaskShader.bind();
    mMaskShader.uniform("tImage", 0);
    mMaskShader.uniform("tMask", 1);
 
    // draw mesh
    gl::draw(mesh);

    // set texture back to 0 (cinder bug https://forum.libcinder.org/#Topic/23286000002064071 )
    glEnableClientState(GL_TEXTURE0);
    
    // unbind everything
    mMaskShader.unbind();
    mask.unbind();
    texture.unbind();
}