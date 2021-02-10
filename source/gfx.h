#ifndef __GFX_H__
#define __GFX_H__

#include "types.h"
#include <string>
#include <vector>

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

  enum buffer_type
  {
    vertex_buffer,
    index_buffer,
    other_buffer // idk, not sure about this
  };



  struct texture_desc_t
  {
    u32 width_;
    u32 height_;
    void* data_;
    // TODO add type and wrapping params
    // also think about adding here a path 
  };

 

  struct buffer_desc_t
  {
    u64 size_;
    void* data_;
    buffer_type type_;
  };

  
  struct buffer_t
  {
    u32 buffer_;
    buffer_type type_;
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
  using texture_t = u32;
  
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
  texture_t texture_ctor(texture_desc_t desc);

  void buffer_delete(buffer_t b);
  void vertex_array_delete(vertex_array_t vao);
  void shader_delete(shader_t s);
  void program_delete(program_t p);
  void texture_delete(texture_t t);

  void bind_vertex_array(vertex_array_t vao);
  void bind_buffer(buffer_t b);
  void bind_program(program_t p);
  void bind_texture(texture_t t, u32 active_texture);
}




#endif // __GFX_H__
