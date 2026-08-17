#include "el/renderer/pencil_renderer.hpp"

namespace el
{
	ElUniquePtr<ElCommandBuffer> ElPencilRenderer::m_commandBuffer = nullptr;

	void ElPencilRenderer::init(int window_width, int window_height, const char* title)
	{
		backend::initWindow(window_width, window_height, title);
		m_commandBuffer = elMakeUnique<ElCommandBuffer>(2 * 1024 * 1024);
	}

	void ElPencilRenderer::shutdown()
	{
		m_commandBuffer.reset();
		backend::closeWindow();
	}

	void ElPencilRenderer::flush(const ElCamera2d& camera)
	{
		m_commandBuffer->sort(); // Z-Sort all commands before rendering!
		backend::beginFrame();
		backend::executeQueue(*m_commandBuffer, ElAssetManager::getTexturesAll(), camera);
		m_commandBuffer->clear();
		backend::endFrame();
	}

	bool ElPencilRenderer::shouldClose()
	{
		return backend::shouldClose();
	}

	void ElPencilRenderer::setResolutionMode(backend::EResolutionMode mode)
	{
		backend::setResolutionMode(mode);
	}

	void ElPencilRenderer::setFPS(int fps)
	{
		backend::setFPS(fps);
	}

	void ElPencilRenderer::setVsync(bool onVsync)
	{
		backend::setVsync(onVsync);
	}

	void ElPencilRenderer::setFullScreen(bool set)
	{
		backend::setFullScreen(set);
	}

	// ------------------------------------------------------------------------
	// DRAWING API
	// ------------------------------------------------------------------------

	void ElPencilRenderer::clearScreen(const ElColor& color)
	{
		ElClearCommand cmd;
		cmd.color = color;
		// Z-index 0 ensures it gets sorted to the very beginning of the draw queue
		m_commandBuffer->submit(ERenderCommandType::ClearScreen, 0, 0, cmd);
	}

