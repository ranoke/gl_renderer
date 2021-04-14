#ifndef __MESH_H_
#define __MESH_H_

#include "gfx/gfx_utils.h"
#include "renderer/render_object.h"
#include "renderer/camera.h"


#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// for now it will be implemented object oriented way which is pretty bad to do
// TODO move it to ECS
// HACK

namespace scene
{
  // if the object is built from many objects every of this
  // objects should have its own transform ??? 
  struct submesh_t 
  {
    glm::vec3 position_ = {0,0,0}; // relative to the mesh
    glm::vec3 rotation_ = {0,0,0}; // its own rotation
    glm::vec3 scale_    = {1,1,1}; // its own scale

    renderer::render_object_t render_object_;
  };

  struct mesh_t
  {
    // for now i will pass camera to it, but later it should probably get
    // the active camera and maybe shader from the scane structure 
    // to wich it will have a pointer
    void render(gfx::program_t shader, const renderer::free_camera_t& camera)
    {
      glm::mat4 model = get_transfrom();
      for(auto& sm : mesh_)
      {
        gfx::bind_program(shader);
        gfx::bind_buffer(sm.render_object_.vertex_buffer_);
        gfx::bind_buffer(sm.render_object_.index_buffer_);
        gfx::vertex_array_set_layout({{gfx::gl_float3}, {gfx::gl_float2}, {gfx::gl_float3}});

        // TODO add lighting
        gfx::set_uniform_mat4(shader, "u_view", camera.view_);
        gfx::set_uniform_mat4(shader, "u_proj", camera.projection_);
        gfx::set_uniform_mat4(shader, "u_model", model);

        gfx::draw_elements(gfx::gl_triangles, sm.render_object_.index_count_, gfx::gl_uint, 0);
      }
    }

    glm::mat4 get_transfrom()
    {
      glm::mat4 r = glm::toMat4(glm::quat(rotation_));

			return glm::translate(glm::mat4(1.0f), position_)
				* r
				* glm::scale(glm::mat4(1.0f), scale_);
    } 

    glm::vec3 position_ = {0,0,0};
    glm::vec3 rotation_ = {0,0,0};
    glm::vec3 scale_    = {1,1,1};

    std::vector<submesh_t> mesh_;
  };



} // namespace scene

#endif // __MESH_H_