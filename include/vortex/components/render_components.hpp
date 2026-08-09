#pragma once

#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/math/vortex_math.hpp"

namespace vortex::components
{
	// NOTE: In a true ECS architecture, these components DO NOT hold World Position or World Rotation!
	// That is the job of the TransformComponent. 
	// These components only hold the visual definition (Size, Color, Source Rect) and sorting data.
	
	struct VxSpriteComponent
	{
		unsigned int texture_id;
		math::VxRect source; // The crop from the sprite sheet
		math::VxVec2 size;   // The unscaled width/height of the destination
		math::VxVec2 origin; // Pivot point relative to the size
		renderer::VxColor tint;
		uint16_t layer = 0;  // Used for Z-Sorting in the Command Buffer
	};

	struct VxRectComponent
	{
		math::VxVec2 size;
		math::VxVec2 origin;
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxRoundedRectComponent
	{
		math::VxVec2 size;
		math::VxVec2 origin;
		float roundness = 0.5f;
		int segments = 0; // 0 lets the backend auto-calculate
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxCircleComponent
	{
		float radius;
		float start_angle = 0.0f;
		float end_angle = 360.0f;
		int segments = 0;
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxEllipseComponent
	{
		float radius_x;
		float radius_y;
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxRingComponent
	{
		float inner_radius;
		float outer_radius;
		float start_angle = 0.0f;
		float end_angle = 360.0f;
		int segments = 0;
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxCapsuleComponent
	{
		float radius;
		float half_length;
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxPolygonComponent
	{
		int sides;
		float radius;
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxTriangleComponent
	{
		// These should be local offsets from the Transform's position
		math::VxVec2 v1, v2, v3; 
		renderer::VxColor color;
		uint16_t layer = 0;
	};

	struct VxLineComponent
	{
		// End point as a local offset from Transform position
		math::VxVec2 end_offset; 
		float thickness;
		renderer::VxColor color;
		uint16_t layer = 0;
	};
}