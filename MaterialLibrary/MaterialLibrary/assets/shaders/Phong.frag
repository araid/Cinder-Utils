#version 150

// uniforms
uniform vec3		uLightLocViewSpace;
uniform vec3        uDiffuseColor;
uniform vec3        uSpecularColor;
uniform float       uSpecularPower;

// inputs passed from the vertex shader
in vec4		VertexViewSpace;
in vec3		NormalViewSpace;
in vec4		Color;

// fragment color output
out vec4 	oColor;

void main( void )
{
    vec3 normal = normalize(NormalViewSpace);
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
    vec3 vDiffuseColor = fDiffuse * uDiffuseColor;
    vec3 vSpecularColor = fSpecular * uSpecularColor;
    
    // output colors to buffer
    oColor.rgb = (vDiffuseColor + vSpecularColor).rgb;
    oColor.a = 1.0;
}