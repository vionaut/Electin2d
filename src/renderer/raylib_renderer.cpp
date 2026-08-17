#include "el/renderer/renderer_backend.hpp"
#include "el/core/config/el_config.hpp"
#include "el/core/utilities/algorithm.hpp"

#include "raylib.h"
#include "rlgl.h"

using namespace el;

namespace
{
	struct
	{
		RenderTexture2D virtual_canvas;
		el::backend::EResolutionMode resolution_mode = el::backend::EResolutionMode::PillarBox;
		float pillarBox_ratio = 16.0f/ 9.0f;

		//int msaa_4x = 0x00000020; // Internal raylib msaa flag

		bool isVsyncOn;
		int fps;
		int vsync_fps;

		int resolution_width;
		int resolution_height;

		int screen_width;
		int screen_height;

		int initialized_screen_width;
		int initialized_screen_height;

		int active_monitor;

		bool openIn_fullScreen = false;
		bool isCanvas_initialized = false;
	} ElGraphicsSettings;

	inline Rectangle toRayRect(el::ElRect& rect)
	{
		return {
			rect.x, rect.y,
			rect.width, rect.height
		};
	}

	inline Texture2D toRayTex(ElTexture& texture)
	{
		return {
			texture.id,
			texture.width,
			texture.height,
			texture.mipmaps,
			texture.format
		};
	}

	inline Vector2 toRayVec2(el::ElVec2& vec2)
	{
		return {
			vec2.x, vec2.y
		};
	}

	inline Vector3 toRayVec3(el::ElVec2& vec2)
	{
		return {
			vec2.x, vec2.y, 0
		};
	}

	inline Color toRayColor(ElColor& color)
	{
		return {
			color.r, color.g, color.b, color.a
		};
	}

