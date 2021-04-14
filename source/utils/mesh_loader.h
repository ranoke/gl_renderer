#ifndef __MESH_LOADER_H_
#define __MESH_LOADER_H_

#include "gfx/gfx.h"
#include "gfx/gfx_utils.h"

#include "scene/mesh.h"
#include "utils/tiny_obj_loader.h"

#include <iostream>

scene::mesh_t obj_load_mesh(const std::string& path)
{
  scene::mesh_t obj;

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

  // Append `default` material
  materials.push_back(tinyobj::material_t());

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%d].diffuse_texname = %s\n", int(i),
           materials[i].diffuse_texname.c_str());
  }

  // Load diffuse textures
  // {
  //   for (size_t m = 0; m < materials.size(); m++) {
  //     tinyobj::material_t* mp = &materials[m];

  //     if (mp->diffuse_texname.length() > 0) {
  //       // Only load the texture if it is not already loaded
  //       if (textures.find(mp->diffuse_texname) == textures.end()) {
  //         GLuint texture_id;
  //         int w, h;
  //         int comp;

  //         std::string texture_filename = mp->diffuse_texname;
  //         if (!FileExists(texture_filename)) {
  //           // Append base dir.
  //           texture_filename = base_dir + mp->diffuse_texname;
  //           if (!FileExists(texture_filename)) {
  //             std::cerr << "Unable to find file: " << mp->diffuse_texname
  //                       << std::endl;
  //             exit(1);
  //           }
  //         }

  //         unsigned char* image =
  //             stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
  //         if (!image) {
  //           std::cerr << "Unable to load texture: " << texture_filename
  //                     << std::endl;
  //           exit(1);
  //         }
  //         std::cout << "Loaded texture: " << texture_filename << ", w = " << w
  //                   << ", h = " << h << ", comp = " << comp << std::endl;

  //         glGenTextures(1, &texture_id);
  //         glBindTexture(GL_TEXTURE_2D, texture_id);
  //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //         if (comp == 3) {
  //           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
  //                        GL_UNSIGNED_BYTE, image);
  //         } else if (comp == 4) {
  //           glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
  //                        GL_UNSIGNED_BYTE, image);
  //         } else {
  //           assert(0);  // TODO
  //         }
  //         glBindTexture(GL_TEXTURE_2D, 0);
  //         stbi_image_free(image);
  //         textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
  //       }
  //     }
  //   }
  // }

	// TODO
	return obj;
}

#endif // __MESH_LOADER_H_