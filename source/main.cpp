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

  auto vs = gfx::shader_ctor(vertex_src, GL_VERTEX_SHADER);
  auto fs = gfx::shader_ctor(fragment_src, GL_FRAGMENT_SHADER);

  auto p = gfx::program_ctor({vs, fs});


  auto b = gfx::buffer_ctor(desc);

  auto vao = gfx::vertex_array_ctor();
	glBindVertexArray(vao);

      float positions[] = {
          -0.5f, -0.5f, 0,
            0.0f,  0.5f,0,
            0.5f, -0.5f,0
      };

      // Create buffer and copy data
      GLuint buffer;
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positions, GL_STATIC_DRAW);

      // define vertex layout
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
      glEnableVertexAttribArray(0);
  
  while(1)
  {
    gfx::clear_color(0.25f, 0.25f, 0.25f);
    gfx::clear(1);

    glBindBuffer(GL_ARRAY_BUFFER, b.buffer_);
    glUseProgram(p);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    window.update();
  }
  return 0;
}
