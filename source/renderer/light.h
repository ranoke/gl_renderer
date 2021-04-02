#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "base/types.h"

#include <glm/glm.hpp>

namespace renderer
{

  struct light_t
  {
    light_t()
    {}
    light_t(const glm::vec3 pos, const glm::vec3& color, float ambient_strength = 0.1f, float specular_strenth = 0.5f, u32 resolution = 32)
      : position_(pos)
      , color_(color)
      , ambient_strength_(ambient_strength)
      , specular_strength_(specular_strenth)
      , resolution_(resolution)
    {
    }


    glm::vec3 position_;
    glm::vec3 color_;
    float ambient_strength_;
    float specular_strength_;
    u32 resolution_;
    
  };


}


#endif // __LIGHT_H__