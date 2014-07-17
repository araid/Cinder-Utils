uniform sampler2D tImage;
uniform sampler2D tMask;

void main(void)
{
    vec4 textureColor = texture2D( tImage, gl_TexCoord[0].st );
    vec4 maskColor = texture2D( tMask, gl_TexCoord[1].st );
    
    gl_FragColor.rgba = textureColor.rgba;
    gl_FragColor.a = maskColor.r;    
}