#version 330 core
out vec4 frag_color;

uniform sampler2D u_tex;
in vec2 uv;
in vec3 normal;
void main()
{
  vec3 objectColor = vec3(0.8, 0.3, 0.4);

  frag_color = vec4(objectColor, 1.0);
}