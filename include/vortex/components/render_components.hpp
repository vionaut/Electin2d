#pragma once

#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/math/vortex_math.hpp"
namespace vortex::components
{
	struct VxSpriteComponent
	{
		renderer::VxTexture* img_texture;

		math::VxRect src_rectangle;

		renderer::VxColor tint;

		int z_index;
	};
}