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
    light_t(const glm::vec3 pos, float strength)
      : position_(pos)
      , strength_(strength)
    {
    }

    glm::vec3 position_;
    float strength_;
  };


}


#endif // __LIGHT_H__