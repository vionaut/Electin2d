#pragma once

#include "el/renderer/el_graphics.hpp"
#include "el/core/assets/asset_manager.hpp"
#include "el/renderer/command_buffer.hpp"
#include "el/renderer/renderer_backend.hpp"
#include "el/core/utilities/memory.hpp"
#include "el/components/render_components.hpp"

namespace el
{
	class ElPencilRenderer
	{
	public:
		static void init(int window_width, int window_height, const char* title);
		static void shutdown();
		static void flush(const ElCamera2d& camera);
		static bool shouldClose();

		// The settings API
		static void setResolutionMode(backend::EResolutionMode mode);
		static void setFPS(int fps);
		static void setVsync(bool onVsync);
		static void setFullScreen(bool set);

		// The Drawing API
		static void clearScreen(const ElColor& color);

		static void drawSprite(const ElVec2& pos, float rotation, const ElSpriteComponent& sprite);
		static void drawRect(const ElVec2& pos, float rotation, const ElRectComponent& rect);
		static void drawRoundedRect(const ElVec2& pos, float rotation, const ElRoundedRectComponent& rect);
		static void drawCircle(const ElVec2& pos, const ElCircleComponent& circle);
		static void drawEllipse(const ElVec2& pos, float rotation, const ElEllipseComponent& ellipse);
		static void drawRing(const ElVec2& pos, const ElRingComponent& ring);
		static void drawCapsule(const ElVec2& pos, float rotation, const ElCapsuleComponent& capsule);
		static void drawPolygon(const ElVec2& pos, float rotation, const ElPolygonComponent& poly);

		static void drawTriangle(const ElVec2& pos, float rotation, const ElTriangleComponent& tri);
		static void drawLine(const ElVec2& pos, float rotation, const ElLineComponent& line);
		static void drawSpline(const ElVec2& pos, float rotation, const ElSplineComponent& spline);
		static void drawPixel(const ElVec2& pos, const ElPixelComponent& pixel);

		static void drawMesh(const ElVec2& pos, float rotation, const ElMeshComponent& mesh);

	private:
		static ElSpriteCommand spriteCTC(const ElVec2& pos, float rotation, const ElSpriteComponent& comp);
		static ElRectCommand rectCTC(const ElVec2& pos, float rotation, const ElRectComponent& comp);
		static ElRoundedRectCommand roundedRectCTC(const ElVec2& pos, float rotation, const ElRoundedRectComponent& comp);
		static ElCircleCommand circleCTC(const ElVec2& pos, const ElCircleComponent& comp);
		static ElEllipseCommand ellipseCTC(const ElVec2& pos, float rotation, const ElEllipseComponent& comp);
		static ElRingCommand ringCTC(const ElVec2& pos, const ElRingComponent& comp);
		static ElCapsuleCommand capsuleCTC(const ElVec2& pos, float rotation, const ElCapsuleComponent& comp);
		static ElPolygonCommand polygonCTC(const ElVec2& pos, float rotation, const ElPolygonComponent& comp);
		static ElTriangleCommand triangleCTC(const ElVec2& pos, float rotation, const ElTriangleComponent& comp);
		static ElLineCommand lineCTC(const ElVec2& pos, float rotation, const ElLineComponent& comp);
		static ElSplineCommand splineCTC(const ElVec2& pos, float rotation, const ElSplineComponent& comp);
		static ElPixelCommand pixelCTC(const ElVec2& pos, const ElPixelComponent& comp);
		static ElMeshCommand meshCTC(const ElVec2& pos, float rotation, const ElMeshComponent& comp);

		static ElUniquePtr<ElCommandBuffer> m_commandBuffer;
	};
}