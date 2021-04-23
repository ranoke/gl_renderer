#ifndef __MESH_H_
#define __MESH_H_

#include "gfx/gfx_utils.h"
#include "renderer/vertex.h"
#include "renderer/render_object.h"
#include "renderer/camera.h"


#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// for now it will be implemented object oriented way which is pretty bad to do
// TODO move it to ECS
// HACK


namespace renderer
{

  struct mesh_t
  {
    mesh_t() = default;
    mesh_t(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const renderer::render_object_t& robj, const renderer::material_t material)
      : position_(position)
      , rotation_(rotation)
      , scale_(scale)
      , render_object_(robj)
      , material_(material)
    {

    }
    mesh_t(const mesh_t& other) = default;
    mesh_t(mesh_t&& other) = default;
    // for now i will pass camera to it, but later it should probably get
    // the active camera and maybe shader from the scane structure 
    // to wich it will have a pointer
    void render(gfx::program_t shader, const renderer::free_camera_t& camera)
    {
      glm::mat4 model = get_transfrom();
			gfx::bind_program(shader);
			gfx::bind_buffer(render_object_.vertex_buffer_);
			gfx::bind_buffer(render_object_.index_buffer_);
			gfx::vertex_array_set_layout({{gfx::gl_type::gl_float3}, {gfx::gl_type::gl_float2}, {gfx::gl_type::gl_float3}});

			auto& mat = material_;
			gfx::set_uniform_mat4(shader, "u_view", camera.view_);
			gfx::set_uniform_mat4(shader, "u_proj", camera.projection_);
			gfx::set_uniform_mat4(shader, "u_model", model);
			gfx::set_uniform_vec3(shader, "u_view_pos", camera.position_);
			gfx::set_uniform_vec3(shader, "u_material.ambient", mat.ambient_);
			gfx::set_uniform_vec3(shader, "u_material.diffuse", mat.diffuse_);
			gfx::set_uniform_vec3(shader, "u_material.specular", mat.specular_);
			gfx::set_uniform_float(shader, "u_material.shininess", mat.shininess_);
			gfx::set_uniform_vec3(shader, "u_light.position", {0,0,0});
			gfx::set_uniform_vec3(shader, "u_light.ambient", {1,1,1});
			gfx::set_uniform_vec3(shader, "u_light.diffuse", {0.8, 0.8, 0.8});
			gfx::set_uniform_vec3(shader, "u_light.specular", {0.5, 0.5, 0.5});

			gfx::draw_elements(gfx::gl_draw_mode::triangles, render_object_.index_count_, gfx::gl_type::gl_uint, 0);
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
  
    renderer::render_object_t render_object_;
    renderer::material_t      material_;
  };



} // namespace scene

#endif // __MESH_H_