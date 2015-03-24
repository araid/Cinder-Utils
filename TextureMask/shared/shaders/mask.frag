#version 150

uniform sampler2D       uImageTex;
uniform sampler2D       uMaskTex;

smooth in vec2          vMaskTexCoord;
smooth in vec2          vImageTexCoord;
out vec4                oColor;

void main()
{
    // sample both textures
    vec4 maskCol  = texture( uMaskTex, vMaskTexCoord );
    vec4 imageCol = texture( uImageTex, vImageTexCoord );

    // apply correct blending
    oColor = imageCol;
    oColor.a *= maskCol.r;
}
