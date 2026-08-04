#include "vortex/ecs/registry.hpp"
#include "vortex/systems/physics_system.hpp"

#include "box2d/box2d.h"

namespace
{
    b2BodyType VxGet_b2BodyType(vortex::components::ERigidBodyType type)
    {
        switch (type)
        {
        case vortex::components::ERigidBodyType::Static:
            return b2_staticBody;
        case vortex::components::ERigidBodyType::Dynamic:
            return b2_dynamicBody;
        case vortex::components::ERigidBodyType::Kinematic:
            return b2_kinematicBody;
        }
    }
}

namespace vortex::systems
{
    VxPhysicsSystem::VxPhysicsSystem(void* reg, math::VxVec2 gravity)
        : m_timeStep(1.0f / 60.0f), m_timeStepBucket(0.0f), m_physicsSubStepCount(4), m_gravity(gravity), m_registry(reg)
    {
        b2WorldDef physics_world = b2DefaultWorldDef();
        physics_world.gravity = b2Vec2{ m_gravity.x, m_gravity.y };
        b2WorldId physics_world_id = b2CreateWorld(&physics_world);
        m_worldId = b2StoreWorldId(physics_world_id);
    }

    VxPhysicsSystem::~VxPhysicsSystem()
    {
        b2DestroyWorld(b2LoadWorldId(m_worldId));

        m_worldId = b2StoreWorldId(b2_nullWorldId);
    }

    void VxPhysicsSystem::update(float dt)
    {
        m_timeStepBucket += dt;
        // Hard capping bucket to avoid physics spiral of death
        if (m_timeStepBucket > 0.25f)
            m_timeStepBucket = 0.25f; // Maximum 15 time steps

        b2WorldId world_id = b2LoadWorldId(m_worldId);
        while (m_timeStepBucket >= m_timeStep)
        {
            m_timeStepBucket -= m_timeStep;

            b2World_Step(world_id, m_timeStep, m_physicsSubStepCount);
        }

        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto view = world_registry->getView<components::VxPhysicsRuntimeComponent, components::VxMotionComponent, components::VxTransformComponent>();

        for (ecs::VxEntity ent : view)
        {
            auto& b2_runtime_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;
            auto& transform = world_registry->getComponent<components::VxTransformComponent>(ent);
            auto& motion = world_registry->getComponent<components::VxMotionComponent>(ent);

            auto b2_body_id = b2LoadBodyId(b2_runtime_id);

            transform.prev_pos = transform.curr_pos;
            transform.curr_pos.x = b2Body_GetPosition(b2_body_id).x;
            transform.curr_pos.y = b2Body_GetPosition(b2_body_id).y;

            b2Rot rotation = b2Body_GetRotation(b2_body_id);
            transform.rotation = b2Rot_GetAngle(rotation);

            b2Vec2 b2_linear_velocity = b2Body_GetLinearVelocity(b2_body_id);
            motion.linear_velocity.x = b2_linear_velocity.x;
            motion.linear_velocity.y = b2_linear_velocity.y;

            motion.angular_velocity = b2Body_GetAngularVelocity(b2_body_id);
        }
    }

    void VxPhysicsSystem::setTPS(int tps)
    {
        m_timeStep = 1.0f / tps;
    }

    void VxPhysicsSystem::setGravity(math::VxVec2 gravity)
    {
        m_gravity = gravity;
    }

    void VxPhysicsSystem::setSubStepCount(int count)
    {
        m_physicsSubStepCount = count;
    }

