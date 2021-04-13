#ifndef __MESH_H_
#define __MESH_H_

#include "renderer/render_object.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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