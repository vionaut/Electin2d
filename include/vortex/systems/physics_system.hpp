#pragma once

#include "vortex/components/physics_components.hpp"
#include "vortex/core/math/vortex_math.hpp"

#include <cstdint>

namespace vortex::systems
{
    class VxPhysicsSystem
    {
    public:
        VxPhysicsSystem(void *reg, math::VxVec2 gravity);
        ~VxPhysicsSystem();

        void update(float dt);
        void setTPS(int tps);
        void setGravity(math::VxVec2 gravity);
        void setSubStepCount(int count);

        void applyLinearImpulse(uint64_t ent, const math::VxVec2 &point, const math::VxVec2 &linear_impulse);
        void applyAngularImpulse(uint64_t ent, float angular_impulse);

        void applyForce(uint64_t ent, const math::VxVec2 &point, const math::VxVec2 &linear_force);
        void applyTorque(uint64_t ent, float torque);

        void setLinearVelocity(uint64_t ent, const math::VxVec2 &new_velocity);
        void setAngularVelocity(uint64_t ent, float new_angular_velocity);

        void setTransform(uint64_t ent, const components::VxTransformComponent &new_transform);

        void createPhysicsEntity(uint64_t ent);  // Physics system needs to create an internal physics entity after the entity manager to apply physics on it
        void destroyPhysicsEntity(uint64_t ent); // To destroy the said entity

        void attachBoxCollider(uint64_t ent);
        void attachCircleCollider(uint64_t ent);
        void attachCapsuleCollider(uint64_t ent);

    private:
        // Private functions---

        // Member variables---

        float m_timeStep; // Physics time step (default = 16.67 ms at 60 tps)

        float m_timeStepBucket; // I'll explain this in documentation

        int m_physicsSubStepCount; // Internal physics sub step count (default = 4)

        math::VxVec2 m_gravity; // The rate at which thou shall descend to the depth of hell, well since it's a vector, you can descend horizontally too

        void *m_registry; // As a good programmer(I hope) should! This holds the pointer to the registry to make changes into it i guess

        uint32_t m_worldId; // The box2d world id, that's right, we're giving out world ids now
    };
}