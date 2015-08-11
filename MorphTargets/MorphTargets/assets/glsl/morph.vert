#version 150

// normalized morph weights
uniform float uWeights[5];

// cinder tarnsformation uniforms
uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;

// morph target ins
in vec3 vPositionMorph0;
in vec3 vPositionMorph1;
in vec3 vPositionMorph2;
in vec3 vPositionMorph3;
in vec3 vPositionMorph4;
in vec3 vNormalMorph0;
in vec3 vNormalMorph1;
in vec3 vNormalMorph2;
in vec3 vNormalMorph3;
in vec3 vNormalMorph4;

// vertex data
in vec4 ciPosition;
in vec2 ciTexCoord0;
in vec3 ciNormal;
in vec4 ciColor;

// output to pass to fragment
out vec2 TexCoord;
out vec4 Position;
out vec3 Normal;
out vec4 Color;

void main( void )
{
    // interpolate vertex and normal
    vec4 morphedPos = ciPosition;
    morphedPos.xyz += ( vPositionMorph0 - ciPosition.xyz ) * uWeights[0];
    morphedPos.xyz += ( vPositionMorph1 - ciPosition.xyz ) * uWeights[1];
    morphedPos.xyz += ( vPositionMorph2 - ciPosition.xyz ) * uWeights[2];
    morphedPos.xyz += ( vPositionMorph3 - ciPosition.xyz ) * uWeights[3];
    morphedPos.xyz += ( vPositionMorph4 - ciPosition.xyz ) * uWeights[4];
    morphedPos.w = 1.0;
    
    vec3 norphedNorm = ciNormal;
    norphedNorm += ( vNormalMorph0 - ciNormal ) * uWeights[0];
    norphedNorm += ( vNormalMorph1 - ciNormal ) * uWeights[1];
    norphedNorm += ( vNormalMorph2 - ciNormal ) * uWeights[2];
    norphedNorm += ( vNormalMorph3 - ciNormal ) * uWeights[3];
    norphedNorm += ( vNormalMorph4 - ciNormal ) * uWeights[4];
    
    // calculate view space position (required for lighting)
    Position = ciModelView * morphedPos;
    
    // calculate view space normal (required for lighting & normal mapping)
    Normal = normalize(ciNormalMatrix * norphedNorm);
    
    // pass texture coordinates
    TexCoord = ciTexCoord0;
    Color = ciColor;
    
    // vertex shader must always pass projection space position
    gl_Position = ciModelViewProjection * morphedPos;
}
