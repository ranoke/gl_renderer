#ifndef __MESH_LOADER_H__
#define __MESH_LOADER_H__

#include "base/base.h"
#include "gfx/gfx.h"

#include "renderer/render_object.h"
#include "renderer/material.h"
#include "renderer/mesh.h"

#include "tiny_obj_loader.h"

#include <string>
#include <utility>



renderer::mesh_t
obj_load(const std::string& path)
{
	renderer::mesh_t mesh;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), "./res");
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

	auto& mat = mesh.material_;
	// load material
	if(materials.size() > 1)
	{
		const tinyobj::material_t& m = materials[0];
		mat.ambient_ = { m.ambient[0], m.ambient[1], m.ambient[2] };
		mat.diffuse_ = { m.diffuse[0], m.diffuse[1], m.diffuse[2] };
		mat.specular_ = { m.specular[0], m.specular[1], m.specular[2] };
		mat.transmittance_ = { m.transmittance[0], m.transmittance[1], m.transmittance[2] };
		mat.emission_ = { m.emission[0], m.emission[1], m.emission[2] };
		mat.shininess_ = m.shininess;
		mat.index_of_refraction_ = m.ior;
		mat.dissolve_ = m.dissolve;
		mat.ambient_tex_ = gfx_utils::texture_load(m.ambient_texname);
		mat.diffuse_tex_ = gfx_utils::texture_load(m.diffuse_texname);
		mat.specular_tex_ = gfx_utils::texture_load(m.specular_texname);
		mat.specular_highlight_tex_ = gfx_utils::texture_load(m.specular_highlight_texname);
		mat.bump_tex_ = gfx_utils::texture_load(m.bump_texname);
		mat.displacement_tex_ = gfx_utils::texture_load(m.displacement_texname);
		mat.alpha_tex_ = gfx_utils::texture_load(m.alpha_texname);
		mat.reflection_tex_ = gfx_utils::texture_load(m.reflection_texname);
	}

	std::vector<vertex_t> vertices;
	std::vector<u32> indices;

	for (const auto &shape : shapes)
	{
		for (const auto &index : shape.mesh.indices)
		{
			vertex_t v;
			const auto v_id  = index.vertex_index;
			const auto n_id  = index.normal_index;
			const auto uv_id = index.texcoord_index;
			v.position_ = {
					attrib.vertices[v_id * 3 + 0],
					attrib.vertices[v_id * 3 + 1],
					attrib.vertices[v_id * 3 + 2]};
			v.normal_ = {
					attrib.normals[n_id * 3 + 0],
					attrib.normals[n_id * 3 + 1],
					attrib.normals[n_id * 3 + 2]};
			if(attrib.texcoords.size())
			{
				v.uv_ = {
						attrib.texcoords[uv_id*2 + 0],
						attrib.texcoords[uv_id*2 + 1],
				};
			}

			vertices.push_back(v);
			indices.push_back(indices.size());
		}
	}
	gfx::buffer_desc_t vbo_desc = {
		vertices.size()*sizeof(vertex_t),
		vertices.data(),
		gfx::buffer_type::vertex_buffer
	};
	gfx::buffer_t vbo = gfx::buffer_ctor(vbo_desc);

	gfx::buffer_desc_t ibo_desc = {
		indices.size() * sizeof(u32),
		indices.data(),
		gfx::buffer_type::index_buffer
	};
	gfx::buffer_t ibo = gfx::buffer_ctor(ibo_desc);

	mesh.render_object_ = { path, vbo, ibo, (u32)indices.size() };

	return mesh;
}

// HACK // TODO 
// not even a mesh - just a renderer thingy that i can draw
renderer::render_object_t
obj_load_render_object(const std::string &path)
{

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
			const auto v_id  = index.vertex_index;
			const auto n_id  = index.normal_index;
			const auto uv_id = index.texcoord_index;
			v.position_ = {
					attrib.vertices[v_id * 3 + 0],
					attrib.vertices[v_id * 3 + 1],
					attrib.vertices[v_id * 3 + 2]};
			v.normal_ = {
					attrib.normals[n_id * 3 + 0],
					attrib.normals[n_id * 3 + 1],
					attrib.normals[n_id * 3 + 2]};
			if(attrib.texcoords.size())
			{
				v.uv_ = {
						attrib.texcoords[uv_id*2 + 0],
						attrib.texcoords[uv_id*2 + 1],
				};
			}

			vertices.push_back(v);
			indices.push_back(indices.size());
		}
	}
	gfx::buffer_desc_t vbo_desc = {
		vertices.size()*sizeof(vertex_t),
		vertices.data(),
		gfx::buffer_type::vertex_buffer
	};
	gfx::buffer_t vbo = gfx::buffer_ctor(vbo_desc);

	gfx::buffer_desc_t ibo_desc = {
		indices.size() * sizeof(u32),
		indices.data(),
		gfx::buffer_type::index_buffer
	};
	gfx::buffer_t ibo = gfx::buffer_ctor(ibo_desc);



	return { path, vbo, ibo, (u32)indices.size() };
}

#endif // __MESH_LOADER_H__