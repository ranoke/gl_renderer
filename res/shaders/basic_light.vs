#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 frag_pos;
out vec3 normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    normal = mat3(transpose(inverse(u_model))) * a_normal;  
    
    gl_Position = u_proj * u_view * vec4(frag_pos, 1.0);
}