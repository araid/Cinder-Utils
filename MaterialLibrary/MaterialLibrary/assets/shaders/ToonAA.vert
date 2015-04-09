// based on http://prideout.net/blog/?p=22

#version 150

in vec4	ciPosition;
in vec3	ciNormal;

uniform mat4 ciModelView;
uniform mat4 ciModelViewProjection;
uniform mat3 ciNormalMatrix;
uniform vec3 DiffuseMaterial;

out vec3 EyespaceNormal;
out vec3 Diffuse;

void main()
{
    Diffuse = DiffuseMaterial;
    EyespaceNormal = ciNormalMatrix * ciNormal;
    gl_Position = ciModelViewProjection * ciPosition;
}