	void ElPencilRenderer::drawSprite(const ElVec2& pos, float rotation, const ElSpriteComponent& sprite)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawSprite, sprite.layer, sprite.texture_id, spriteCTC(pos, rotation, sprite));
	}

	void ElPencilRenderer::drawRect(const ElVec2& pos, float rotation, const ElRectComponent& rect)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawRectangle, rect.layer, 0, rectCTC(pos, rotation, rect));
	}

	void ElPencilRenderer::drawRoundedRect(const ElVec2& pos, float rotation, const ElRoundedRectComponent& rect)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawRoundedRect, rect.layer, 0, roundedRectCTC(pos, rotation, rect));
	}

	void ElPencilRenderer::drawCircle(const ElVec2& pos, const ElCircleComponent& circle)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawCircle, circle.layer, 0, circleCTC(pos, circle));
	}

	void ElPencilRenderer::drawEllipse(const ElVec2& pos, float rotation, const ElEllipseComponent& ellipse)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawEllipse, ellipse.layer, 0, ellipseCTC(pos, rotation, ellipse));
	}

	void ElPencilRenderer::drawRing(const ElVec2& pos, const ElRingComponent& ring)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawRing, ring.layer, 0, ringCTC(pos, ring));
	}

	void ElPencilRenderer::drawCapsule(const ElVec2& pos, float rotation, const ElCapsuleComponent& capsule)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawCapsule, capsule.layer, 0, capsuleCTC(pos, rotation, capsule));
	}

	void ElPencilRenderer::drawPolygon(const ElVec2& pos, float rotation, const ElPolygonComponent& poly)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawPolygon, poly.layer, 0, polygonCTC(pos, rotation, poly));
	}

	void ElPencilRenderer::drawTriangle(const ElVec2& pos, float rotation, const ElTriangleComponent& tri)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawTriangle, tri.layer, 0, triangleCTC(pos, rotation, tri));
	}

	void ElPencilRenderer::drawLine(const ElVec2& pos, float rotation, const ElLineComponent& line)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawLine, line.layer, 0, lineCTC(pos, rotation, line));
	}

	void ElPencilRenderer::drawSpline(const ElVec2& pos, float rotation, const ElSplineComponent& spline)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawSpline, spline.layer, 0, splineCTC(pos, rotation, spline));
	}

	void ElPencilRenderer::drawPixel(const ElVec2& pos, const ElPixelComponent& pixel)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawPixel, pixel.layer, 0, pixelCTC(pos, pixel));
	}

	void ElPencilRenderer::drawMesh(const ElVec2& pos, float rotation, const ElMeshComponent& mesh)
	{
		m_commandBuffer->submit(ERenderCommandType::DrawMesh, mesh.layer, 0, meshCTC(pos, rotation, mesh));
	}

	ElSplineCommand ElPencilRenderer::splineCTC(const ElVec2& pos, float rotation, const ElSplineComponent& comp)
	{
		ElSplineCommand cmd;
		cmd.position = pos;
		cmd.rotation = rotation;
		cmd.points = comp.points; // NOTE: Local offsets!
		cmd.point_count = comp.point_count;
		cmd.thickness = comp.thickness;
		cmd.color = comp.color;
		return cmd;
	}

	ElPixelCommand ElPencilRenderer::pixelCTC(const ElVec2& pos, const ElPixelComponent& comp)
	{
		ElPixelCommand cmd;
		cmd.position = { pos.x + comp.local_offset.x, pos.y + comp.local_offset.y };
		cmd.color = comp.color;
		return cmd;
	}

	ElMeshCommand ElPencilRenderer::meshCTC(const ElVec2& pos, float rotation, const ElMeshComponent& comp)
	{
		ElMeshCommand cmd;
		cmd.position = pos;
		cmd.rotation = rotation;
		cmd.vertices = comp.vertices; // NOTE: Local offsets!
		cmd.vertex_count = comp.vertex_count;
		cmd.color = comp.color;
		return cmd;
	}

	ElSpriteCommand ElPencilRenderer::spriteCTC(const ElVec2& pos, float rotation, const ElSpriteComponent& comp)
	{
		ElSpriteCommand cmd;
		cmd.dest = { pos.x, pos.y, comp.half_extents.x * 2.0f, comp.half_extents.y * 2.0f };
		cmd.source = comp.source;
		cmd.origin = comp.origin;
		cmd.texture_id = comp.texture_id;
		cmd.rotation = rotation;
		cmd.tint = comp.tint;
		return cmd;
	}

	ElRectCommand ElPencilRenderer::rectCTC(const ElVec2& pos, float rotation, const ElRectComponent& comp)
	{
		ElRectCommand cmd;
		cmd.rect = { pos.x, pos.y, comp.half_extents.x * 2.0f, comp.half_extents.y * 2.0f };
		cmd.origin = comp.origin;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	ElRoundedRectCommand ElPencilRenderer::roundedRectCTC(const ElVec2& pos, float rotation, const ElRoundedRectComponent& comp)
	{
		ElRoundedRectCommand cmd;
		cmd.rect = { pos.x, pos.y, comp.half_extents.x * 2.0f, comp.half_extents.y * 2.0f };
		cmd.origin = comp.origin;
		cmd.rotation = rotation;
		cmd.roundness = comp.roundness;
		cmd.segments = comp.segments;
		cmd.color = comp.color;
		return cmd;
	}

	ElCircleCommand ElPencilRenderer::circleCTC(const ElVec2& pos, const ElCircleComponent& comp)
	{
		ElCircleCommand cmd;
		cmd.center = pos;
		cmd.radius = comp.radius;
		cmd.start_angle = comp.start_angle;
		cmd.end_angle = comp.end_angle;
		cmd.segments = comp.segments;
		cmd.color = comp.color;
		return cmd;
	}

	ElEllipseCommand ElPencilRenderer::ellipseCTC(const ElVec2& pos, float rotation, const ElEllipseComponent& comp)
	{
		ElEllipseCommand cmd;
		cmd.center = pos;
		cmd.radius_x = comp.radius_x;
		cmd.radius_y = comp.radius_y;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	ElRingCommand ElPencilRenderer::ringCTC(const ElVec2& pos, const ElRingComponent& comp)
	{
		ElRingCommand cmd;
		cmd.center = pos;
		cmd.inner_radius = comp.inner_radius;
		cmd.outer_radius = comp.outer_radius;
		cmd.start_angle = comp.start_angle;
		cmd.end_angle = comp.end_angle;
		cmd.segments = comp.segments;
		cmd.color = comp.color;
		return cmd;
	}

	ElCapsuleCommand ElPencilRenderer::capsuleCTC(const ElVec2& pos, float rotation, const ElCapsuleComponent& comp)
	{
		ElCapsuleCommand cmd;
		cmd.center_pos = pos;
		cmd.radius = comp.radius;
		cmd.half_length = comp.half_length;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	ElPolygonCommand ElPencilRenderer::polygonCTC(const ElVec2& pos, float rotation, const ElPolygonComponent& comp)
	{
		ElPolygonCommand cmd;
		cmd.center = pos;
		cmd.sides = comp.sides;
		cmd.radius = comp.radius;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	ElTriangleCommand ElPencilRenderer::triangleCTC(const ElVec2& pos, float rotation, const ElTriangleComponent& comp)
	{
		ElTriangleCommand cmd;
		cmd.center_pos = pos;
		cmd.v1 = comp.v1;
		cmd.v2 = comp.v2;
		cmd.v3 = comp.v3;
		cmd.rotation = rotation;
		cmd.color = comp.color;
		return cmd;
	}

	ElLineCommand ElPencilRenderer::lineCTC(const ElVec2& pos, float rotation, const ElLineComponent& comp)
	{
		ElLineCommand cmd;
		cmd.start_pos = pos;
		cmd.end_offset = comp.end_offset;
		cmd.rotation = rotation;
		cmd.thickness = comp.thickness;
		cmd.color = comp.color;
		return cmd;
	}
}
