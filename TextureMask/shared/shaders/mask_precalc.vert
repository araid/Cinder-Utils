#version 150 core

in vec4 ciPosition;
in vec2 ciTexCoord0;
in vec2 ciTexCoord1;

uniform mat4 ciModelViewProjection;

smooth out vec2 vMaskTexCoord;
smooth out vec2 vImageTexCoord;

void main()
{
    gl_Position = ciModelViewProjection * ciPosition;
    
    // pass 2 sets of texture coordinates to fragment
    vImageTexCoord = ciTexCoord0;
    vMaskTexCoord = ciTexCoord1;
}




