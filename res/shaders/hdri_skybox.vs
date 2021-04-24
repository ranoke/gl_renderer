#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

out vec3 local_pos;

uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
  local_pos = a_pos;
  gl_Position = u_proj * u_view * vec4(local_pos, 1.0);
}  