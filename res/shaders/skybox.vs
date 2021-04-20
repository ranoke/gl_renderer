#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

out vec3 TexCoords;

uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    TexCoords = aPos;
    gl_Position = u_proj * u_view * vec4(aPos, 1.0);
}  