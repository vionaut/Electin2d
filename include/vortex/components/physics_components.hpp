#pragma once

#include <cstdint>
#include "vortex/core/math/vortex_math.hpp"

namespace vortex::components
{
	enum class ERigidBodyType : uint8_t
	{
		Static = 0,
		Kinematic = 1,
		Dynamic = 2
	};

	// Component Structs-------------------------------------------------------------------------------------------------

	struct VxTransformComponent
	{
		math::VxVec2 position;
		float rotation;
	};

	struct VxInterpolationComponent
	{
		math::VxVec2 prev_pos;
		float prev_rotation;
	};

	struct VxBoxColliderComponent
	{
		math::VxVec2 half_extents;
		math::VxVec2 center_offset;
		float rotation_offset;

		float density;
		float friction;
		float restitution;

		bool is_sensor;
	};

	struct VxCircleColliderComponent
	{
		float radius;
		math::VxVec2 center_offset;

		float density;
		float friction;
		float restitution;

		bool is_sensor;
	};

	struct VxCapsuleColliderComponent
	{
		float length, radius;
		math::VxVec2 center_offset;
		float rotation_offset;

		float density;
		float friction;
		float restitution;

		bool is_sensor;
	};

	struct VxRigidBodyComponent
	{
		float linear_damping;
		float angular_damping;
		float gravity_scale;

		ERigidBodyType body_type;

		bool fixed_rotation;
	};

	struct VxMotionComponent
	{
		math::VxVec2 linear_velocity;
		float angular_velocity;
	};

	struct VxPhysicsRuntimeComponent
	{
		uint64_t physics_body_id;
	};

	struct VxForceAccumulatorComponent
	{
		math::VxVec2 net_force;
		float net_torque;
	};
}