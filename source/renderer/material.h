#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "base/types.h"
#include "gfx/gfx.h"

#include <glm/glm.hpp>

namespace renderer
{
	struct material_t
	{

		glm::vec3 ambient_;
		glm::vec3 diffuse_;
		glm::vec3 specular_;
		glm::vec3 transmittance_;
		glm::vec3 emission_;
		f32 shininess_;
		f32 index_of_refraction_;
		f32 dissolve_;

		gfx::texture_t ambient_tex_;
		gfx::texture_t diffuse_tex_;
		gfx::texture_t specular_tex_;
		gfx::texture_t specular_highlight_tex_;
		gfx::texture_t bump_tex_;
		gfx::texture_t displacement_tex_;
		gfx::texture_t alpha_tex_;
		gfx::texture_t reflection_tex_;

		f32 roughness_;             // [0 : 1]
		f32 metalic_;               // [0 : 1]
		f32 sheen_;                 // [0 : 1]
		f32 clearcoat_thikness_;    // [0 : 1]
		f32 clearcoat_roughness_;   // [0 : 1]
		f32 anisotropy_;            // [0 : 1]
		f32 anisotropy_rotation_;   // [0 : 1]

	};
} // renderer


#endif // __MATERIAL_H_