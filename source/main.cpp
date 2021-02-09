#include <stdio.h>
#include <iostream>
#include "window.h"
#include "gfx.h"

#include <GL/glew.h>

std::string vertex_src = R"(
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vertexColor; // specify a color output to the fragment shader


void main()
{
  gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
  vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color

}
)";

std::string fragment_src = R"(
#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  


void main()
{
  FragColor = vertexColor;
} 
)";

float vertices[] = {
  -0.5f, -0.5f, 0,
   0.0f,  0.5f, 0,
   0.5f, -0.5f, 0
};

int main()
{
  window_t window("gl_raytracing", 640, 480);
  gfx::init();

  gfx::buffer_desc_t desc = {
    sizeof(vertices),
    vertices,
    gfx::vertex_buffer
  };

  auto p = gfx::program_ctor({
    gfx::shader_ctor(vertex_src, GL_VERTEX_SHADER),
    gfx::shader_ctor(fragment_src, GL_FRAGMENT_SHADER)
  });



  auto vao = gfx::vertex_array_ctor();
  gfx::bind_vertex_array(vao);

  auto b = gfx::buffer_ctor(desc);
  

  gfx::vertex_buffer_layout_t layout({
      {gfx::gl_float3}
  });

  gfx::vertex_array_set_vbo(vao, b, layout);
  
  
  while(1)
  {
    gfx::clear_color(0.25f, 0.25f, 0.25f);
    gfx::clear(1);

    gfx::bind_program(p);
    gfx::bind_buffer(b);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    window.update();
  }
  return 0;
}
