#ifndef __MESH_LOADER_H__
#define __MESH_LOADER_H__

#include "base/base.h"
#include "gfx/gfx.h"
#include "renderer/render_object.h"

#include "tiny_obj_loader.h"


#include <string>
#include <utility>

std::vector<renderer::render_object_t>
obj_load(const std::string& path)
{
	std::vector<renderer::render_object_t> objs;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;


  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), "./");
  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }
  if (!err.empty()) {
    std::cerr << err << std::endl;
  }

  if (!ret) {
    std::cerr << "Failed to load " << path << std::endl;
		assert(false);
  }


  printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
  printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
  printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
  printf("# of materials = %d\n", (int)materials.size());
  printf("# of shapes    = %d\n", (int)shapes.size());

	assert(false && "write implementation");
	// TODO
	return objs;
}

// HACK // TODO 
// not even a mesh - just a renderer thingy that i can draw
renderer::render_object_t
obj_load_render_object(const std::string &path)
{
	struct vertex_t
	{
		glm::vec3 position_;
		glm::vec2 uv_;
		glm::vec3 normal_;
	};

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(),
															"", true);

	std::vector<vertex_t> vertices;
	std::vector<u32> indices;

	for (const auto &shape : shapes)
	{
		for (const auto &index : shape.mesh.indices)
		{
			vertex_t v;
			v.position_ = {
					attrib.vertices[index.vertex_index*3 + 0],
					attrib.vertices[index.vertex_index*3 + 1],
					attrib.vertices[index.vertex_index*3 + 2]};
			v.normal_ = {
					attrib.normals[index.normal_index*3 + 0],
					attrib.normals[index.normal_index *3 + 1],
					attrib.normals[index.normal_index *3 + 2]};
			if(attrib.texcoords.size())
			{
				v.uv_ = {
						attrib.texcoords[index.texcoord_index*2 + 0],
						attrib.texcoords[index.texcoord_index*2 + 1],
				};
			}

			vertices.push_back(v);
			indices.push_back(indices.size());
		}
	}
	gfx::buffer_desc_t vbo_desc = {
		vertices.size()*sizeof(vertex_t),
		vertices.data(),
		gfx::vertex_buffer
	};
	gfx::buffer_t vbo = gfx::buffer_ctor(vbo_desc);

	gfx::buffer_desc_t ibo_desc = {
		indices.size() * sizeof(u32),
		indices.data(),
		gfx::index_buffer
	};
	gfx::buffer_t ibo = gfx::buffer_ctor(ibo_desc);

	std::vector<gfx::texture_t> textures; // TODO add texture loading
	

	return { path, vbo, ibo, (u32)indices.size(), {} };
}

#endif // __MESH_LOADER_H__