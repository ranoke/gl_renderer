#ifndef __COMPONENTS_H_
#define __COMPONENTS_H_

#include <flecs.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scene
{
  // components for the ecs( flecs )
  struct c_transform_t
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

  };

  void components_register(const flecs::world& ecs)
  {

  }

} // namespace scene

#endif // __COMPONENTS_H_