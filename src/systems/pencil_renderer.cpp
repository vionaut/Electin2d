#include "vortex/systems/pencil_renderer.hpp"

namespace vortex::renderer
{
	utils::VxUniquePtr<renderer::VxCommandBuffer> VxPencilRenderer::m_commandBuffer = nullptr;

	void VxPencilRenderer::init(int window_width, int window_height, const char* title)
	{
		backend::initWindow(window_width, window_height, title);
		m_commandBuffer = utils::vxMakeUnique<renderer::VxCommandBuffer>(2 * 1024 * 1024);
	}

	void VxPencilRenderer::shutdown()
	{
		m_commandBuffer.reset();
		backend::closeWindow();
	}

	void VxPencilRenderer::flush(const VxCamera2d& camera)
	{
		m_commandBuffer->sort(); // Z-Sort all commands before rendering!
		backend::beginFrame();
		backend::executeQueue(*m_commandBuffer, core::VxAssetManager::getTexturesAll(), camera);
		m_commandBuffer->clear();
		backend::endFrame();
	}

	bool VxPencilRenderer::shouldClose()
	{
		return backend::shouldClose();
	}

	void VxPencilRenderer::setResolutionMode(backend::EResolutionMode mode)
	{
		backend::setResolutionMode(mode);
	}

	void VxPencilRenderer::setFPS(int fps)
	{
		backend::setFPS(fps);
	}

	void VxPencilRenderer::setVsync(bool onVsync)
	{
		backend::setVsync(onVsync);
	}

	void VxPencilRenderer::setFullScreen(bool set)
	{
		backend::setFullScreen(set);
	}

	// ------------------------------------------------------------------------
	// DRAWING API
	// ------------------------------------------------------------------------

	void VxPencilRenderer::clearScreen(const VxColor& color)
	{
		VxClearCommand cmd;
		cmd.color = color;
		// Z-index 0 ensures it gets sorted to the very beginning of the draw queue
		m_commandBuffer->submit(ERenderCommandType::ClearScreen, 0, 0, cmd);
	}

