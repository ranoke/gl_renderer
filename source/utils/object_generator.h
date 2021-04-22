#ifndef __MESH_GENERATOR_H__
#define __MESH_GENERATOR_H__

#include "base/base.h"
#include "base/types.h"
#include "renderer/render_object.h"
#include "stb_perlin.h"

#include <glm/glm.hpp>

#include <vector>

template <typename D>
void add(std::vector<f32> &v, D &a);

template <>
void add<glm::vec3>(std::vector<f32> &v, glm::vec3 &a)
{
  v.push_back(a.x);
  v.push_back(a.y);
  v.push_back(a.z);
}

template <>
void add<glm::vec4>(std::vector<f32> &v, glm::vec4 &a)
{
  v.push_back(a.x);
  v.push_back(a.y);
  v.push_back(a.z);
  v.push_back(a.w);
}

// not quite a mesh i guess
renderer::render_object_t
generate_terrain(i32 width, i32 height, glm::vec3 scale, f32 octaves = 8.0f, f32 lacunarity = 2.0f, f32 gain = 0.5f)
{
  f32 scale_x = scale.x;
  f32 scale_y = scale.y;
  f32 scale_z = scale.z;
  std::vector<std::vector<f32>> heightmap;
  heightmap.resize(height);
  for (auto& v : heightmap)
    v.resize(width);

  glm::vec2 dim = glm::vec2(height, width);

  auto seed = rand();

  f32 min = 0.0, max = 0.0;
  for (i32 i = 0; i < dim.x; i++)
  {
    for (i32 j = 0; j < dim.y; j++)
    {
      heightmap[i][j] = stb_perlin_fbm_noise3(i * (1.0f / dim.x), j * (1.0f / dim.y), 0, lacunarity, gain, static_cast<i32>(octaves));
      if (heightmap[i][j] > max)
        max = heightmap[i][j];
      if (heightmap[i][j] < min)
        min = heightmap[i][j];
    }
  }

  //Normalize
  for (i32 i = 0; i < dim.x; i++)
  {
    for (i32 j = 0; j < dim.y; j++)
    {
      heightmap[i][j] = (heightmap[i][j] - min) / (max - min);
    }
  }

  std::vector<u32> indices;
  std::vector<f32> positions;

  //Loop over all positions and add the triangles!
  for (u64 i = 0; i < dim.x - 1; i++)
  {
    for (u64 j = 0; j < dim.y - 1; j++)
    {

      //Add to Position Vector
      glm::vec3 a = glm::vec3(scale_x * i, scale_y * heightmap[i][j], scale_z * j);
      glm::vec3 b = glm::vec3(scale_x * (i + 1), scale_y * heightmap[i + 1][j], scale_z * j);
      glm::vec3 c = glm::vec3(scale_x * i, scale_y * heightmap[i][j + 1], scale_z * (j + 1));
      glm::vec3 d = glm::vec3(scale_x * (i + 1), scale_y * heightmap[i + 1][j + 1], scale_z * (j + 1));

      //UPPER TRIANGLE

      //Add Indices
      indices.push_back(static_cast<u32>(positions.size() / 3 + 0));
      indices.push_back(static_cast<u32>(positions.size() / 3 + 1));
      indices.push_back(static_cast<u32>(positions.size() / 3 + 2));
      indices.push_back(static_cast<u32>(positions.size() / 3 + 0));

      add(positions, a);
      add(positions, b);
      add(positions, c);

      //Lower Triangle
      indices.push_back(static_cast<u32>(positions.size() / 3 + 0));
      indices.push_back(static_cast<u32>(positions.size() / 3 + 1));
      indices.push_back(static_cast<u32>(positions.size() / 3 + 2));
      indices.push_back(static_cast<u32>(positions.size() / 3 + 0));

      add(positions, d);
      add(positions, c);
      add(positions, b);
    }
  }
  gfx::buffer_desc_t vbo_desc = {
    positions.size() * sizeof(f32),
      positions.data(),
      gfx::buffer_type::vertex_buffer
  };
  gfx::buffer_desc_t ibo_desc = {
      indices.size() * sizeof(u32),
      indices.data(),
      gfx::buffer_type::index_buffer };
  gfx::buffer_t vbo = gfx::buffer_ctor(vbo_desc);
  gfx::buffer_t ibo = gfx::buffer_ctor(ibo_desc);
  return renderer::render_object_t("", vbo, ibo, indices.size());
}

#endif // __MESH_GENERATOR_H__