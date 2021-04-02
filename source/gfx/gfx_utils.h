#ifndef __GFX_UTILS_H__
#define __GFX_UTILS_H__

#include "gfx.h"
#include "utils/stb_image.h"

#include <cstring>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>

// here will be read shader and so on...
namespace gfx_utils
{

  
  gfx::texture_t texture_load_cubemap(const char** path)
  {
    gfx::texture_desc_t desc = {0, 0, 0, gfx::gl_texture_cubemap};
    i32 w,h,c;
    desc.data_ = new void*[6];

    void** data = (void**)desc.data_;
    for(int i = 0; i < 6; i++)
    {
      data[i] = stbi_load(path[i], &w, &h, &c, 0);
      if(data[i] == NULL)
      {
        assert(false);
        return (gfx::texture_t){0, (gfx::gl_texture_type)0};
      }
    }
    desc.width_ = w;
    desc.height_ = h;
    gfx::texture_t t = gfx::texture_ctor(desc, false);

    for(int i = 0; i < 6; i++)
    {
      stbi_image_free(data[i]);
    }

    delete[] desc.data_;

    return t;
  }

  // loads 2d texture
  gfx::texture_t texture_load(const char* path)
  {
    int w,h,c;
    void* data = stbi_load(path, &w, &h, &c, 0);
    if(data == NULL)
    {
      assert(false && "failed to load texture");
      return (gfx::texture_t){0};
    }
    gfx::texture_desc_t desc = {w, h, data, gfx::gl_texture_cubemap};
    gfx::texture_t t = gfx::texture_ctor(desc);
    stbi_image_free(data);
    return t;
  }

  // shader 
  gfx::program_t program_load(const char* vertex_path, const char* fragment_path)
  {
    std::ifstream vertex_file(vertex_path);
    std::ifstream fragment_file(fragment_path);
    std::string vertex_src, fragment_src;
    {
      std::stringstream ss;
      std::string line;
      while(getline(vertex_file, line))
      {
        ss << line << "\n";
      }
      vertex_src = ss.str();
    }

    {
      std::stringstream ss;
      std::string line;
      while(getline(fragment_file, line))
      {
        ss << line << "\n";
      }
      fragment_src = ss.str();
    }

    gfx::program_t p = gfx::program_ctor({gfx::shader_ctor(vertex_src, GL_VERTEX_SHADER),
                                          gfx::shader_ctor(fragment_src, GL_FRAGMENT_SHADER)});

    return p;
  }

}

#endif // __GFX_UTILS_H__
