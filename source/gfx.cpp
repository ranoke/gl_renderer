#include "gfx.h"
#include <GL/glew.h>

#include <iostream>
#include <cassert>


namespace gfx
{
  u32 gl_type_components_count(gl_type t);
  u32 gl_type_get(gl_type t);
  u32 gl_type_size(gl_type t);
  u32 gl_buffer_type(buffer_type t);
  

  vertex_buffer_layout_object_t::vertex_buffer_layout_object_t(gl_type type, bool normalized)
    : type_(type)
    , count_(gl_type_components_count(type))
    , size_(gl_type_size(type))
    , normalized_(normalized)
  {
  }
  
  vertex_buffer_layout_t::vertex_buffer_layout_t(std::initializer_list<vertex_buffer_layout_object_t> list)
    : layout_(list)
  {
    // and here calc stride
    stride_ = 0;
    for(auto& i : list)
    {
      stride_ += i.size_;
    }
  }

  void init()
  {
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      assert(false);
      exit(-1);
    }
    i32 minor, major;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    fprintf(stdout, "Status: Using OpenGL %d.%d\n", major, minor);

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

  // make sure u binded vao and vbo before
  void vertex_array_set_layout(const vertex_buffer_layout_t& layout)
  {
    // TODO after enabling warnings add push pop warning disabler
    int a = 0;
    u32 offset = 0;
    for(auto& i : layout.layout_)
    {
      glVertexAttribPointer(
        a,
        i.count_,
        gl_type_get(i.type_),
        i.normalized_, 
        layout.stride_, 
        (const void*)offset
      );
      glEnableVertexAttribArray(a);
      offset += i.size_;
      a++;
    }
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
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

  void bind_vertex_array(vertex_array_t vao)
  {
    glBindVertexArray(vao);
  }

  void bind_buffer(buffer_t b)
  {
    u32 type = gl_buffer_type(b.type_);
    glBindBuffer(type, b.buffer_);
  }

  void bind_program(program_t p)
  {
    glUseProgram(p);
  }

  void bind_texture(texture_t t, u32 active_texture)
  {
    // TODO maybe create struct for texture so 
    // we know the type of it
    glActiveTexture(GL_TEXTURE0 + active_texture);
    glBindTexture(GL_TEXTURE_2D, t);
  }

  u32 gl_type_components_count(gl_type t)
  {
    // TODO add more types
    switch(t)
    {
      case gl_int: return 1;
      case gl_uint: return 1;
      case gl_float: return 1;
      case gl_float2: return 2;
      case gl_float3: return 3;
      case gl_float4: return 4;
      default: return 4; // The initial value is 4. (as in the opengl docs)
    }
    assert(false);
    return 0;
  }

  u32 gl_type_get(gl_type t)
  {
    // TODO add more types
    switch(t)
    {
      case gl_int: return GL_INT;
      case gl_uint: return GL_UNSIGNED_INT;
      default: return GL_FLOAT; // The initial value is GL_FLOAT. (as in the opengl docs)
      
    }
    assert(false);
    return 0;
  }

  u32 gl_type_size(gl_type t)
  {
    // TODO add more types
    switch(t)
    {
      case gl_int: return 1*4;
      case gl_uint: return 1*4;
      case gl_float: return 1*4;
      case gl_float2: return 2*4;
      case gl_float3: return 3*4;
      case gl_float4: return 4*4;
    }
    assert(false);
    return 0;

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
