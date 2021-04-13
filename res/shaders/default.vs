#version 330 core
layout (location = 0) in vec3 a_pos; // the position variable has attribute position 0
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model;

uniform vec3 u_light_pos;

out vec2 uv;
out vec3 normal;

void main()
{
  gl_Position = u_proj*u_view*u_model*vec4(a_pos, 1.0); // see how we directly give a vec3 to vec4's constructor
  uv = a_uv;
  normal = a_normal;
}