	void VxPencilRenderer::drawSprite(const math::VxVec2& pos, float rotation, const components::VxSpriteComponent& sprite)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawSprite, sprite.layer, sprite.texture_id, spriteCTC(pos, rotation, sprite));
	}

	void VxPencilRenderer::drawRect(const math::VxVec2& pos, float rotation, const components::VxRectComponent& rect)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawRectangle, rect.layer, 0, rectCTC(pos, rotation, rect));
	}

	void VxPencilRenderer::drawRoundedRect(const math::VxVec2& pos, float rotation, const components::VxRoundedRectComponent& rect)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawRoundedRect, rect.layer, 0, roundedRectCTC(pos, rotation, rect));
	}

	void VxPencilRenderer::drawCircle(const math::VxVec2& pos, const components::VxCircleComponent& circle)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawCircle, circle.layer, 0, circleCTC(pos, circle));
	}

	void VxPencilRenderer::drawEllipse(const math::VxVec2& pos, float rotation, const components::VxEllipseComponent& ellipse)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawEllipse, ellipse.layer, 0, ellipseCTC(pos, rotation, ellipse));
	}

	void VxPencilRenderer::drawRing(const math::VxVec2& pos, const components::VxRingComponent& ring)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawRing, ring.layer, 0, ringCTC(pos, ring));
	}

	void VxPencilRenderer::drawCapsule(const math::VxVec2& pos, float rotation, const components::VxCapsuleComponent& capsule)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawCapsule, capsule.layer, 0, capsuleCTC(pos, rotation, capsule));
	}

	void VxPencilRenderer::drawPolygon(const math::VxVec2& pos, float rotation, const components::VxPolygonComponent& poly)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawPolygon, poly.layer, 0, polygonCTC(pos, rotation, poly));
	}

	void VxPencilRenderer::drawTriangle(const math::VxVec2& pos, float rotation, const components::VxTriangleComponent& tri)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawTriangle, tri.layer, 0, triangleCTC(pos, rotation, tri));
	}

	void VxPencilRenderer::drawLine(const math::VxVec2& pos, float rotation, const components::VxLineComponent& line)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawLine, line.layer, 0, lineCTC(pos, rotation, line));
	}

	void VxPencilRenderer::drawSpline(const math::VxVec2& pos, float rotation, const components::VxSplineComponent& spline)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawSpline, spline.layer, 0, splineCTC(pos, rotation, spline));
	}

	void VxPencilRenderer::drawPixel(const math::VxVec2& pos, const components::VxPixelComponent& pixel)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawPixel, pixel.layer, 0, pixelCTC(pos, pixel));
	}

	void VxPencilRenderer::drawMesh(const math::VxVec2& pos, float rotation, const components::VxMeshComponent& mesh)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawMesh, mesh.layer, 0, meshCTC(pos, rotation, mesh));
	}

	VxSplineCommand VxPencilRenderer::splineCTC(const math::VxVec2& pos, float rotation, const components::VxSplineComponent& comp)
	{
		VxSplineCommand cmd;
		cmd.points = comp.points; // NOTE: Memory is managed by the user. Assumes points are already in world-space!
		cmd.point_count = comp.point_count;
		cmd.thickness = comp.thickness;
		cmd.color = comp.color;
		return cmd;
	}

	VxPixelCommand VxPencilRenderer::pixelCTC(const math::VxVec2& pos, const components::VxPixelComponent& comp)
	{
		VxPixelCommand cmd;
		cmd.position = { pos.x + comp.local_offset.x, pos.y + comp.local_offset.y };
		cmd.color = comp.color;
		return cmd;
	}

	VxMeshCommand VxPencilRenderer::meshCTC(const math::VxVec2& pos, float rotation, const components::VxMeshComponent& comp)
	{
		VxMeshCommand cmd;
		cmd.vertices = comp.vertices; // NOTE: Memory is managed by the user. Assumes points are already in world-space!
		cmd.vertex_count = comp.vertex_count;
		cmd.color = comp.color;
		return cmd;
	}

	VxSpriteCommand VxPencilRenderer::spriteCTC(const math::VxVec2& pos, float rotation, const components::VxSpriteComponent& comp)
	{
		VxSpriteCommand cmd;
		cmd.dest = { pos.x, pos.y, comp.size.x, comp.size.y };
		cmd.source = comp.source;
		cmd.origin = comp.origin;
		cmd.texture_id = comp.texture_id;
		cmd.rotation = rotation;
		cmd.tint = comp.tint;
		return cmd;
	}

	VxRectCommand VxPencilRenderer::rectCTC(const math::VxVec2& pos, float rotation, const components::VxRectComponent& comp)
	{
		VxRectCommand cmd;
		cmd.rect = { pos.x, pos.y, comp.size.x, comp.size.y };
		cmd.origin = comp.origin;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	VxRoundedRectCommand VxPencilRenderer::roundedRectCTC(const math::VxVec2& pos, float rotation, const components::VxRoundedRectComponent& comp)
	{
		VxRoundedRectCommand cmd;
		cmd.rect = { pos.x, pos.y, comp.size.x, comp.size.y };
		cmd.origin = comp.origin;
		cmd.rotation = rotation;
		cmd.roundness = comp.roundness;
		cmd.segments = comp.segments;
		cmd.color = comp.color;
		return cmd;
	}

	VxCircleCommand VxPencilRenderer::circleCTC(const math::VxVec2& pos, const components::VxCircleComponent& comp)
	{
		VxCircleCommand cmd;
		cmd.center = pos;
		cmd.radius = comp.radius;
		cmd.start_angle = comp.start_angle;
		cmd.end_angle = comp.end_angle;
		cmd.segments = comp.segments;
		cmd.color = comp.color;
		return cmd;
	}

	VxEllipseCommand VxPencilRenderer::ellipseCTC(const math::VxVec2& pos, float rotation, const components::VxEllipseComponent& comp)
	{
		VxEllipseCommand cmd;
		cmd.center = pos;
		cmd.radius_x = comp.radius_x;
		cmd.radius_y = comp.radius_y;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	VxRingCommand VxPencilRenderer::ringCTC(const math::VxVec2& pos, const components::VxRingComponent& comp)
	{
		VxRingCommand cmd;
		cmd.center = pos;
		cmd.inner_radius = comp.inner_radius;
		cmd.outer_radius = comp.outer_radius;
		cmd.start_angle = comp.start_angle;
		cmd.end_angle = comp.end_angle;
		cmd.segments = comp.segments;
		cmd.color = comp.color;
		return cmd;
	}

	VxCapsuleCommand VxPencilRenderer::capsuleCTC(const math::VxVec2& pos, float rotation, const components::VxCapsuleComponent& comp)
	{
		VxCapsuleCommand cmd;
		cmd.center_pos = pos;
		cmd.radius = comp.radius;
		cmd.half_length = comp.half_length;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	VxPolygonCommand VxPencilRenderer::polygonCTC(const math::VxVec2& pos, float rotation, const components::VxPolygonComponent& comp)
	{
		VxPolygonCommand cmd;
		cmd.center = pos;
		cmd.sides = comp.sides;
		cmd.radius = comp.radius;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	VxTriangleCommand VxPencilRenderer::triangleCTC(const math::VxVec2& pos, float rotation, const components::VxTriangleComponent& comp)
	{
		VxTriangleCommand cmd;
		// Triangle vertices are local offsets from the origin (pos)
		cmd.v1 = { pos.x + comp.v1.x, pos.y + comp.v1.y };
		cmd.v2 = { pos.x + comp.v2.x, pos.y + comp.v2.y };
		cmd.v3 = { pos.x + comp.v3.x, pos.y + comp.v3.y };
		cmd.color = comp.color;
		return cmd;
	}

	VxLineCommand VxPencilRenderer::lineCTC(const math::VxVec2& pos, float rotation, const components::VxLineComponent& comp)
	{
		VxLineCommand cmd;
		cmd.start = pos;
		// Line end_offset is local from the origin (pos)
		cmd.end = { pos.x + comp.end_offset.x, pos.y + comp.end_offset.y };
		cmd.thickness = comp.thickness;
		cmd.color = comp.color;
		return cmd;
	}
}
