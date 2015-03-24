#version 150

// uniform textures and parameters
uniform sampler2D   uDiffuseMap, uNormalMap;
uniform vec3		uLightLocViewSpace;
uniform vec3        uDiffuseColor;
uniform vec3        uSpecularColor;
uniform float       uSpecularPower;

// inputs passed from the vertex shader
in vec4		VertexViewSpace;
in vec3		NormalViewSpace, TangentViewSpace, BitangentViewSpace;
in vec2		TexCoord;
in vec4		Color;

// fragment color output
out vec4 	oColor;

void main( void )
{
    // fetch the normal from the normal map
    vec3 vMappedNormal = texture(uNormalMap, TexCoord.st).rgb * 2.0 - 1.0;
    
    // modify it using the normal & tangents from the 3D mesh (normal mapping)
    vec3 normal = normalize((TangentViewSpace * vMappedNormal.x) + (BitangentViewSpace * vMappedNormal.y) + (NormalViewSpace * vMappedNormal.z));
    
    vec3 vToCamera = normalize( -VertexViewSpace.xyz );
    vec3 light = normalize( uLightLocViewSpace - VertexViewSpace.xyz );
    vec3 reflect = normalize(-reflect(light, normal));
    
    // calculate diffuse term
    float fDiffuse = max( dot(normal, light), 0.0 );
    fDiffuse = clamp( fDiffuse, 0.1, 1.0 );
    
    // calculate specular term
    float fSpecular = pow( max( dot(reflect, vToCamera), 0.0), uSpecularPower );
    fSpecular = clamp(fSpecular, 0.0, 1.0);
    
    // calculate final colors
    vec3 vDiffuseColor = fDiffuse * texture( uDiffuseMap, TexCoord ).rgb * uDiffuseColor;
    vec3 vSpecularColor = fSpecular * uSpecularColor;
    
    // output colors to buffer
    oColor.rgb = (vDiffuseColor + vSpecularColor).rgb;
    oColor.a = 1.0f;
}