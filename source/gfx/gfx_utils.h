#ifndef __GFX_UTILS_H__
#define __GFX_UTILS_H__

#include "gfx.h"

#include "base/base.h"
#include "utils/stb_image.h"

#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include <GL/glew.h>

namespace gfx_utils
{

  template<typename T>
	struct library_t
  {
    using type = T;
    void add(const std::string& path);
    //void add(const std::string& name, type obj);
    void add(const std::string& name, const type& obj); // actually the shader is the program 
    void add(const std::string& name, type&& obj);
    const type& get(const std::string& name);
    bool exists(const std::string& name);
    std::unordered_map<std::string, type> data_;
  };


  // loads just a normal 2d texture
  gfx::texture_t texture_load(const std::string &path);

  //
  //
  // currently every image should be the same size
  // load in this order:
  // right, left, top, bottom, front, back
  gfx::texture_t texture_load_cubemap(const std::vector<const char *> &path);

  gfx::texture_t texture_load_hdri(const std::string& path);

  // loads 2d texture
  gfx::texture_t texture_load(const char *path);

  // shader
  gfx::program_t program_load(const char *vertex_path, const char *fragment_path);


  


}

#endif // __GFX_UTILS_H__
