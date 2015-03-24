#version 150

uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

in vec4	ciPosition;
in vec3 ciNormal;
in vec4	ciColor;

out vec4 Color;
out vec3 Normal;

void main( void )
{
    // calculate view space normal (required for lighting)
    Normal = normalize(ciNormalMatrix * ciNormal);
    
    // pass texture color
    Color = ciColor;
    
    // vertex shader must always pass projection space position
    gl_Position = ciModelViewProjection * ciPosition;
}
