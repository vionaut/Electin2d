#pragma once

#include "el/core/math/el_math.hpp"

namespace el
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

	struct ElColor
	{
		unsigned char r, g, b, a;
	};

	// Base texture struct
	struct ElTexture
	{
		unsigned int id; // Id assigned by raylib
		int width, height;
		int mipmaps;
		int format;
	};

	struct ElCamera2d
	{
		ElVec2 target;
		ElVec2 offset;
		float rotation = 0.0f;
		float zoom = 1.0f;
	};

	// --- BYTE BUFFER COMMAND STRUCTS ---
	struct ElClearCommand {
		ElColor color;
	};

	struct ElSpriteCommand {
		ElRect dest;
		ElRect source;
		ElVec2 origin;
		unsigned int texture_id; // Id assigned by ElAssetManager
		float rotation;
		ElColor tint;
	};

	struct ElRectCommand {
		ElRect rect;
		ElVec2 origin;
		float rotation;
		ElColor color;
	};

	struct ElCircleCommand {
		ElVec2 center;
		float radius;
		float start_angle;
		float end_angle;
		int segments;
		ElColor color;
	};

	struct ElTriangleCommand {
		ElVec2 center_pos;
		ElVec2 v1, v2, v3;
		float rotation;
		ElColor color;
	};

	struct ElLineCommand {
		ElVec2 start_pos;
		ElVec2 end_offset;
		float rotation;
		float thickness;
		ElColor color;
	};

	struct ElPolygonCommand {
		ElVec2 center;
		int sides;
		float radius;
		float rotation;
		ElColor color;
	};

	struct ElRoundedRectCommand {
		ElRect rect;
		ElVec2 origin;
		float rotation;
		float roundness;
		int segments;
		ElColor color;
	};

	struct ElEllipseCommand {
		ElVec2 center;
		float radius_x;
		float radius_y;
		float rotation;
		ElColor color;
	};

	struct ElRingCommand {
		ElVec2 center;
		float inner_radius;
		float outer_radius;
		float start_angle;
		float end_angle;
		int segments;
		ElColor color;
	};

	struct ElSplineCommand {
		ElVec2 position;
		float rotation;
		ElVec2* points;
		int point_count;
		float thickness;
		ElColor color;
	};

	struct ElCapsuleCommand {
		ElVec2 center_pos;
		float radius;
		float half_length;
		float rotation; // 0 degree points to the +x and 90 degree points to the +y meaning downwards
		ElColor color;
	};

	struct ElMeshCommand {
		ElVec2 position;
		float rotation;
		ElVec2* vertices;
		int vertex_count;
		ElColor color;
	};

	struct ElPixelCommand {
		ElVec2 position;
		ElColor color;
	};
}