#include <stdio.h>
#include <iostream>
#include <cstring>
#include "window.h"
#include "gfx.h"

#include <GL/glew.h>

std::string vertex_src = R"(
#version 330 core
layout (location = 0) in vec3 a_pos; // the position variable has attribute position 0
layout (location = 1) in vec2 a_uv;

out vec2 uv;

void main()
{
  gl_Position = vec4(a_pos, 1.0); // see how we directly give a vec3 to vec4's constructor
  uv = a_uv;

}
)";

std::string fragment_src = R"(
#version 330 core
out vec4 frag_color;

uniform sampler2D u_tex;
in vec2 uv;

void main()
{
  frag_color = texture(u_tex, uv);
} 
)";

float vertices[] = {
     1.f,  1.f, 0.0f,  0.0f, 1.0f,// top right
     1.f, -1.f, 0.0f,  0.0f, 0.0f,// bottom right
    -1.f, -1.f, 0.0f,  1.0f, 0.0f,// bottom left
    -1.f,  1.f, 0.0f,  1.0f, 1.0f// top left 
};

u32 indices[] = { 0, 1, 2, 0, 2, 3};

int main()
{
  window_t window("gl_raytracing", 640, 480);
  gfx::init();

  
  auto p = gfx::program_ctor({
    gfx::shader_ctor(vertex_src, GL_VERTEX_SHADER),
    gfx::shader_ctor(fragment_src, GL_FRAGMENT_SHADER)
  });

  
  // deal with buffers
  gfx::buffer_desc_t vdesc = {
    sizeof(vertices),
    vertices,
    gfx::vertex_buffer
  };

  gfx::buffer_desc_t idesc = {
    sizeof(indices),
    indices,
    gfx::index_buffer
  }; 

  auto vao = gfx::vertex_array_ctor();
  auto vbo = gfx::buffer_ctor(vdesc);
  auto ibo = gfx::buffer_ctor(idesc);

  // so i am not sure about this design 
  // maybe i will change it later to something else
  // that requieres less number of lines
  gfx::bind_vertex_array(vao);
  gfx::bind_buffer(vbo);
  gfx::bind_buffer(ibo);
  gfx::vertex_array_set_layout({
    {gfx::gl_float3},
    {gfx::gl_float2}
  });

  // create texture
  gfx::texture_desc_t tdesc = {
    100,
    100,
    new uint8_t[1000*3]
  };
  {
    uint8_t* d = (uint8_t*)tdesc.data_;
    memset(d, 0, sizeof(d));
    d[0] = 255;
    d[1] = 0;
    d[2] = 0;
  }

  auto texture = gfx::texture_ctor(tdesc);

  
  while(1)
  {
    gfx::clear_color(0.25f, 0.25f, 0.25f);
    gfx::clear(1);

    gfx::bind_texture(texture, 0);
    gfx::bind_program(p);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    window.update();
  }
  return 0;
}
