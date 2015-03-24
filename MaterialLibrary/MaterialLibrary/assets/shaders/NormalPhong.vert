#version 150

uniform mat4	ciModelView;
uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4		ciPosition;
in vec2		ciTexCoord0;
in vec3		ciNormal;
in vec3     ciTangent;
in vec4		ciColor;

out vec4	Color;
out vec2	TexCoord;

out vec4    VertexViewSpace;
out vec3	NormalViewSpace;
out vec3	TangentViewSpace;
out vec3	BitangentViewSpace;

void main( void )
{
    // calculate view space position (required for lighting)
    VertexViewSpace = ciModelView * ciPosition;
    
    // calculate view space normal (required for lighting & normal mapping)
    NormalViewSpace = normalize(ciNormalMatrix * ciNormal);
    
    // calculate tangent and construct the bitangent (required for normal mapping)
    TangentViewSpace = normalize( ciNormalMatrix * ciTangent );
    BitangentViewSpace = normalize( cross( TangentViewSpace, NormalViewSpace ) );
    
    // pass texture coordinates and color
    TexCoord = ciTexCoord0;
    Color 	 = ciColor;
    
    // vertex shader must always pass projection space position
    gl_Position = ciModelViewProjection * ciPosition;
}
