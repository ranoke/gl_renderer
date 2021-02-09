#include "gfx.h"
#include <GL/glew.h>

#include <iostream>
#include <cassert>


namespace gfx
{
  u32 gl_buffer_type(buffer_type t);

  void init()
  {
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      assert(false);
      exit(-1);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

  }

  void clear_color(f32 r, f32 g, f32 b)
  {
    glClearColor(r, g, b, 1.f);
  }

  void clear(i32 flags)
  {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  vertex_array_t vertex_array_ctor()
  {
    vertex_array_t vao;
    glGenVertexArrays(1, &vao);
    return vao;
  }

  void vertex_array_set_vbo(vertex_array_t vao, buffer_t vbo, vertex_buffer_layout_t layout)
  {
    
  }
  void vertex_array_set_ibo(vertex_array_t vao, buffer_t ibo)
  {
  }

  buffer_t buffer_ctor(buffer_desc_t desc)
  {
    u32 b;
    glGenBuffers(1, &b);
    u32 type = gl_buffer_type(desc.type_);
    glBindBuffer(type, b);

    // TODO add to buffer_desc_t usage ( GL_DYNAMIC_DRAW, etc )
    glBufferData(type, desc.size_, desc.data_, GL_STATIC_DRAW);


    return {b, desc.type_};
  }


  shader_t shader_ctor(const std::string& src, u32 type)
  {
    shader_t s = glCreateShader(type); 
    const char* src_c = (const char*)src.c_str();
    glShaderSource(s, 1, &src_c, 0);
    glCompileShader(s);

    i32 status = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
      i32 max_length = 0;
      glGetShaderiv(s, GL_INFO_LOG_LENGTH, &max_length);
      char* log = new char[max_length];
      glGetShaderInfoLog(s, max_length, &max_length, log);
      printf("ERROR: shader compile failed: \n");
      printf("%s", log);
      delete[] log;
      assert(false && "shader compile failed");
      shader_delete(s);
      return 0;
    }

    return s;
  }


  program_t program_ctor(const std::initializer_list<shader_t>& shaders)
  {
    program_t p = glCreateProgram();
    for(auto& s : shaders)
      glAttachShader(p, s);

    glLinkProgram(p);

    i32 status = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
      i32 max_length = 0;
      glGetProgramiv(p, GL_INFO_LOG_LENGTH, &max_length);
      char* log = new char[max_length];
      glGetProgramInfoLog(p,  max_length, &max_length, log);
      printf("ERROR: program link failed: \n");
      printf("%s", log);
      delete[] log;
      assert(false && "program link failed");
      program_delete(p);
      // TODO think do i need to delete shaders in case linking failed 
      return 0;
    }

    for(auto& s : shaders)
      glDetachShader(p, s);

    return p;
  }

  program_t program_ctor(const std::initializer_list<std::string>& shaders)
  {
    assert(false); 
    // TODO fill it
    return 0;
  }

  texture_t texture_ctor(texture_desc_t desc)
  {
    texture_t t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, desc.width_, desc.height_, 0, GL_RGB,
                GL_UNSIGNED_BYTE, desc.data_);
    glGenerateMipmap(GL_TEXTURE_2D);

    return t;
  }

  void buffer_delete(buffer_t b)
  {
    glDeleteBuffers(1, &b.buffer_);
  }
  void vertex_array_delete(vertex_array_t vao)
  {
    glDeleteVertexArrays(1, &vao);
  }
  void shader_delete(shader_t s)
  {
    glDeleteShader(s);
  }
  void program_delete(program_t p)
  {
    glDeleteProgram(p);
  }
  void texture_delete(texture_t t)
  {
    glDeleteTextures(1, &t);
  }


  u32 gl_buffer_type(buffer_type t)
  {
    switch(t)
    {
    case vertex_buffer: return GL_ARRAY_BUFFER;
    case index_buffer: return GL_ELEMENT_ARRAY_BUFFER;
      
    }
    assert(false && "failed to get gl buffer type");
    return 0;
  }
  
} // namespace gfx
