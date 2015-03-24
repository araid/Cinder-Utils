#version 150 core

in vec4 ciPosition;
in vec2 ciTexCoord0;

uniform vec2 uImageSize;
uniform vec4 uMaskToImage;
uniform mat4 ciModelViewProjection;

smooth out vec2 vMaskTexCoord;
smooth out vec2 vImageTexCoord;

void main()
{
    gl_Position = ciModelViewProjection * ciPosition;
    
    // image texture coordinates
    vImageTexCoord = ciTexCoord0;

    // Calculate mask texture coordinates scaling from center
    // this can probably be optimized but like this it's easy to understand
    vMaskTexCoord = vec2(ciTexCoord0.x, 1.0 - ciTexCoord0.y);   // invert y axis
    vMaskTexCoord -= uMaskToImage.xy;                           // apply mask offset
    vMaskTexCoord -= vec2(0.5, 0.5);                            // center image at 0,0
    vMaskTexCoord *= uMaskToImage.zw;                           // apply mask to movie relationship
    vMaskTexCoord += vec2(0.5, 0.5);                            // translate back
    vMaskTexCoord *= uImageSize;                                // scale if our image is ARB - otherwise this should be 1
}




