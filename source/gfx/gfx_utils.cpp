#include "gfx_utils.h"

#include <string>

namespace gfx_utils
{

  std::string get_filename_from_path(const std::string path)
  {
    auto last_slash = path.find_last_of("/\\");
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
    auto last_dot = path.rfind('.');
    last_dot = last_dot == std::string::npos ? path.size() - last_slash : last_dot - last_slash;
    return path.substr(last_slash, last_dot);
  }

  void shader_library_t::add(const std::string &path)
  {
    assert(false && "i dunno what to do with a path");
    // do i create a parser for a shader
    // to make them in one file
    // so it would be easier to deal with them
    // or do i assume that .gls for example means to load
    // everything that named bedore the .gls ???
  }
  void shader_library_t::add(const std::string &name, gfx::program_t shader)
  {
    assert(!exists(name) && "shader with this name already exists");
    shaders_[name] = shader;
  }
  gfx::program_t shader_library_t::get(const std::string &name)
  {
    return shaders_[name];
  }
  bool shader_library_t::exists(const std::string &name)
  {
    return shaders_.find(name) != shaders_.end();
  }
  void texture_library_t::add(const std::string &path)
  {
    // i will probably have to make it more robust TODO
    gfx::texture_t t = texture_load(path);
    std::string name = get_filename_from_path(path);
    assert(!exists(name) && "file already exists");
    textures_[name] = t;
  }
  void texture_library_t::add(const std::string &name, const gfx::texture_t &texture)
  {
    assert(!exists(name) && "file already exists");
    textures_[name] = texture;
  }
  const gfx::texture_t &texture_library_t::get(const std::string &name)
  {
    return textures_[name];
  }
  bool texture_library_t::exists(const std::string &name)
  {
    return textures_.find(name) != textures_.end();
  }
  gfx::texture_t texture_load(const std::string &path)
  {
    gfx::texture_desc_t desc = {0, 0, 0, gfx::gl_texture_type::texture_2d};

    // TODO switch for channels and formats of texture

    int channels;
    desc.data_ = stbi_load(path.c_str(), (int *)&desc.width_, (int *)&desc.height_, &channels, 0);

    gfx::texture_t t = gfx::texture_ctor(desc, false);

    stbi_image_free(desc.data_);

    return t;
  }

  gfx::texture_t texture_load_cubemap(const std::vector<const char *> &path)
  {
    gfx::texture_desc_t desc = {0, 0, 0, gfx::gl_texture_type::texture_cubemap};
    i32 w, h, c;
    desc.data_ = malloc(6 * sizeof(void *));
    void** data = (void** )desc.data_;
    for (int i = 0; i < 6; i++)
    {
      data[i] = stbi_load(path[i], &w, &h, &c, 0);
      if (data[i] == NULL)
      {
        assert(false);
        gfx::texture_t t = { 0, (gfx::gl_texture_type)0 };
        return t;
      }
    }
    desc.width_ = w;
    desc.height_ = h;
    const gfx::texture_t t = gfx::texture_ctor(desc, false);

    for (int i = 0; i < 6; i++)
    {
      stbi_image_free(data[i]);
    }

    free(desc.data_);

    return t;
  }

  gfx::texture_t texture_load(const char *path)
  {
    int w, h, c;
    void *data = stbi_load(path, &w, &h, &c, 0);
    if (data == NULL)
    {
      assert(false && "failed to load texture");
      gfx::texture_t t =  {0};
      return t;
    }
    gfx::texture_desc_t desc = {static_cast<u32>(w), static_cast<u32>(h), data, gfx::gl_texture_type::texture_cubemap};
    gfx::texture_t t = gfx::texture_ctor(desc);
    stbi_image_free(data);
    return t;
  }

  gfx::program_t program_load(const char *vertex_path, const char *fragment_path)
  {
    std::ifstream vertex_file(vertex_path);
    std::ifstream fragment_file(fragment_path);
    std::string vertex_src, fragment_src;
    {
      std::stringstream ss;
      std::string line;
      while (getline(vertex_file, line))
      {
        ss << line << "\n";
      }
      vertex_src = ss.str();
    }

    {
      std::stringstream ss;
      std::string line;
      while (getline(fragment_file, line))
      {
        ss << line << "\n";
      }
      fragment_src = ss.str();
    }

    gfx::program_t p = gfx::program_ctor({gfx::shader_ctor(vertex_src, GL_VERTEX_SHADER),
                                          gfx::shader_ctor(fragment_src, GL_FRAGMENT_SHADER)});

    return p;
  }
} // namespace gfx_utils