#ifndef __SCENE_H_
#define __SCENE_H_

#include "components.h"

#include <flecs.h>


namespace scene
{
  struct scene_t
  {
    scene_t()
    {
      components_register(ecs_);
    }
    flecs::world ecs_;
  };
} // namespace scene

#endif // __SCENE_H_