#include "vortex/renderer/renderer_backend.hpp"
#include "vortex/core/config/vortex_config.hpp"
#include "vortex/core/utilities/algorithm.hpp"

#include "raylib.h"
#include "rlgl.h"

using namespace vortex::renderer;

namespace
{
	inline Rectangle toRayRect(vortex::math::VxRect& rect)
	{
		return {
			rect.x, rect.y,
			rect.width, rect.height
		};
	}

	inline Texture2D toRayTex(VxTexture& texture)
	{
		return {
			texture.id,
			texture.width,
			texture.height,
			texture.mipmaps,
			texture.format
		};
	}

	inline Vector2 toRayVec2(vortex::math::VxVec2& vec2)
	{
		return {
			vec2.x, vec2.y
		};
	}

	inline Vector3 toRayVec3(vortex::math::VxVec2& vec2)
	{
		return {
			vec2.x, vec2.y, 0
		};
	}

	inline Color toRayColor(VxColor& color)
	{
		return {
			color.r, color.g, color.b, color.a
		};
	}

	void processCommand(uint64_t key, const uint8_t* raw_buffer, vortex::containers::VxStaticArray<VxTexture, vortex::config::MAX_TEXTURES>& textures)
	{
		uint32_t offset = static_cast<uint32_t>(key & 0xFFFFFFFF);

		ERenderCommandType type;
		vortex::utils::memcpy(&type, raw_buffer + offset, sizeof(ERenderCommandType));
		offset += sizeof(ERenderCommandType);

		switch (type)
		{
		case ERenderCommandType::ClearScreen:
		{
			VxClearCommand clr;
			vortex::utils::memcpy(&clr, raw_buffer + offset, sizeof(VxClearCommand));

			ClearBackground(Color{ clr.color.r, clr.color.g, clr.color.b, clr.color.a });
			break;
		}

		case ERenderCommandType::DrawSprite:
		{
			VxSpriteCommand sprite;
			vortex::utils::memcpy(&sprite, raw_buffer + offset, sizeof(VxSpriteCommand));

			VxTexture& texture = textures[sprite.texture_id];

			Texture2D ray_tex = toRayTex(texture);
			Rectangle ray_src = toRayRect(sprite.source);
			Rectangle ray_dest = toRayRect(sprite.dest);
			Vector2 ray_origin = toRayVec2(sprite.origin);
			Color ray_color = toRayColor(sprite.tint);

			DrawTexturePro(ray_tex, ray_src, ray_dest, ray_origin, sprite.rotation, ray_color);

			break;
		}

		case ERenderCommandType::DrawRectangle:
		{
			VxRectCommand rect;
			vortex::utils::memcpy(&rect, raw_buffer + offset, sizeof(VxRectCommand));

			Rectangle ray_rect = toRayRect(rect.rect);
			Vector2 ray_origin = toRayVec2(rect.origin);
			Color ray_color = toRayColor(rect.color);

			DrawRectanglePro(ray_rect, ray_origin, rect.rotation, ray_color);

			break;
		}

		case ERenderCommandType::DrawRoundedRect:
		{
			VxRoundedRectCommand rounded_rect;
			vortex::utils::memcpy(&rounded_rect, raw_buffer + offset, sizeof(VxRoundedRectCommand));

			// 1. Save the currrent world state
			rlPushMatrix();

			// 2. MOVE the center of the universe to the Shape's exact Pivot Point (Origin)
			rlTranslatef(rounded_rect.rect.x + rounded_rect.origin.x, rounded_rect.rect.y + rounded_rect.origin.y, 0.0f);

			// 3. SPIN the universe around the Z-axis (2D rotation) by your rotation angle!
			rlRotatef(rounded_rect.rotation, 0.0f, 0.0f, 1.0f);

			// 4. DRAW the shape!
			// BUT... since the universe's (0,0) is now sitting exactly at our pivot point,
			// we draw the rectangle at (0,0) minus the origin!
			Rectangle rayRect = {
				-rounded_rect.origin.x,
				-rounded_rect.origin.y,
				rounded_rect.rect.width,
				rounded_rect.rect.height
			};

			Color ray_color = toRayColor(rounded_rect.color);

			DrawRectangleRounded(rayRect, rounded_rect.roundness, rounded_rect.segments, ray_color);

			rlPopMatrix();

			break;
		}

		case ERenderCommandType::DrawLine:
		{
			VxLineCommand line;
			vortex::utils::memcpy(&line, raw_buffer + offset, sizeof(VxLineCommand));
			DrawLineEx(toRayVec2(line.start), toRayVec2(line.end), line.thickness, toRayColor(line.color));
			break;
		}

		case ERenderCommandType::DrawSpline:
		{
			VxSplineCommand spline;
			vortex::utils::memcpy(&spline, raw_buffer + offset, sizeof(VxSplineCommand));

			DrawSplineCatmullRom((Vector2*)spline.points, spline.point_count, spline.thickness, toRayColor(spline.color));
			break;
		}

		case ERenderCommandType::DrawTriangle:
		{
			VxTriangleCommand tri;
			vortex::utils::memcpy(&tri, raw_buffer + offset, sizeof(VxTriangleCommand));
			DrawTriangle(toRayVec2(tri.v1), toRayVec2(tri.v2), toRayVec2(tri.v3), toRayColor(tri.color));
			break;
		}

		case ERenderCommandType::DrawCircle:
		{
			VxCircleCommand circle;
			vortex::utils::memcpy(&circle, raw_buffer + offset, sizeof(VxCircleCommand));
			DrawCircleSector(toRayVec2(circle.center), circle.radius, circle.start_angle, circle.end_angle, circle.segments, toRayColor(circle.color));
			break;
		}

		case ERenderCommandType::DrawEllipse:
		{
			VxEllipseCommand ellipse;
			vortex::utils::memcpy(&ellipse, raw_buffer + offset, sizeof(VxEllipseCommand));
			
			rlPushMatrix();
			rlTranslatef(ellipse.center.x, ellipse.center.y, 0.0f);
			rlRotatef(ellipse.rotation, 0.0f, 0.0f, 1.0f);
			
			DrawEllipse(0, 0, ellipse.radius_x, ellipse.radius_y, toRayColor(ellipse.color));
			
			rlPopMatrix();
			break;
		}

		case ERenderCommandType::DrawRing:
		{
			VxRingCommand ring;
			vortex::utils::memcpy(&ring, raw_buffer + offset, sizeof(VxRingCommand));
			DrawRing(toRayVec2(ring.center), ring.inner_radius, ring.outer_radius, ring.start_angle, ring.end_angle, ring.segments, toRayColor(ring.color));
			break;
		}

		case ERenderCommandType::DrawCapsule:
		{
			VxCapsuleCommand capsule;
			vortex::utils::memcpy(&capsule, raw_buffer + offset, sizeof(VxCapsuleCommand));
			
			// 1. Save the universe and move to the capsule's start_pos (treating it as the pivot)
			rlPushMatrix();
			rlTranslatef(capsule.center_pos.x, capsule.center_pos.y, 0.0f);
			rlRotatef(capsule.rotation, 0.0f, 0.0f, 1.0f);

			// 2. Because the universe moved, our new start is (0,0) and our end is just the difference!
			Vector2 start = { -(capsule.half_length - capsule.radius) , 0.0f };

			Vector2 end = { (capsule.half_length - capsule.radius), 0.0f };
			Color ray_color = toRayColor(capsule.color);

			// 3. Draw the two round caps at the endpoints
			DrawCircleV(start, capsule.radius, ray_color);
			DrawCircleV(end, capsule.radius, ray_color);

			// 4. A thick line perfectly fills the rectangle between two circles!
			DrawLineEx(start, end, capsule.radius * 2.0f, ray_color);
			
			rlPopMatrix();

			break;
		}

		case ERenderCommandType::DrawPolygon:
		{
			VxPolygonCommand poly;
			vortex::utils::memcpy(&poly, raw_buffer + offset, sizeof(VxPolygonCommand));
			DrawPoly(toRayVec2(poly.center), poly.sides, poly.radius, poly.rotation, toRayColor(poly.color));
			break;
		}

		case ERenderCommandType::DrawMesh:
		{
			VxMeshCommand mesh;
			vortex::utils::memcpy(&mesh, raw_buffer + offset, sizeof(VxMeshCommand));
			// DrawTriangleFan takes a center point and fans out triangles to the rest of the array!
			DrawTriangleFan((Vector2*)mesh.vertices, mesh.vertex_count, toRayColor(mesh.color));
			break;
		}

		case ERenderCommandType::DrawPixel:
		{
			VxPixelCommand pixel;
			vortex::utils::memcpy(&pixel, raw_buffer + offset, sizeof(VxPixelCommand));
			DrawPixelV(toRayVec2(pixel.position), toRayColor(pixel.color));
			break;
		}
		}
	}
}

