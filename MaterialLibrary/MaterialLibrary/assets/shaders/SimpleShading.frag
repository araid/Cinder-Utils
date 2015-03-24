#version 150

// inputs passed from the vertex shader
in vec3 Normal;
in vec4	Color;

// fragment color output
out vec4 oColor;

void main( void )
{
    vec3 normal = normalize( -Normal );
    float diffuse = max( dot( normal, vec3( 0, 0, -1 ) ), 0 );
    oColor = Color * diffuse;
    oColor.a = 1.0f;
}