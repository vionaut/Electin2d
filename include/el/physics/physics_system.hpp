#pragma once

#include "el/components/physics_components.hpp"
#include "el/core/math/el_math.hpp"

#include <cstdint>

namespace el
{
    class ElPhysicsSystem
    {
    public:
        ElPhysicsSystem(void *reg, ElVec2 gravity);
        ~ElPhysicsSystem();

        void update(float dt);
        void setTPS(int tps);
        void setGravity(ElVec2 gravity);
        void setSubStepCount(int count);

        void applyLinearImpulse(uint64_t ent, const ElVec2 &point, const ElVec2 &linear_impulse);
        void applyAngularImpulse(uint64_t ent, float angular_impulse);

        void applyForce(uint64_t ent, const ElVec2 &point, const ElVec2 &linear_force);
        void applyTorque(uint64_t ent, float torque);

        void setLinearVelocity(uint64_t ent, const ElVec2 &new_velocity);
        void setAngularVelocity(uint64_t ent, float new_angular_velocity);

        void setTransform(uint64_t ent, const ElTransformComponent &new_transform);

        template <ERigidBodyType BodyType>
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

        ElVec2 m_gravity; // The rate at which thou shall descend to the depth of hell, well since it's a vector, you can descend horizontally too

        void *m_registry; // As a good programmer(I hope) should! This holds the pointer to the registry to make changes into it i guess

        uint32_t m_worldId; // The box2d world id, that's right, we're giving out world ids now
    };
}