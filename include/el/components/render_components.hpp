#pragma once

#include <cstdint>
#include "el/renderer/el_graphics.hpp"
#include "el/core/math/el_math.hpp"

namespace el
{
	// NOTE: In a true ECS architecture, these components DO NOT hold World Position or World Rotation!
	// That is the job of the TransformComponent. 
	// These components only hold the visual definition (Size, Color, Source Rect) and sorting data.
	
	struct ElSpriteComponent
	{
		unsigned int texture_id;
		ElRect source; // The crop from the sprite sheet
		ElVec2 half_extents; // The unscaled half-width/height of the destination
		ElVec2 origin; // Pivot point relative to the size (width/height)
		ElColor tint;
		uint16_t layer = 0;  // Used for Z-Sorting in the Command Buffer
	};

	struct ElRectComponent
	{
		ElVec2 half_extents;
		ElVec2 origin;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElRoundedRectComponent
	{
		ElVec2 half_extents;
		ElVec2 origin;
		float roundness = 0.5f;
		int segments = 0; // 0 lets the backend auto-calculate
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElCircleComponent
	{
		float radius;
		float start_angle = 0.0f;
		float end_angle = 360.0f;
		int segments = 0;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElEllipseComponent
	{
		float radius_x;
		float radius_y;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElRingComponent
	{
		float inner_radius;
		float outer_radius;
		float start_angle = 0.0f;
		float end_angle = 360.0f;
		int segments = 0;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElCapsuleComponent
	{
		float radius;
		float half_length;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElPolygonComponent
	{
		int sides;
		float radius;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElTriangleComponent
	{
		// These should be local offsets from the Transform's position
		ElVec2 v1, v2, v3; 
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElLineComponent
	{
		// End point as a local offset from Transform position
		ElVec2 end_offset; 
		float thickness;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElSplineComponent
	{
		// Array of points. Note: Since this is a pointer, the memory must be managed by the user!
		ElVec2* points; 
		int point_count;
		float thickness;
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElPixelComponent
	{
		// Local offset from Transform position
		ElVec2 local_offset; 
		ElColor color;
		uint16_t layer = 0;
	};

	struct ElMeshComponent
	{
		// Array of vertices. Note: Since this is a pointer, the memory must be managed by the user!
		ElVec2* vertices;
		int vertex_count;
		ElColor color;
		uint16_t layer = 0;
	};
}