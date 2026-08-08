#pragma once

#include "vortex/core/math/vortex_math.hpp"

namespace vortex::renderer
{
	enum class ERenderCommandType
	{
		ClearScreen,

		DrawSprite,
		DrawRectangle,
		DrawRoundedRect,
		DrawLine,
		DrawSpline,
		DrawTriangle,
		DrawCircle,
		DrawEllipse,
		DrawRing,
		DrawCapsule,
		DrawPolygon,
		DrawMesh,
		DrawPixel
	}; 

	struct VxColor
	{
		unsigned char r, g, b, a;
	};

	// Base texture struct
	struct VxTexture
	{
		unsigned int id; // Id assigned by raylib
		int width, height;
		int mipmaps;
		int format;
	};

	struct VxCamera2d
	{
		math::VxVec2 target;
		math::VxVec2 offset;
		float rotation = 0.0f;
		float zoom = 1.0f;
	};

	// --- BYTE BUFFER COMMAND STRUCTS ---
	struct VxClearCommand {
		VxColor color;
	};

	struct VxSpriteCommand {
		math::VxRect dest;
		math::VxRect source;
		math::VxVec2 origin;
		unsigned int texture_id; // Id assigned by VxAssetManager
		float rotation;
		VxColor tint;
	};

	struct VxRectCommand {
		math::VxRect rect;
		math::VxVec2 origin;
		float rotation;
		VxColor color;
	};

	struct VxCircleCommand {
		math::VxVec2 center;
		float radius;
		float start_angle;
		float end_angle;
		int segments;
		VxColor color;
	};

	struct VxTriangleCommand {
		math::VxVec2 v1, v2, v3;
		VxColor color;
	};

	struct VxLineCommand {
		math::VxVec2 start;
		math::VxVec2 end;
		float thickness;
		VxColor color;
	};

	struct VxPolygonCommand {
		math::VxVec2 center;
		int sides;
		float radius;
		float rotation;
		VxColor color;
	};

	struct VxRoundedRectCommand {
		math::VxRect rect;
		math::VxVec2 origin;
		float rotation;
		float roundness;
		int segments;
		VxColor color;
	};

	struct VxEllipseCommand {
		math::VxVec2 center;
		float radius_x;
		float radius_y;
		float rotation;
		VxColor color;
	};

	struct VxRingCommand {
		math::VxVec2 center;
		float inner_radius;
		float outer_radius;
		float start_angle;
		float end_angle;
		int segments;
		VxColor color;
	};

	struct VxSplineCommand {
		const math::VxVec2* points;
		int point_count;
		float thickness;
		VxColor color;
	};

	struct VxCapsuleCommand {
		math::VxVec2 center_pos;
		float radius;
		float half_length;
		float rotation; // 0 degree points to the +x and 90 degree points to the +y meaning downwards
		VxColor color;
	};

	struct VxMeshCommand {
		const math::VxVec2* vertices;
		int vertex_count;
		VxColor color;
	};

	struct VxPixelCommand {
		math::VxVec2 position;
		VxColor color;
	};
}