	void processCommand(uint64_t key, const uint8_t* raw_buffer, el::ElStaticArray<ElTexture, el::MAX_TEXTURES + 1>& textures)
	{
		uint32_t offset = static_cast<uint32_t>(key & 0xFFFFFFFF);

		ERenderCommandType type;
		el::memcpy(&type, raw_buffer + offset, sizeof(ERenderCommandType));
		offset += sizeof(ERenderCommandType);

		switch (type)
		{
		case ERenderCommandType::ClearScreen:
		{
			ElClearCommand clr;
			el::memcpy(&clr, raw_buffer + offset, sizeof(ElClearCommand));

			ClearBackground(Color{ clr.color.r, clr.color.g, clr.color.b, clr.color.a });
			break;
		}

		case ERenderCommandType::DrawSprite:
		{
			ElSpriteCommand sprite;
			el::memcpy(&sprite, raw_buffer + offset, sizeof(ElSpriteCommand));

			ElTexture& texture = textures[sprite.texture_id];

			Texture2D ray_tex = toRayTex(texture);
			Rectangle ray_src = toRayRect(sprite.source);
			// Do NOT flip texture height anymore since we are not using rlScalef
			Rectangle ray_dest = toRayRect(sprite.dest);
			ray_dest.y = -ray_dest.y; // Invert world Y
			Vector2 ray_origin = toRayVec2(sprite.origin);
			Color ray_color = toRayColor(sprite.tint);

			DrawTexturePro(ray_tex, ray_src, ray_dest, ray_origin, -sprite.rotation, ray_color);

			break;
		}

		case ERenderCommandType::DrawRectangle:
		{
			ElRectCommand rect;
			el::memcpy(&rect, raw_buffer + offset, sizeof(ElRectCommand));

			Rectangle ray_rect = toRayRect(rect.rect);
			ray_rect.y = -ray_rect.y; // Invert world Y
			Vector2 ray_origin = toRayVec2(rect.origin);
			Color ray_color = toRayColor(rect.color);

			DrawRectanglePro(ray_rect, ray_origin, -rect.rotation, ray_color);

			break;
		}

		case ERenderCommandType::DrawRoundedRect:
		{
			ElRoundedRectCommand rounded_rect;
			el::memcpy(&rounded_rect, raw_buffer + offset, sizeof(ElRoundedRectCommand));

			// 1. Save the currrent world state
			rlPushMatrix();

			// 2. MOVE the center of the universe to the Shape's exact Pivot Point (Origin)
			rlTranslatef(rounded_rect.rect.x, -rounded_rect.rect.y, 0.0f);

			// 3. SPIN the universe around the Z-axis (2D rotation) by your rotation angle!
			rlRotatef(-rounded_rect.rotation, 0.0f, 0.0f, 1.0f);

			// 4. DRAW the shape!
			// BUT... since the universe's (0,0) is now sitting exactly at our pivot point,
			// we draw the rectangle offset by negative origin!
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
			ElLineCommand line;
			el::memcpy(&line, raw_buffer + offset, sizeof(ElLineCommand));
			
			rlPushMatrix();
			rlTranslatef(line.start_pos.x, -line.start_pos.y, 0.0f);
			rlRotatef(-line.rotation, 0.0f, 0.0f, 1.0f);
			
			Vector2 end_offset = toRayVec2(line.end_offset);
			end_offset.y = -end_offset.y;
			DrawLineEx({0.0f, 0.0f}, end_offset, line.thickness, toRayColor(line.color));
			
			rlPopMatrix();
			break;
		}

		case ERenderCommandType::DrawSpline:
		{
			ElSplineCommand spline;
			el::memcpy(&spline, raw_buffer + offset, sizeof(ElSplineCommand));

			rlPushMatrix();
			rlTranslatef(spline.position.x, -spline.position.y, 0.0f);
			rlRotatef(-spline.rotation, 0.0f, 0.0f, 1.0f);

			for (int i = 0; i < spline.point_count; ++i)
			{
				spline.points[i].y = -spline.points[i].y;
			}

			DrawSplineCatmullRom((Vector2*)spline.points, spline.point_count, spline.thickness, toRayColor(spline.color));
			
			rlPopMatrix();
			break;
		}

		case ERenderCommandType::DrawTriangle:
		{
			ElTriangleCommand tri;
			el::memcpy(&tri, raw_buffer + offset, sizeof(ElTriangleCommand));
			
			rlPushMatrix();
			rlTranslatef(tri.center_pos.x, -tri.center_pos.y, 0.0f);
			rlRotatef(-tri.rotation, 0.0f, 0.0f, 1.0f);
			
			Vector2 v1 = toRayVec2(tri.v1); v1.y = -v1.y;
			Vector2 v2 = toRayVec2(tri.v2); v2.y = -v2.y;
			Vector2 v3 = toRayVec2(tri.v3); v3.y = -v3.y;

			// Swap v2 and v3 to maintain CCW winding order after Y flip
			DrawTriangle(v1, v3, v2, toRayColor(tri.color));
			
			rlPopMatrix();
			break;
		}

		case ERenderCommandType::DrawCircle:
		{
			ElCircleCommand circle;
			el::memcpy(&circle, raw_buffer + offset, sizeof(ElCircleCommand));
			Vector2 center = toRayVec2(circle.center);
			center.y = -center.y;
			// To preserve the visual start/end angles in an inverted Y system, negate the angles and swap them
			DrawCircleSector(center, circle.radius, -circle.end_angle, -circle.start_angle, circle.segments, toRayColor(circle.color));
			break;
		}

		case ERenderCommandType::DrawEllipse:
		{
			ElEllipseCommand ellipse;
			el::memcpy(&ellipse, raw_buffer + offset, sizeof(ElEllipseCommand));
			
			rlPushMatrix();
			rlTranslatef(ellipse.center.x, -ellipse.center.y, 0.0f);
			rlRotatef(-ellipse.rotation, 0.0f, 0.0f, 1.0f);
			
			DrawEllipse(0, 0, ellipse.radius_x, ellipse.radius_y, toRayColor(ellipse.color));
			
			rlPopMatrix();
			break;
		}

		case ERenderCommandType::DrawRing:
		{
			ElRingCommand ring;
			el::memcpy(&ring, raw_buffer + offset, sizeof(ElRingCommand));
			Vector2 center = toRayVec2(ring.center);
			center.y = -center.y;
			DrawRing(center, ring.inner_radius, ring.outer_radius, -ring.end_angle, -ring.start_angle, ring.segments, toRayColor(ring.color));
			break;
		}

		case ERenderCommandType::DrawCapsule:
		{
			ElCapsuleCommand capsule;
			el::memcpy(&capsule, raw_buffer + offset, sizeof(ElCapsuleCommand));
			
			// 1. Save the universe and move to the capsule's start_pos (treating it as the pivot)
			rlPushMatrix();
			rlTranslatef(capsule.center_pos.x, -capsule.center_pos.y, 0.0f);
			rlRotatef(-capsule.rotation, 0.0f, 0.0f, 1.0f);

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
			ElPolygonCommand poly;
			el::memcpy(&poly, raw_buffer + offset, sizeof(ElPolygonCommand));
			Vector2 center = toRayVec2(poly.center);
			center.y = -center.y;
			DrawPoly(center, poly.sides, poly.radius, -poly.rotation, toRayColor(poly.color));
			break;
		}

		case ERenderCommandType::DrawMesh:
		{
			ElMeshCommand mesh;
			el::memcpy(&mesh, raw_buffer + offset, sizeof(ElMeshCommand));
			
			rlPushMatrix();
			rlTranslatef(mesh.position.x, -mesh.position.y, 0.0f);
			rlRotatef(-mesh.rotation, 0.0f, 0.0f, 1.0f);
			
			for (int i = 0; i < mesh.vertex_count; ++i)
			{
				mesh.vertices[i].y = -mesh.vertices[i].y;
			}

			// DrawTriangleFan takes a center point and fans out triangles to the rest of the array!
			// Note: Triangle fan winding order may need to be reversed for proper backface culling if enabled
			DrawTriangleFan((Vector2*)mesh.vertices, mesh.vertex_count, toRayColor(mesh.color));
			
			rlPopMatrix();
			break;
		}

		case ERenderCommandType::DrawPixel:
		{
			ElPixelCommand pixel;
			el::memcpy(&pixel, raw_buffer + offset, sizeof(ElPixelCommand));
			Vector2 pos = toRayVec2(pixel.position);
			pos.y = -pos.y;
			DrawPixelV(pos, toRayColor(pixel.color));
			break;
		}
		}
	}
}

