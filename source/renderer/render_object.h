#ifndef __MESH_H__
#define __MESH_H__

#include "base/base.h"
#include "base/types.h"

#include "gfx/gfx.h"

#include "material.h"

#include <string>
#include <cstring>
#include <utility>


namespace renderer
{

  struct render_object_t
  {
    render_object_t() = default;
    render_object_t(const std::string& path, const gfx::buffer_t& vertex_buffer, const gfx::buffer_t& index_buffer, u32 index_count)
      : path_(path)
      , vertex_buffer_(vertex_buffer)
      , index_buffer_(index_buffer)
      , index_count_(index_count)
    {

    }
    virtual ~render_object_t()
    {
      if(vertex_buffer_.type_ != zero_enum(gfx::buffer_type))
        gfx::buffer_delete(vertex_buffer_);
      if(index_buffer_.type_ != zero_enum(gfx::buffer_type))
        gfx::buffer_delete(index_buffer_);

    }
    // strange things xd TODO
    render_object_t(const render_object_t& other)    
    {
      memcpy(this, &other, sizeof(render_object_t));
      memset((void*)&other, 0, sizeof(render_object_t));
    }
    render_object_t(render_object_t&& other) noexcept = default;
      
    // same here TODO!
    render_object_t& operator=(const render_object_t& other)
    {
      memcpy(this, &other, sizeof(render_object_t));
      memset((void*)&other, 0, sizeof(render_object_t));
      return *this;
    }
    render_object_t& operator=(render_object_t&& other) noexcept = default;


    std::string path_; // actually dont know if i need this TODO think
    gfx::buffer_t vertex_buffer_;
    gfx::buffer_t index_buffer_;
    u32 index_count_;
  };

} // namespace renderer

#endif // __MESH_H__