#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "base/base.h"
#include "base/types.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace renderer
{


	struct free_camera_t
	{
		free_camera_t(float fov_degrees, float aspect, float near__, float far__)
		{
			projection_ = glm::perspective(glm::radians(fov_degrees), aspect, near__, far__);
			
		}

		void update_view()
		{
			glm::vec3 direction;
			direction.x = cos(glm::radians(rotation_.x)) * cos(glm::radians(rotation_.y));
			direction.y = sin(glm::radians(rotation_.y));
			direction.z = sin(glm::radians(rotation_.x)) * cos(glm::radians(rotation_.y));
			camera_front_ = glm::normalize(direction);
			view_ = glm::lookAt(position_, position_ + camera_front_, camera_up_);
		}

		glm::mat4 get_viewprojection() { return projection_ * view_; }

		glm::vec3 camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 camera_up_    = glm::vec3(0.0f, 1.0f,  0.0f);

		glm::vec3 position_   = glm::vec3(0,0,0);
		glm::vec3 rotation_   = glm::vec3(0,0,0); // pitch, yaw, roll i guess
		glm::mat4 view_       = glm::mat4(1.f);
		glm::mat4 projection_ = glm::mat4(1.f);
	};

} // namespace renderer
#endif __CAMERA_H__