namespace el::backend
{
	void setResolutionMode(EResolutionMode mode)
	{
		ElGraphicsSettings.resolution_mode = mode;
	}

	void setCanvasResolution(int width, int height)
	{
		ElGraphicsSettings.resolution_width = width;
		ElGraphicsSettings.resolution_height = height;

		if (ElGraphicsSettings.isCanvas_initialized) {
			UnloadRenderTexture(ElGraphicsSettings.virtual_canvas);
		}

		ElGraphicsSettings.virtual_canvas = LoadRenderTexture(ElGraphicsSettings.resolution_width, ElGraphicsSettings.resolution_height);
		ElGraphicsSettings.isCanvas_initialized = true;
	}

	void setFPS(int fps)
	{
		if(!ElGraphicsSettings.isVsyncOn)
			ElGraphicsSettings.fps = fps;
		SetTargetFPS(ElGraphicsSettings.fps);
	}

	//void setFXAA_4X(bool set)
	//{
	//	ElGraphicsSettings.msaa_4x = 0x00000020 * set;
	//}

	void setVsync(bool set) {
		ElGraphicsSettings.isVsyncOn = set;
		if(set)
			ElGraphicsSettings.fps = ElGraphicsSettings.vsync_fps;
		SetTargetFPS(ElGraphicsSettings.fps);
	}

	void setFullScreen(bool set)
	{
		if (set && !IsWindowFullscreen())
		{
			int monitor = GetCurrentMonitor();
			ElGraphicsSettings.screen_width = GetMonitorWidth(monitor);
			ElGraphicsSettings.screen_height = GetMonitorHeight(monitor);

			SetWindowSize(ElGraphicsSettings.screen_width, ElGraphicsSettings.screen_height);

			ToggleFullscreen();
		}
		else if (!set && IsWindowFullscreen())
		{
			ToggleFullscreen();

			ElGraphicsSettings.screen_width = ElGraphicsSettings.initialized_screen_width;
			ElGraphicsSettings.screen_height = ElGraphicsSettings.initialized_screen_height;

			SetWindowSize(ElGraphicsSettings.screen_width, ElGraphicsSettings.screen_height);
		}
	}

	void initWindow(int width, int height, const char* title)
	{
		ElGraphicsSettings.initialized_screen_width = width;
		ElGraphicsSettings.initialized_screen_height = height;
		SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_WINDOW_HIGHDPI) ;

		InitWindow(ElGraphicsSettings.initialized_screen_width, ElGraphicsSettings.initialized_screen_height, title);
		SetExitKey(KEY_NULL);

		setCanvasResolution(INITIAL_RESOLUTION_WIDTH, INITIAL_RESOLUTION_HEIGHT);

		ElGraphicsSettings.active_monitor = GetCurrentMonitor();
		ElGraphicsSettings.screen_width = GetMonitorWidth(ElGraphicsSettings.active_monitor);
		ElGraphicsSettings.screen_height = GetMonitorHeight(ElGraphicsSettings.active_monitor);
		ElGraphicsSettings.vsync_fps = GetMonitorRefreshRate(ElGraphicsSettings.active_monitor);

		ClearWindowState(FLAG_WINDOW_HIDDEN);
		setFullScreen(ElGraphicsSettings.openIn_fullScreen);

