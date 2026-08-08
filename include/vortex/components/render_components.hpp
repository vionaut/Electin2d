#pragma once

#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/math/vortex_math.hpp"
namespace vortex::components
{
	struct VxSpriteComponent
	{
		renderer::VxTexture *img_texture;

		math::VxRect src_rectangle;

		renderer::VxColor tint;

		int z_index;
	};

	struct VxShapeTypeComponent
	{
		renderer::EShapeType shape_type;
	};

	struct VxRectangleComponent
	{
		renderer::VxColor color;

		math::VxVec2 center_offset;
		math::VxVec2 half_width, half_height;

		int z_index;
	};

	struct VxCircleComponent
	{
		renderer::VxColor color;

		math::VxVec2 center_offset;
		float radius;

		int z_index;
	};

	struct VxTriangleComponent
	{
		renderer::VxColor color;

		math::VxVec2 v1, v2, v3;

		int z_index;
	};
}