#include "el/renderer/render_system.hpp"
#include "el/components/render_components.hpp"
#include "el/components/physics_components.hpp" // For TransformComponent
#include "el/renderer/pencil_renderer.hpp"
#include "el/ecs/registry.hpp"

namespace el
{
    ElRenderSystem::ElRenderSystem(void* reg)
        : m_registry(reg)
    {
    }

    void ElRenderSystem::update()
    {
        ElRegistry* world_registry = reinterpret_cast<ElRegistry*>(m_registry);

        // --- SPRITES ---
        {
            auto view = world_registry->getView<ElSpriteComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& sprite = world_registry->getComponent<ElSpriteComponent>(ent);
                ElPencilRenderer::drawSprite(transform.position, transform.rotation, sprite);
            }
        }

        // --- RECTANGLES ---
        {
            auto view = world_registry->getView<ElRectComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& rect = world_registry->getComponent<ElRectComponent>(ent);
                ElPencilRenderer::drawRect(transform.position, transform.rotation, rect);
            }
        }

        // --- ROUNDED RECTANGLES ---
        {
            auto view = world_registry->getView<ElRoundedRectComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& rect = world_registry->getComponent<ElRoundedRectComponent>(ent);
                ElPencilRenderer::drawRoundedRect(transform.position, transform.rotation, rect);
            }
        }

        // --- CIRCLES ---
        {
            auto view = world_registry->getView<ElCircleComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& circle = world_registry->getComponent<ElCircleComponent>(ent);
                ElPencilRenderer::drawCircle(transform.position, circle); 
            }
        }

        // --- ELLIPSES ---
        {
            auto view = world_registry->getView<ElEllipseComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& ellipse = world_registry->getComponent<ElEllipseComponent>(ent);
                ElPencilRenderer::drawEllipse(transform.position, transform.rotation, ellipse);
            }
        }

        // --- RINGS ---
        {
            auto view = world_registry->getView<ElRingComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& ring = world_registry->getComponent<ElRingComponent>(ent);
                ElPencilRenderer::drawRing(transform.position, ring);
            }
        }

        // --- CAPSULES ---
        {
            auto view = world_registry->getView<ElCapsuleComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& capsule = world_registry->getComponent<ElCapsuleComponent>(ent);
                ElPencilRenderer::drawCapsule(transform.position, transform.rotation, capsule);
            }
        }

        // --- POLYGONS ---
        {
            auto view = world_registry->getView<ElPolygonComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& poly = world_registry->getComponent<ElPolygonComponent>(ent);
                ElPencilRenderer::drawPolygon(transform.position, transform.rotation, poly);
            }
        }

        // --- TRIANGLES ---
        {
            auto view = world_registry->getView<ElTriangleComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& tri = world_registry->getComponent<ElTriangleComponent>(ent);
                ElPencilRenderer::drawTriangle(transform.position, transform.rotation, tri);
            }
        }

        // --- LINES ---
        {
            auto view = world_registry->getView<ElLineComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& line = world_registry->getComponent<ElLineComponent>(ent);
                ElPencilRenderer::drawLine(transform.position, transform.rotation, line);
            }
        }

        // --- SPLINES ---
        {
            auto view = world_registry->getView<ElSplineComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& spline = world_registry->getComponent<ElSplineComponent>(ent);
                ElPencilRenderer::drawSpline(transform.position, transform.rotation, spline);
            }
        }

        // --- PIXELS ---
        {
            auto view = world_registry->getView<ElPixelComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& pixel = world_registry->getComponent<ElPixelComponent>(ent);
                ElPencilRenderer::drawPixel(transform.position, pixel);
            }
        }

        // --- MESHES ---
        {
            auto view = world_registry->getView<ElMeshComponent, ElTransformComponent>();
            for (ElEntity ent : view)
            {
                auto& transform = world_registry->getComponent<ElTransformComponent>(ent);
                auto& mesh = world_registry->getComponent<ElMeshComponent>(ent);
                ElPencilRenderer::drawMesh(transform.position, transform.rotation, mesh);
            }
        }
    }
}