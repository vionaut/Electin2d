#pragma once

#include "vortex/renderer/vortex_graphics.hpp"
#include "vortex/core/assets/asset_manager.hpp"
#include "vortex/renderer/command_buffer.hpp"
#include "vortex/renderer/renderer_backend.hpp"
#include "vortex/core/utilities/memory.hpp"
#include "vortex/components/render_components.hpp"

namespace vortex::renderer
{
	class VxPencilRenderer
	{
	public:
		static void init(int window_width, int window_height, const char* title);
		static void shutdown();
		static void flush(const VxCamera2d& camera);
		static bool shouldClose();

		// The settings API
		static void setResolutionMode(backend::EResolutionMode mode);
		static void setFPS(int fps);
		static void setVsync(bool onVsync);
		static void setFullScreen(bool set);

		// The Drawing API
		static void clearScreen(const VxColor& color);

		static void drawSprite(const math::VxVec2& pos, float rotation, const components::VxSpriteComponent& sprite);
		static void drawRect(const math::VxVec2& pos, float rotation, const components::VxRectComponent& rect);
		static void drawRoundedRect(const math::VxVec2& pos, float rotation, const components::VxRoundedRectComponent& rect);
		static void drawCircle(const math::VxVec2& pos, const components::VxCircleComponent& circle);
		static void drawEllipse(const math::VxVec2& pos, float rotation, const components::VxEllipseComponent& ellipse);
		static void drawRing(const math::VxVec2& pos, const components::VxRingComponent& ring);
		static void drawCapsule(const math::VxVec2& pos, float rotation, const components::VxCapsuleComponent& capsule);
		static void drawPolygon(const math::VxVec2& pos, float rotation, const components::VxPolygonComponent& poly);

		static void drawTriangle(const math::VxVec2& pos, float rotation, const components::VxTriangleComponent& tri);
		static void drawLine(const math::VxVec2& pos, float rotation, const components::VxLineComponent& line);
		static void drawSpline(const math::VxVec2& pos, float rotation, const components::VxSplineComponent& spline);
		static void drawPixel(const math::VxVec2& pos, const components::VxPixelComponent& pixel);

		static void drawMesh(const math::VxVec2& pos, float rotation, const components::VxMeshComponent& mesh);

	private:
		static VxSpriteCommand spriteCTC(const math::VxVec2& pos, float rotation, const components::VxSpriteComponent& comp);
		static VxRectCommand rectCTC(const math::VxVec2& pos, float rotation, const components::VxRectComponent& comp);
		static VxRoundedRectCommand roundedRectCTC(const math::VxVec2& pos, float rotation, const components::VxRoundedRectComponent& comp);
		static VxCircleCommand circleCTC(const math::VxVec2& pos, const components::VxCircleComponent& comp);
		static VxEllipseCommand ellipseCTC(const math::VxVec2& pos, float rotation, const components::VxEllipseComponent& comp);
		static VxRingCommand ringCTC(const math::VxVec2& pos, const components::VxRingComponent& comp);
		static VxCapsuleCommand capsuleCTC(const math::VxVec2& pos, float rotation, const components::VxCapsuleComponent& comp);
		static VxPolygonCommand polygonCTC(const math::VxVec2& pos, float rotation, const components::VxPolygonComponent& comp);
		static VxTriangleCommand triangleCTC(const math::VxVec2& pos, float rotation, const components::VxTriangleComponent& comp);
		static VxLineCommand lineCTC(const math::VxVec2& pos, float rotation, const components::VxLineComponent& comp);
		static VxSplineCommand splineCTC(const math::VxVec2& pos, float rotation, const components::VxSplineComponent& comp);
		static VxPixelCommand pixelCTC(const math::VxVec2& pos, const components::VxPixelComponent& comp);
		static VxMeshCommand meshCTC(const math::VxVec2& pos, float rotation, const components::VxMeshComponent& comp);

		static utils::VxUniquePtr<renderer::VxCommandBuffer> m_commandBuffer;
	};
}