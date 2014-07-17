uniform sampler2DRect tImage;
uniform sampler2DRect tMask;

void main(void)
{
    vec4 textureColor = texture2DRect( tImage, gl_TexCoord[0].st );
    vec4 maskColor = texture2DRect( tMask, gl_TexCoord[1].st );
    
    gl_FragColor.rgba = textureColor.rgba;
    gl_FragColor.a = maskColor.r;    
}