namespace vortex::renderer::backend
{
	void initWindow(int width, int height, const char* title)
	{
		SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT) ;

		InitWindow(width, height, title);

		int monitor = GetCurrentMonitor();
		int screen_width = GetMonitorWidth(monitor);
		int screen_height = GetMonitorHeight(monitor);
		int refresh_rate = GetMonitorRefreshRate(monitor);

		SetWindowSize(screen_width, screen_height);
		SetWindowPosition(0, 0);

		ClearWindowState(FLAG_WINDOW_HIDDEN);

		SetWindowState(FLAG_FULLSCREEN_MODE);

		if (refresh_rate > 0)
			SetTargetFPS(refresh_rate);
		else
			SetTargetFPS(60);
	}

	void closeWindow()
	{
		CloseWindow();
	}

	bool shouldClose()
	{
		return WindowShouldClose();
	}

	VxTexture loadTexture(const char* file_path)
	{
		Texture2D rayTex = LoadTexture(file_path);

		VxTexture tex = {};
		tex.id = rayTex.id;
		tex.width = rayTex.width;
		tex.height = rayTex.height;
		tex.mipmaps = rayTex.mipmaps;
		tex.format = rayTex.format;

		return tex;
	}

	void unloadTexture(VxTexture texture)
	{
		Texture2D rayTex = { texture.id, texture.width, texture.height, texture.mipmaps, texture.format };
		UnloadTexture(rayTex);
	}

	void beginFrame()
	{
		BeginDrawing();
	}

	void executeQueue(const VxCommandBuffer& buffer, containers::VxStaticArray<VxTexture, config::MAX_TEXTURES>& textures, const VxCamera2d& camera)
	{
		Camera2D rayCamera = { 0 };
		rayCamera.target = { camera.target.x, camera.target.y };
		rayCamera.offset = { camera.offset.x, camera.offset.y };
		rayCamera.rotation = camera.rotation;
		rayCamera.zoom = camera.zoom;

		containers::VxArray<uint64_t> keys = buffer.getKeys();
		const uint8_t* raw_buffer = buffer.getBuffer();

		BeginMode2D(rayCamera);

		for (uint64_t key : keys)
		{
			processCommand(key, raw_buffer, textures);
		}

		EndMode2D();
	}

	void endFrame()
	{
		EndDrawing();
	}
}
