#ifndef __GFX_H__
#define __GFX_H__

#include "base/base.h"
#include "base/types.h"

#include <string>
#include <vector>
#include <utility>
#include <cstring>

#include <glm/glm.hpp>


namespace gfx
{


  enum gl_type
  {
    // TODO add more types
    gl_int,
    gl_uint,
    gl_float,
    gl_float2,
    gl_float3,
    gl_float4
  };

  enum gl_draw_mode
  {
    gl_points,
    gl_lines,
    gl_triangles
  };

  enum gl_texture_type
  {
    gl_texture_2d,
    gl_texture_cubemap
  };

  enum buffer_type
  {
    vertex_buffer = 1,
    index_buffer,
    other_buffer // idk, not sure about this
  };


  struct texture_desc_t
  {
    u32 width_;
    u32 height_;
    void* data_;
    gl_texture_type type_;
  
    // TODO add type and wrapping params
    // also think about adding here a path 
  };

  

  struct buffer_desc_t
  {
    u64 size_;
    void* data_;
    buffer_type type_ = zero_enum(buffer_type); // dont forget to init this becouse if not it will lead to memory leaks probably :(
  };

  
  struct buffer_t
  {

    buffer_t(){}
    buffer_t(u32 buffer, buffer_type type = zero_enum(buffer_type))
      : buffer_(buffer)
      , type_(type)
    {
    }
    buffer_t(const buffer_t& other)
    {
      memcpy(this, &other, sizeof(buffer_t));
    }
    buffer_t(buffer_t&& other)
      : buffer_(std::exchange(other.buffer_, 0))
      , type_(std::exchange(other.type_, zero_enum(buffer_type)))
    {}
    buffer_t& operator=(const buffer_t& other)
    {
      memcpy(this, &other, sizeof(buffer_t));
      return *this;
    }
    buffer_t& operator=(buffer_t&& other)
    {
      buffer_ = std::exchange(other.buffer_, 0);
      type_ = std::exchange(other.type_, zero_enum(buffer_type));
      return *this;
    }

    u32 buffer_;
    buffer_type type_ = zero_enum(buffer_type);
  };

  struct texture_t
  {
    u32 texture_;
    gl_texture_type type_;
  };

  struct vertex_buffer_layout_object_t
  {
    gl_type type_;
    u32 count_;
    u32 size_;
    bool normalized_;

    vertex_buffer_layout_object_t(gl_type type, bool normalized = false);
  };

  struct vertex_buffer_layout_t
  {
    std::vector<vertex_buffer_layout_object_t> layout_;
    u32 stride_;

    vertex_buffer_layout_t(std::initializer_list<vertex_buffer_layout_object_t> list);
  };


  using shader_t = u32;
  using program_t = u32;
  //using texture_t = u32;
  
  using vertex_array_t = u32;

  void init(); 

  void clear_color(f32 r, f32 g, f32 b);
  void clear(i32 flags);

  vertex_array_t vertex_array_ctor();
  void vertex_array_set_layout(const vertex_buffer_layout_t& layout);


  buffer_t buffer_ctor(buffer_desc_t desc);

  shader_t shader_ctor(const std::string& src, u32 type);
  // TODO add shader_ctor with path as an arg 
  program_t program_ctor(const std::initializer_list<shader_t>& shaders);
  program_t program_ctor(const std::initializer_list<std::string>& shaders_path);
  texture_t texture_ctor(texture_desc_t desc, bool free_data = true);

  void buffer_delete(buffer_t b);
  void vertex_array_delete(vertex_array_t vao);
  void shader_delete(shader_t s);
  void program_delete(program_t p);
  void texture_delete(texture_t t);

  void bind_vertex_array(vertex_array_t vao);
  void bind_buffer(buffer_t b);
  void bind_program(program_t p);
  void bind_texture(texture_t t, u32 active_texture);

  void draw_elements(gl_draw_mode mode, u64 count, gl_type type, u64 offset);

  void set_uniform_int(program_t program, const char* name, int value);
  void set_uniform_float(program_t program, const char* name, float val);
  void set_uniform_vec3(program_t program, const char* name, const glm::vec3& val);
  void set_uniform_mat4(program_t program, const char* name, const glm::mat4& val);
}




#endif // __GFX_H__
