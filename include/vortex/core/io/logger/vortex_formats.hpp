#pragma once

#include "quill/bundled/fmt/format.h"
#include "quill/DirectFormatCodec.h"
#include "vortex/components/physics_components.hpp"

#define VX_GENERATE_QUILL_CODEC(ComponentType)                                   \
	template <>                                                                  \
	struct quill::Codec<ComponentType> : quill::DirectFormatCodec<ComponentType> \
	{                                                                            \
	};

VX_GENERATE_QUILL_CODEC(vortex::components::VxTransformComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxInterpolationComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxBoxColliderComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxCircleColliderComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxCapsuleColliderComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxRigidBodyComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxMotionComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxPhysicsRuntimeComponent)
VX_GENERATE_QUILL_CODEC(vortex::components::VxForceAccumulatorComponent)

#define VX_GENERATE_FORMATTER_PARSE(ComponentType)                                                                  \
	bool name_only = false;                                                                                         \
	constexpr auto parse(fmtquill::format_parse_context &ctx)                                                       \
	{                                                                                                               \
		auto it = ctx.begin();                                                                                      \
		auto end = ctx.end();                                                                                       \
		if (it != end && *it != '}')                                                                                \
		{                                                                                                           \
			if (it + 4 <= end && it[0] == 'n' && it[1] == 'a' && it[2] == 'm' && it[3] == 'e')                      \
			{                                                                                                       \
				name_only = true;                                                                                   \
				it += 4;                                                                                            \
			}                                                                                                       \
			else                                                                                                    \
			{                                                                                                       \
				throw fmtquill::format_error("Invalid format specifier for " #ComponentType ". Use {} or {:name}"); \
			}                                                                                                       \
		}                                                                                                           \
		return it;                                                                                                  \
	}

template <>
struct fmtquill::formatter<vortex::components::ERigidBodyType>
{
	// We don't even need custom parsing here, just use standard {}
	constexpr auto parse(fmtquill::format_parse_context &ctx) { return ctx.begin(); }

	auto format(const vortex::components::ERigidBodyType &type, fmtquill::format_context &ctx) const
	{
		switch (type)
		{
		case vortex::components::ERigidBodyType::Static:
			return fmtquill::format_to(ctx.out(), "Static");
		case vortex::components::ERigidBodyType::Kinematic:
			return fmtquill::format_to(ctx.out(), "Kinematic");
		case vortex::components::ERigidBodyType::Dynamic:
			return fmtquill::format_to(ctx.out(), "Dynamic");
		default:
			return fmtquill::format_to(ctx.out(), "Unknown");
		}
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxTransformComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxTransformComponent)

	auto format(const vortex::components::VxTransformComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxTransformComponent");
		return fmtquill::format_to(ctx.out(), "VxTransformComponent: [Pos({:.2f}, {:.2f}), Rot: {:.2f}]", t.position.x, t.position.y, t.rotation);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxInterpolationComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxInterpolationComponent)

	auto format(const vortex::components::VxInterpolationComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxInterpolationComponent");
		return fmtquill::format_to(ctx.out(), "VxInterpolationComponent: [Prev_pos({:.2f}, {:.2f}), Prev_rot: {:.2f}]", t.prev_pos.x, t.prev_pos.y, t.prev_rotation);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxBoxColliderComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxBoxColliderComponent)

	auto format(const vortex::components::VxBoxColliderComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxBoxColliderComponent");
		return fmtquill::format_to(ctx.out(), "VxBoxColliderComponent: [Half_Extents({:.2f}, {:.2f}), Center_Offset({:.2f}, {:.2f}), Rotation_Offset({:.2f}), Density({:.2f}), Friction({:.2f}), Restitution({:.2f}), Is_Sensor({})]",
								   t.half_extents.x, t.half_extents.y, t.center_offset.x, t.center_offset.y, t.rotation_offset, t.density, t.friction, t.restitution, t.is_sensor);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxCircleColliderComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxCircleColliderComponent)

	auto format(const vortex::components::VxCircleColliderComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxCircleColliderComponent");
		return fmtquill::format_to(ctx.out(), "VxCircleColliderComponent: [Radius({:.2f}), Center_Offset({:.2f}, {:.2f}), Density({:.2f}), Friction({:.2f}), Restitution({:.2f}), Is_Sensor({})]",
								   t.radius, t.center_offset.x, t.center_offset.y, t.density, t.friction, t.restitution, t.is_sensor);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxCapsuleColliderComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxCapsuleColliderComponent)

	auto format(const vortex::components::VxCapsuleColliderComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxCapsuleColliderComponent");
		return fmtquill::format_to(ctx.out(), "VxCapsuleColliderComponent: [Length({:.2f}), Radius({:.2f}), Center_Offset({:.2f}, {:.2f}), Rotation_Offset({:.2f}), Density({:.2f}), Friction({:.2f}), Restitution({:.2f}), Is_Sensor({})]",
								   t.length, t.radius, t.center_offset.x, t.center_offset.y, t.rotation_offset, t.density, t.friction, t.restitution, t.is_sensor);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxRigidBodyComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxRigidBodyComponent)

	auto format(const vortex::components::VxRigidBodyComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxRigidBodyComponent");
		return fmtquill::format_to(ctx.out(), "VxRigidBodyComponent: [Linear_Damping({:.2f}), Angular_Damping({:.2f}), Gravity_Scale({:.2f}), Body_Type({}), Fixed_Rotation({})]",
								   t.linear_damping, t.angular_damping, t.gravity_scale, t.body_type, t.fixed_rotation);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxMotionComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxMotionComponent)

	auto format(const vortex::components::VxMotionComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxMotionComponent");
		return fmtquill::format_to(ctx.out(), "VxMotionComponent: [Linear_Velocity({:.2f}, {:.2f}), Angular_Velocity({:.2f})]",
								   t.linear_velocity.x, t.linear_velocity.y, t.angular_velocity);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxPhysicsRuntimeComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxPhysicsRuntimeComponent)

	auto format(const vortex::components::VxPhysicsRuntimeComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxPhysicsRuntimeComponent");
		return fmtquill::format_to(ctx.out(), "VxPhysicsRuntimeComponent: [Body_ID({})]", t.physics_body_id);
	}
};

template <>
struct fmtquill::formatter<vortex::components::VxForceAccumulatorComponent>
{
	VX_GENERATE_FORMATTER_PARSE(VxForceAccumulatorComponent)

	auto format(const vortex::components::VxForceAccumulatorComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "VxForceAccumulatorComponent");
		return fmtquill::format_to(ctx.out(), "VxForceAccumulatorComponent: [Net_Force({:.2f}, {:.2f}), Net_Torque({:.2f})]",
								   t.net_force.x, t.net_force.y, t.net_torque);
	}
};