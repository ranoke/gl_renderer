#ifndef __MESH_LOADER_H__
#define __MESH_LOADER_H__

#include "base/base.h"
#include "gfx/gfx.h"
#include "renderer/mesh.h"

#include "tiny_obj_loader.h"


#include <string>
#include <utility>

// TODO
// so i decided that i dont want to implement mesh for now so what i will do
// is just load the mesh and load it straight to the GPU
// i guess it will be the best variant for now

enum mesh_loader_properties_e
{
	mesh_loader_properties_vertices = BIT(0),
	mesh_loader_properties_colors = BIT(1),
	mesh_loader_properties_uvs = BIT(2),
	mesh_loader_properties_normals = BIT(3),
};

// std::pair<gfx::buffer_t, gfx::buffer_t>
// obj_load_mesh(const std::string &path, u64 props)
// {
// 	gfx::buffer_t vbo;
// 	gfx::buffer_t ibo;
// 	tinyobj::attrib_t attrib;
// 	std::vector<tinyobj::shape_t> shapes;
// 	std::vector<tinyobj::material_t> materials;

// 	std::string warn;
// 	std::string err;
// 	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), NULL, true, true);

// 	u64 buffer_count = 0;
// 	float* buffer_ptr = nullptr;

// 	if (props & mesh_loader_properties_vertices)
// 		buffer_count += attrib.vertices.size();
// 	if (props & mesh_loader_properties_colors)
// 		buffer_count += attrib.colors.size();
// 	if (props & mesh_loader_properties_uvs)
// 		buffer_count += attrib.texcoords.size();
//   if (props & mesh_loader_properties_normals)
// 		buffer_count += attrib.normals.size();

// 	buffer_ptr = new float[buffer_count];

// 	u32 data_offset =   (props & mesh_loader_properties_vertices ? 3 : 0) +
// 											(props & mesh_loader_properties_colors ? 3 : 0) +
// 											(props & mesh_loader_properties_uvs ? 2 : 0) +
// 											(props & mesh_loader_properties_normals ? 3 : 0);
// 	if (props & mesh_loader_properties_vertices)
// 	for (u64 v = 0; v < attrib.vertices.size() / 3; v++)
// 	{
// 		buffer_ptr[v*data_offset + 0] = attrib.vertices[3 * v + 0];
// 		buffer_ptr[v*data_offset + 1] = attrib.vertices[3 * v + 1];
// 		buffer_ptr[v*data_offset + 2] = attrib.vertices[3 * v + 2];
// 	}

// 	u32 color_pre_offset = props & mesh_loader_properties_vertices ? 3 : 0;
// 	if (props & mesh_loader_properties_colors)
// 	for(u64 c = 0; c < attrib.colors.size(); c++)
// 	{
// 		buffer_ptr[c*data_offset + color_pre_offset + 0] = attrib.colors[3 * c + 0];
// 		buffer_ptr[c*data_offset + color_pre_offset + 1] = attrib.colors[3 * c + 1];
// 		buffer_ptr[c*data_offset + color_pre_offset + 2] = attrib.colors[3 * c + 2];
// 	}

// 	u32 uv_pre_offset = props & mesh_loader_properties_vertices ? 3 : 0 +
// 											props & mesh_loader_properties_colors ? 3 : 0;
// 	if (props & mesh_loader_properties_uvs)
// 	for(u64 i = 0; i < attrib.texcoords.size(); i++)
// 	{
// 		buffer_ptr[i*data_offset + color_pre_offset + 0] = attrib.texcoords[3 * i + 0];
// 		buffer_ptr[i*data_offset + color_pre_offset + 1] = attrib.texcoords[3 * i + 1];
// 		buffer_ptr[i*data_offset + color_pre_offset + 2] = attrib.texcoords[3 * i + 2];
// 	}

// 	u32 normal_pre_offset = props & mesh_loader_properties_vertices ? 3 : 0 +
// 											props & mesh_loader_properties_colors ? 3 : 0 +
// 											props & mesh_loader_properties_uvs ? 2 : 0;
// 	if (props & mesh_loader_properties_uvs)
// 	for(u64 i = 0; i < attrib.normals.size(); i++)
// 	{
// 		buffer_ptr[i*data_offset + normal_pre_offset + 0] = attrib.normals[3 * i + 0];
// 		buffer_ptr[i*data_offset + normal_pre_offset + 1] = attrib.normals[3 * i + 1];
// 		buffer_ptr[i*data_offset + normal_pre_offset + 2] = attrib.normals[3 * i + 2];
// 	}

// 	//printf("Load completed:\nStats:\nbuffer_count: %d\nbuffer_size %d\ndata_offset %d\n",
// 	//buffer_count, buffer_count*sizeof(float), data_offset);
// 	//printf("Vertex count: %d\n", attrib.vertices.size()/3);

// 	gfx::buffer_desc_t vbo_desc = {
// 		sizeof(buffer_ptr),
// 		buffer_ptr,
// 		gfx::vertex_buffer
// 	};
// 	vbo = gfx::buffer_ctor(vbo_desc);

// 	gfx::buffer_desc_t ibo_desc = {
// 		shapes[0].mesh.indices.size() * sizeof(tinyobj::index_t),
// 		&shapes[0].mesh.indices[0],
// 		gfx::index_buffer
// 	};
// 	ibo = gfx::buffer_ctor(ibo_desc);

// 	//printf("vbo&ibo - [%d %d]\n", vbo, ibo);
// 	printf("%X\n", buffer_ptr);

// 	return {vbo, ibo};
// }



// HACK // TODO
renderer::mesh_t
obj_load_mesh(const std::string &path)
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
															"./", true);

	std::vector<vertex_t> vertices;
	std::vector<u32> indices;

	for (const auto &shape : shapes)
	{
		for (const auto &index : shape.mesh.indices)
		{
			vertex_t v;
			v.position_ = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]};
			v.normal_ = {
					attrib.normals[3 * index.vertex_index + 0],
					attrib.normals[3 * index.vertex_index + 1],
					attrib.normals[3 * index.vertex_index + 2]};
			v.uv_ = {
					attrib.texcoords[2 * index.vertex_index + 0],
					attrib.texcoords[2 * index.vertex_index + 1],
			};

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


	

	return { path, vbo, ibo, indices.size() };
}

#endif __MESH_LOADER_H__