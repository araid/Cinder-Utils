#version 150

precision highp float;

// inputs from the vertex shader
in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

// output a single color
out vec4 oColor;

void main()
{
    // lambert shading
    const vec3 L = vec3( 0, 0, 1 );
    vec3 N = normalize( Normal );
    oColor = Color * max( 0.0, dot( N, L ));
    oColor.a = 1.0;
}


