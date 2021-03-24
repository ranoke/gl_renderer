#ifndef __GFX_UTILS_H__
#define __GFX_UTILS_H__

#include "gfx.h"
#include "utils/stb_image.h"

#include <cstring>

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

}

#endif // __GFX_UTILS_H__
