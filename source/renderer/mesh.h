#ifndef __MESH_H__
#define __MESH_H__

#include "base/base.h"
#include "base/types.h"

#include "gfx/gfx.h"

#include <string>
#include <cstring>
#include <utility>


namespace renderer
{

  struct mesh_t
  {
    mesh_t()
    {

    }
    mesh_t(const std::string& path, const gfx::buffer_t& vertex_buffer, const gfx::buffer_t& index_buffer, u32 index_count)
      : path_(path)
      , vertex_buffer_(vertex_buffer)
      , index_buffer_(index_buffer)
      , index_count_(index_count)
    {

    }
    virtual ~mesh_t()
    {
      if(vertex_buffer_.type_ != zero_enum(gfx::buffer_type))
        gfx::buffer_delete(vertex_buffer_);
      if(index_buffer_.type_ != zero_enum(gfx::buffer_type))
        gfx::buffer_delete(index_buffer_);

    }
    mesh_t(const mesh_t& other)
    {
      memcpy(this, &other, sizeof(mesh_t));
    }
    mesh_t(mesh_t&& other)
      : path_ (std::move(other.path_))
      , vertex_buffer_(std::move(other.vertex_buffer_))
      , index_buffer_(std::move(other.index_buffer_))
      , index_count_(std::exchange(other.index_count_, 0))
    {
      printf("Move\n");
    }

    mesh_t& operator=(const mesh_t& other) = delete;
    mesh_t& operator=(mesh_t&& other)
    {
      path_ = std::move(other.path_);
      vertex_buffer_ = std::move(other.vertex_buffer_);
      index_buffer_ = std::move(other.index_buffer_);
      index_count_ = std::exchange(other.index_count_, 0);
      return *this;
    }

    

    

    std::string path_; // actually dont know if i need this TODO think
    gfx::buffer_t vertex_buffer_;
    gfx::buffer_t index_buffer_;
    u32 index_count_;
  };

} // namespace renderer

#endif // __MESH_H__