		setVsync(true);
	}

	void closeWindow()
	{
		CloseWindow();
	}

	bool shouldClose()
	{
		return WindowShouldClose();
	}

	ElTexture loadTexture(const char* file_path)
	{
		Texture2D rayTex = LoadTexture(file_path);

		ElTexture tex = {};
		tex.id = rayTex.id;
		tex.width = rayTex.width;
		tex.height = rayTex.height;
		tex.mipmaps = rayTex.mipmaps;
		tex.format = rayTex.format;

		return tex;
	}

	void unloadTexture(ElTexture texture)
	{
		Texture2D rayTex = { texture.id, texture.width, texture.height, texture.mipmaps, texture.format };
		UnloadTexture(rayTex);
	}

	void beginFrame()
	{
		BeginTextureMode(ElGraphicsSettings.virtual_canvas);
		ClearBackground(BLACK);
	}

	void executeQueue(const ElCommandBuffer& buffer, ElStaticArray<ElTexture, MAX_TEXTURES + 1>& textures, const ElCamera2d& camera)
	{
		Camera2D rayCamera = { 0 };
		rayCamera.target = { camera.target.x, -camera.target.y }; // Invert camera target Y
		rayCamera.offset = { camera.offset.x, camera.offset.y };
		rayCamera.rotation = -camera.rotation; // Invert camera rotation
		rayCamera.zoom = camera.zoom;

		ElArray<uint64_t> keys = buffer.getKeys();
		const uint8_t* raw_buffer = buffer.getBuffer();

		BeginMode2D(rayCamera);
		rlPushMatrix();

		for (uint64_t key : keys)
		{
			processCommand(key, raw_buffer, textures);
		}

		rlPopMatrix();
		EndMode2D();
	}

	void endFrame()
	{
		EndTextureMode();

		BeginDrawing();
		ClearBackground(BLACK);

		Rectangle source_rect = { 0.0f, 0.0f, (float)ElGraphicsSettings.resolution_width, -(float)ElGraphicsSettings.resolution_height };
		Rectangle dest_rect = { 0.0f, 0.0f, 0.0f, 0.0f };

		ElGraphicsSettings.screen_width = GetScreenWidth();
		ElGraphicsSettings.screen_height = GetScreenHeight();

		int current_monitor = GetCurrentMonitor();

		if (ElGraphicsSettings.active_monitor != current_monitor)
		{
			ElGraphicsSettings.active_monitor = current_monitor;
			ElGraphicsSettings.vsync_fps = GetMonitorRefreshRate(ElGraphicsSettings.active_monitor);

			setVsync(ElGraphicsSettings.isVsyncOn);
		}

		switch (ElGraphicsSettings.resolution_mode)
		{
		case EResolutionMode::Expand: // If the canvas and the monitor are in the same ratio, it expands, otherwise behave as PillarBox
		{
			float scaleX = (float)ElGraphicsSettings.screen_width / ElGraphicsSettings.resolution_width;
			float scaleY = (float)ElGraphicsSettings.screen_height / ElGraphicsSettings.resolution_height;
			float scale = (scaleX < scaleY) ? scaleX : scaleY;

			dest_rect.width = ElGraphicsSettings.resolution_width * scale;
			dest_rect.height = ElGraphicsSettings.resolution_height * scale;
			dest_rect.x = ((float)ElGraphicsSettings.screen_width - dest_rect.width) * 0.5f;
			dest_rect.y = ((float)ElGraphicsSettings.screen_height - dest_rect.height) * 0.5f;

			break;
		}

		case EResolutionMode::PillarBox:
		{
			float target_width = (float)ElGraphicsSettings.screen_width;
			float target_height = target_width / ElGraphicsSettings.pillarBox_ratio;

			// 2. If doing that makes it too tall for the monitor, we flip it.
			// We stretch all the way vertically, and calculate the width instead.
			if (target_height > (float)ElGraphicsSettings.screen_height)
			{
				target_height = (float)ElGraphicsSettings.screen_height;
				target_width = target_height * ElGraphicsSettings.pillarBox_ratio;
			}

			// 3. Set the destination and center it to create the black bars!
			dest_rect.width = target_width;
			dest_rect.height = target_height;
			dest_rect.x = ((float)ElGraphicsSettings.screen_width - dest_rect.width) * 0.5f;
			dest_rect.y = ((float)ElGraphicsSettings.screen_height - dest_rect.height) * 0.5f;

			break;
		}

		case EResolutionMode::Stretch:
		{
			dest_rect = { 0.0f, 0.0f, (float)ElGraphicsSettings.screen_width, (float)ElGraphicsSettings.screen_height };

			break;
		}
		}

		DrawTexturePro(ElGraphicsSettings.virtual_canvas.texture, source_rect, dest_rect, { 0, 0 }, 0.0f, WHITE);

		EndDrawing();
	}
}
