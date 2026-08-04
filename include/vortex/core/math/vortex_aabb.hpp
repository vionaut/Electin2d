#pragma once

#include "vortex/core/math/vortex_math.hpp"
#include "vortex/components/physics_components.hpp"

namespace vortex::math
{
    struct VxAABB // Axis-aligned bounding box
    {
        VxVec2 min;
        VxVec2 max;
    };

    inline VxAABB getAABB(const vortex::components::VxColliderComponent &collider, const vortex::components::VxTransformComponent &transform)
    {
        VxVec2 min{0, 0};
        VxVec2 max{0, 0};
        VxVec2 center{0, 0};

        switch (collider.shape_type)
        {
        case vortex::components::ECollisionShape::AABB:
            center = {
                transform.curr_pos.x + collider.shape.aabb.offset.x,
                transform.curr_pos.y + collider.shape.aabb.offset.y};

            min = {center.x - collider.shape.aabb.half_extents.x, center.y - collider.shape.aabb.half_extents.y};
            max = {center.x + collider.shape.aabb.half_extents.x, center.y + collider.shape.aabb.half_extents.y};

            break;

        case vortex::components::ECollisionShape::Circle:
            center = {
                transform.curr_pos.x + collider.shape.circle.offset.x,
                transform.curr_pos.y + collider.shape.circle.offset.y};

            min = {center.x - collider.shape.circle.radius, center.y - collider.shape.circle.radius};
            max = {center.x + collider.shape.circle.radius, center.y + collider.shape.circle.radius};

            break;
        }

        return VxAABB{min, max};
    };
}