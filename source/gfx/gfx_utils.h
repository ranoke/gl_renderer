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

  struct shader_library_t
  {
    void add(const std::string& path);
    void add(const std::string& name, gfx::program_t shader); // actually the shader is the program 
    gfx::program_t get(const std::string& name);
    bool exists(const std::string& name);
    std::unordered_map<std::string, gfx::program_t> shaders_;
  };

  struct texture_library_t
  {
    void add(const std::string& path);
    void add(const std::string& name, const gfx::texture_t& texture); 
    const gfx::texture_t& get(const std::string& name);
    bool exists(const std::string& name);
    std::unordered_map<std::string, gfx::texture_t> textures_;
  };

  // loads just a normal 2d texture
  gfx::texture_t texture_load(const std::string &path);

  //
  //
  // currently every image should be the same size
  // load in this order:
  // right, left, top, bottom, front, back
  gfx::texture_t texture_load_cubemap(const std::vector<const char *> &path);

  // loads 2d texture
  gfx::texture_t texture_load(const char *path);

  // shader
  gfx::program_t program_load(const char *vertex_path, const char *fragment_path);

}

#endif // __GFX_UTILS_H__
