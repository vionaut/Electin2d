#pragma once

#include <cstdint>
#include "el/core/math/el_math.hpp"

namespace el
{
	enum class ERigidBodyType : uint8_t
	{
		Static = 0,
		Kinematic = 1,
		Dynamic = 2
	};

	// Component Structs-------------------------------------------------------------------------------------------------

	struct ElTransformComponent
	{
		ElVec2 position;
		float rotation;
	};

	struct ElInterpolationComponent
	{
		ElVec2 prev_pos;
		float prev_rotation;
	};

	struct ElBoxColliderComponent
	{
		ElVec2 half_extents;
		ElVec2 center_offset;
		float rotation_offset;

		float density;
		float friction;
		float restitution;

		bool is_sensor;
	};

	struct ElCircleColliderComponent
	{
		float radius;
		ElVec2 center_offset;

		float density;
		float friction;
		float restitution;

		bool is_sensor;
	};

	struct ElCapsuleColliderComponent
	{
		float half_length, radius;
		ElVec2 center_offset;
		float rotation_offset;

		float density;
		float friction;
		float restitution;

		bool is_sensor;
	};

	struct ElRigidBodyComponent
	{
		float linear_damping;
		float angular_damping;
		float gravity_scale;

		ERigidBodyType body_type;

		bool fixed_rotation;
	};

	struct ElMotionComponent
	{
		ElVec2 linear_velocity;
		float angular_velocity;
	};

	struct ElPhysicsRuntimeComponent
	{
		uint64_t physics_body_id;
	};

	struct ElForceAccumulatorComponent
	{
		ElVec2 net_force;
		float net_torque;
	};
}