    void VxPhysicsSystem::setTransform(uint64_t ent, const components::VxTransformComponent& new_transform)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;
        auto& old_transform = world_registry->getComponent<components::VxTransformComponent>(ent);

        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);
        b2Vec2 b2_new_transform = b2Vec2{ new_transform.curr_pos.x, new_transform.curr_pos.y };
        b2Rot b2_new_rotation = b2MakeRot(new_transform.rotation);

        old_transform = new_transform;

        b2Body_SetTransform(b2_id, b2_new_transform, b2_new_rotation);
    }

    void VxPhysicsSystem::setLinearVelocity(uint64_t ent, const math::VxVec2& new_velocity)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;
        auto& old_velocity = world_registry->getComponent<components::VxMotionComponent>(ent);

        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);
        b2Vec2 b2_new_linear_velocity = b2Vec2{ new_velocity.x, new_velocity.y };

        old_velocity.linear_velocity = new_velocity;

        b2Body_SetLinearVelocity(b2_id, b2_new_linear_velocity);
    }

    void VxPhysicsSystem::setAngularVelocity(uint64_t ent, float new_angular_velocity)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;
        auto& old_velocity = world_registry->getComponent<components::VxMotionComponent>(ent);

        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);
        b2Body_SetAngularVelocity(b2_id, new_angular_velocity);
    }

    void VxPhysicsSystem::applyLinearImpulse(uint64_t ent, const math::VxVec2& point, const math::VxVec2& linear_impulse)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;
        
        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);
        b2Vec2 b2_point = b2Vec2{ point.x, point.y };
        b2Vec2 b2_linear_impulse = b2Vec2{ linear_impulse.x, linear_impulse.y };

        b2Body_ApplyLinearImpulse(b2_id, b2_point, b2_linear_impulse, true);
    }

    void VxPhysicsSystem::applyAngularImpulse(uint64_t ent, float angular_impulse)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;

        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);

        b2Body_ApplyAngularImpulse(b2_id, angular_impulse, true);
    }

    void VxPhysicsSystem::applyForce(uint64_t ent, const math::VxVec2& point, const math::VxVec2& force)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;

        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);
        b2Vec2 b2_point = b2Vec2{ point.x, point.y };
        b2Vec2 b2_force = b2Vec2{ force.x, force.y };
        
        b2Body_ApplyForce(b2_id, b2_force, b2_point, true);
    }

    void VxPhysicsSystem::applyTorque(uint64_t ent, float torque)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& b2_uint_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;

        b2BodyId b2_id = b2LoadBodyId(b2_uint_id);

        b2Body_ApplyTorque(b2_id, torque, true);
    }

    void VxPhysicsSystem::createPhysicsEntity(uint64_t ent)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);

        auto& rigid_body = world_registry->getComponent<components::VxRigidBodyComponent>(ent);
        auto& transform = world_registry->getComponent<components::VxTransformComponent>(ent);
        auto& motion = world_registry->getComponent<components::VxMotionComponent>(ent);

        b2BodyDef b2_body = b2DefaultBodyDef();

        b2_body.type = VxGet_b2BodyType(rigid_body.body_type);

        b2_body.position = b2Vec2{ transform.curr_pos.x, transform.curr_pos.y };
        b2_body.rotation = b2MakeRot(transform.rotation);

        b2_body.linearVelocity = b2Vec2{ motion.linear_velocity.x, motion.linear_velocity.y };
        b2_body.angularVelocity = motion.angular_velocity;

        b2_body.linearDamping = rigid_body.linear_damping;
        b2_body.angularDamping = rigid_body.angular_damping;

        b2_body.gravityScale = rigid_body.gravity_scale;

        b2_body.fixedRotation = rigid_body.fixed_rotation;

        b2_body.userData = reinterpret_cast<void*>(ent);

        b2BodyId b2_entity = b2CreateBody(b2LoadWorldId(m_worldId), &b2_body);

        auto runtime_component = components::VxPhysicsRuntimeComponent{ b2StoreBodyId(b2_entity) };
        world_registry->addComponent<components::VxPhysicsRuntimeComponent>(ent, runtime_component);
    }

    void VxPhysicsSystem::destroyPhysicsEntity(uint64_t ent)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);
        auto& runtime_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;
        b2DestroyBody(b2LoadBodyId(runtime_id));
    }

    void VxPhysicsSystem::attachBoxCollider(uint64_t ent)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);
        auto& box_collider = world_registry->getComponent<components::VxBoxColliderComponent>(ent);
        auto& runtime_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;

        b2Polygon b2_box_collider = b2MakeOffsetBox(box_collider.half_extents.x, box_collider.half_extents.y, b2Vec2{ box_collider.center_offset.x, box_collider.center_offset.y }, b2MakeRot(box_collider.rotation_offset));

        b2ShapeDef b2_shape_def = b2DefaultShapeDef();
        b2_shape_def.density = box_collider.density;
        b2_shape_def.material.friction = box_collider.friction;
        b2_shape_def.material.restitution = box_collider.restitution;

        b2_shape_def.isSensor = box_collider.is_sensor;

        b2CreatePolygonShape(b2LoadBodyId(runtime_id), &b2_shape_def, &b2_box_collider);
    }

    void VxPhysicsSystem::attachCircleCollider(uint64_t ent)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);
        auto& circle_collider = world_registry->getComponent<components::VxCircleColliderComponent>(ent);
        auto& runtime_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;

        b2Circle b2_circle_collider = { {circle_collider.center_offset.x, circle_collider.center_offset.y}, circle_collider.radius };

        b2ShapeDef b2_shape_def = b2DefaultShapeDef();
        b2_shape_def.density = circle_collider.density;
        b2_shape_def.material.friction = circle_collider.friction;
        b2_shape_def.material.restitution = circle_collider.restitution;

        b2_shape_def.isSensor = circle_collider.is_sensor;

        b2CreateCircleShape(b2LoadBodyId(runtime_id), &b2_shape_def, &b2_circle_collider);
    }

    void VxPhysicsSystem::attachCapsuleCollider(uint64_t ent)
    {
        ecs::VxRegistry* world_registry = reinterpret_cast<ecs::VxRegistry*>(m_registry);
        auto& capsule_collider = world_registry->getComponent<components::VxCapsuleColliderComponent>(ent);
        auto& runtime_id = world_registry->getComponent<components::VxPhysicsRuntimeComponent>(ent).physics_body_id;

        b2CosSin rotation_cos_sin = b2ComputeCosSin(capsule_collider.rotation_offset);

        float capsule_half_length = capsule_collider.length / 2;

        b2Vec2 c1 = { capsule_collider.center_offset.x - (capsule_half_length * rotation_cos_sin.sine), capsule_collider.center_offset.y + (capsule_half_length * rotation_cos_sin.cosine) };

        b2Vec2 c2 = { capsule_collider.center_offset.x + (capsule_half_length * rotation_cos_sin.sine), capsule_collider.center_offset.y - (capsule_half_length * rotation_cos_sin.cosine) };

        b2Capsule b2_capsule_collider = { c1, c2, capsule_collider.radius };

        b2ShapeDef b2_shape_def = b2DefaultShapeDef();
        b2_shape_def.density = capsule_collider.density;
        b2_shape_def.material.friction = capsule_collider.friction;
        b2_shape_def.material.restitution = capsule_collider.restitution;

        b2_shape_def.isSensor = capsule_collider.is_sensor;

        b2CreateCapsuleShape(b2LoadBodyId(runtime_id), &b2_shape_def, &b2_capsule_collider);
    }
}