#version 150

uniform vec3 uObjectColor;
uniform vec3 uRimColor;

in vec4 VertexViewSpace;
in vec3	NormalViewSpace;

out vec4 fragcolor;

void main()
{
    vec3 eye = normalize(-VertexViewSpace.rgb);                   // eye vector
    float vdn = 1.0 - max(dot(eye, NormalViewSpace), 0.0);        // the rim contribution

    fragcolor.a = 1.0;
    fragcolor.rgb = uObjectColor + vec3(smoothstep(0.3, 1.0, vdn)) * uRimColor;
}