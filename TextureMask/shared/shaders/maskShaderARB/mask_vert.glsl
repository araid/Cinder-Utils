
void main(void)
{
	// pass the texture coordinates of the vertex to the rasterizer (fragment shader)
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;

	// transform the vertex from object space to '2D space' and pass it
	gl_Position = ftransform();
    
}