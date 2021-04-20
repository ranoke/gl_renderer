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

  enum class gl_type : u32
  {
    // TODO add more types
    gl_byte,
    gl_ubyte,
    gl_int,
    gl_uint,
    gl_float,
    gl_float2,
    gl_float3,
    gl_float4
  };

  enum class gl_draw_mode : u32
  {
    points,
    lines,
    line_loop,
    line_strip,
    triangles,
    triangle_strip,
    triangle_fan,
    lines_adjacency,
    line_strip_adjaceny,
    triangles_adjacency,
    triangle_strip_adjacency
  };

  enum class gl_texture_type : u32
  {
    texture_2d,
    texture_cubemap
  };

  enum class gl_format : u32
  {
    red, // GL_RED,
    rg, // GL_RG,
    rgb, // GL_RGB,
    bgr, // GL_BGR,
    rgba,// GL_RGBA,
    bgra,// GL_BGRA,
    red_int,// GL_RED_INTEGER,
    rg_int, // GL_RG_INTEGER,
    rgb_int,// GL_RGB_INTEGER,
    bgr_int, // GL_BGR_INTEGER,
    rgba_int,// GL_RGBA_INTEGER,
    bgra_int,// GL_BGRA_INTEGER,
    stencil_id,// GL_STENCIL_INDEX,
    depth_comp,// GL_DEPTH_COMPONENT,
    depth_stencil// GL_DEPTH_STENCIL.
  };


  enum class buffer_type : u32
  {
    none = 0,
    vertex_buffer = 1,
    index_buffer,
    other_buffer // idk, not sure about this
  };

  enum class framebuffer_attachment
  {
    framebuffer_color_attachment = BIT(1), // TODO populate
    framebuffer_depth_attachment = BIT(2)
    //framebuffer_
  };

 

  struct buffer_desc_t
  {
    u64 size_;
    void *data_;
    buffer_type type_ = zero_enum(buffer_type); // dont forget to init this becouse if not it will lead to memory leaks probably :(
  };

  struct buffer_t
  {

    buffer_t() = default;
    buffer_t(u32 buffer, buffer_type type = zero_enum(buffer_type))
        : buffer_(buffer), type_(type)
    {
    }
    buffer_t(const buffer_t& other) = default;
    buffer_t(buffer_t &&other) noexcept
        : buffer_(std::exchange(other.buffer_, 0)), type_(std::exchange(other.type_, zero_enum(buffer_type)))
    {
    }
    buffer_t &operator=(const buffer_t &other)
    {
      memcpy(this, &other, sizeof(buffer_t));
      return *this;
    }
    buffer_t &operator=(buffer_t &&other) noexcept
    {
      buffer_ = std::exchange(other.buffer_, 0);
      type_ = std::exchange(other.type_, zero_enum(buffer_type));
      return *this;
    }

    u32 buffer_ = 0;
    buffer_type type_ = buffer_type::none;
  }; 

  // w,h,data, tex_type, data_type, format, internal_format 
  struct texture_desc_t
  {
    u32 width_;
    u32 height_;
    void *data_;
    gl_texture_type texture_type_ = gl_texture_type::texture_2d;
    gl_type data_type_ = gl_type::gl_ubyte;
    gl_format format_ = gl_format::rgb;
    gl_format internal_format_ = gl_format::rgb;

    // TODO add type and wrapping params
    // also think about adding here a path
  };

  struct texture_t
  {
    u32 texture_;
    gl_texture_type texture_type_;
    u32 width_;
    u32 height_;

    operator u32() { return texture_; }
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

    vertex_buffer_layout_t(const std::initializer_list<vertex_buffer_layout_object_t> &list);
  };

  struct framebuffer_desc_t
  {
    u32 width_;
    u32 height_;
    framebuffer_attachment attachment_ = zero_enum(framebuffer_attachment);
  };

  struct framebuffer_t
  {
    u32 buffer_;
    texture_t texture_;
  };

  using shader_t = u32;
  using program_t = u32;
  //using texture_t = u32;

  using vertex_array_t = u32;

  void init();

  void clear_color(f32 r, f32 g, f32 b);
  void clear(i32 flags);

  vertex_array_t vertex_array_ctor();
  void vertex_array_set_layout(const vertex_buffer_layout_t &layout);

  buffer_t buffer_ctor(const buffer_desc_t &desc);

  shader_t shader_ctor(const std::string &src, u32 type);
  // TODO add shader_ctor with path as an arg
  program_t program_ctor(const std::initializer_list<shader_t> &shaders);
  program_t program_ctor(const std::initializer_list<std::string> &shaders_path);
  texture_t texture_ctor(const texture_desc_t &desc, bool free_data = true);
  framebuffer_t framebuffer_ctor(const framebuffer_desc_t &desc);

  void vertex_array_delete(vertex_array_t vao);
  void buffer_delete(const buffer_t &b);
  void shader_delete(shader_t s);
  void program_delete(program_t p);
  void texture_delete(const texture_t &t);
  void framebuffer_delete(const framebuffer_t &b);

  void bind_vertex_array(vertex_array_t vao);
  void bind_buffer(const buffer_t &b);
  void bind_program(program_t p);
  void bind_texture(const texture_t &t, u32 active_texture);
  void bind_framebuffer(const framebuffer_t &b);

  void draw_elements(gl_draw_mode mode, u32 count, gl_type type, u64 offset);

  void set_uniform_int(program_t program, const char *name, int value);
  void set_uniform_float(program_t program, const char *name, float val);
  void set_uniform_vec3(program_t program, const char *name, const glm::vec3 &val);
  void set_uniform_mat4(program_t program, const char *name, const glm::mat4 &val);

  using enum gl_type;
}

#endif // __GFX_H__
