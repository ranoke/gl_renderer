#include "gfx_utils.h"

#include "base/logger.h"

#include <iostream>
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

  template <class T>
  void library_t<T>::add(const std::string &path)
  {
    assert(false && "i dunno what to do with a path");
    // do i create a parser for a shader
    // to make them in one file
    // so it would be easier to deal with them
    // or do i assume that .gls for example means to load
    // everything that named bedore the .gls ???
  }
  //template<class T>
  //void library_t<T>::add(const std::string& name, library_t<T>::type obj)
  //{
  //  assert(!exists(name) && "obj with this name already exists");
  //  data_[name] = obj;
  //}
  template <class T>
  void library_t<T>::add(const std::string &name, const library_t<T>::type &obj)
  {
    assert(!exists(name) && "obj with this name already exists");
    data_[name] = obj;
  }
  template <class T>
  void library_t<T>::add(const std::string &name, library_t<T>::type &&obj)
  {
    assert(!exists(name) && "obj with this name already exists");
    data_[name] = obj;
  }

  template <class T>
  const T &library_t<T>::get(const std::string &name)
  {
    return data_[name];
  }

  template <class T>
  bool library_t<T>::exists(const std::string &name)
  {
    return data_.find(name) != data_.end();
  }

  gfx::texture_t texture_load(const std::string &path)
  {
    return texture_load(path.c_str());
  }

  gfx::texture_t texture_load_cubemap(const std::vector<const char *> &path)
  {
    gfx::texture_desc_t desc = {0, 0, 0, gfx::gl_texture_type::texture_cubemap};
    i32 w, h, c;
    desc.data_ = malloc(6 * sizeof(void *));
    void **data = (void **)desc.data_;
    for (int i = 0; i < 6; i++)
    {
      data[i] = stbi_load(path[i], &w, &h, &c, 3);
      if (data[i] == NULL)
      {
        assert(false);
        gfx::texture_t t;
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

  gfx::texture_t texture_load_hdri(const std::string &path)
  {
    gfx::texture_desc_t desc = {0, 0, 0,
                                gfx::gl_texture_type::texture_2d,
                                gfx::gl_type::gl_float,
                                gfx::gl_format::rgb, gfx::gl_format::rgb_16f};

    stbi_set_flip_vertically_on_load(true);
    i32 w, h, c;
    float *data = stbi_loadf(path.c_str(), &w, &h, &c, 0);
    desc.data_ = data;
    desc.width_ = w;
    desc.height_ = h;
    gfx::texture_t t;
    if (data)
    {
      t = gfx::texture_ctor(desc, false);
      stbi_image_free(data);
    }
    else
    {
      logger::warn("Failed to load hdri {0}", path);
    }
    stbi_set_flip_vertically_on_load(false);

    return t;
  }

  gfx::texture_t texture_load(const char *path)
  {
    int w, h, c;
    void *data = stbi_load(path, &w, &h, &c, 0);
    if (data == NULL)
    {
      logger::warn("Failed to load texture: {0}", path);

      return {};
    }
    gfx::texture_desc_t desc = {
        static_cast<u32>(w),
        static_cast<u32>(h),
        data,
        gfx::gl_texture_type::texture_2d,

    };
    gfx::texture_t t = gfx::texture_ctor(desc, false);
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

  // template implementation for libraries
  template struct gfx_utils::library_t<gfx::program_t>;
  template struct gfx_utils::library_t<gfx::texture_t>;

} // namespace gfx_utils