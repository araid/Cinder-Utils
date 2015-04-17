#version 150

uniform mat4	ciModelView;
uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4		ciPosition;
in vec3		ciNormal;

out vec4    VertexViewSpace;
out vec3	NormalViewSpace;

void main( void )
{
    // calculate view space position (required for lighting)
    VertexViewSpace = ciModelView * ciPosition;
    
    // calculate view space normal (required for lighting)
    NormalViewSpace = normalize(ciNormalMatrix * ciNormal);
    
    // vertex shader must always pass projection space position
    gl_Position = ciModelViewProjection * ciPosition;
}
