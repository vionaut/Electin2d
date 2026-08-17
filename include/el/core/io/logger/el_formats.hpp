#pragma once

#include "quill/bundled/fmt/format.h"
#include "quill/DirectFormatCodec.h"
#include "el/components/physics_components.hpp"
#include "el/components/render_components.hpp"

#define EL_GENERATE_QUILL_CODEC(ComponentType)                                   \
	template <>                                                                  \
	struct quill::Codec<ComponentType> : quill::DirectFormatCodec<ComponentType> \
	{                                                                            \
	};

EL_GENERATE_QUILL_CODEC(el::ElTransformComponent)
EL_GENERATE_QUILL_CODEC(el::ElInterpolationComponent)
EL_GENERATE_QUILL_CODEC(el::ElBoxColliderComponent)
EL_GENERATE_QUILL_CODEC(el::ElCircleColliderComponent)
EL_GENERATE_QUILL_CODEC(el::ElCapsuleColliderComponent)
EL_GENERATE_QUILL_CODEC(el::ElRigidBodyComponent)
EL_GENERATE_QUILL_CODEC(el::ElMotionComponent)
EL_GENERATE_QUILL_CODEC(el::ElPhysicsRuntimeComponent)
EL_GENERATE_QUILL_CODEC(el::ElForceAccumulatorComponent)

EL_GENERATE_QUILL_CODEC(el::ElSpriteComponent)
EL_GENERATE_QUILL_CODEC(el::ElRectComponent)
EL_GENERATE_QUILL_CODEC(el::ElRoundedRectComponent)
EL_GENERATE_QUILL_CODEC(el::ElCircleComponent)
EL_GENERATE_QUILL_CODEC(el::ElEllipseComponent)
EL_GENERATE_QUILL_CODEC(el::ElRingComponent)
EL_GENERATE_QUILL_CODEC(el::ElCapsuleComponent)
EL_GENERATE_QUILL_CODEC(el::ElPolygonComponent)
EL_GENERATE_QUILL_CODEC(el::ElTriangleComponent)
EL_GENERATE_QUILL_CODEC(el::ElLineComponent)
EL_GENERATE_QUILL_CODEC(el::ElSplineComponent)
EL_GENERATE_QUILL_CODEC(el::ElPixelComponent)
EL_GENERATE_QUILL_CODEC(el::ElMeshComponent)

#define EL_GENERATE_FORMATTER_PARSE(ComponentType)                                                                  \
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

#define EL_GENERATE_BASIC_FORMATTER(ComponentType)                                          \
	template <>                                                                             \
	struct fmtquill::formatter<ComponentType>                                               \
	{                                                                                       \
		EL_GENERATE_FORMATTER_PARSE(ComponentType)                                          \
		auto format(const ComponentType &t, fmtquill::format_context &ctx) const            \
		{                                                                                   \
			return fmtquill::format_to(ctx.out(), #ComponentType);                          \
		}                                                                                   \
	};

EL_GENERATE_BASIC_FORMATTER(el::ElSpriteComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElRectComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElRoundedRectComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElCircleComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElEllipseComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElRingComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElCapsuleComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElPolygonComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElTriangleComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElLineComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElSplineComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElPixelComponent)
EL_GENERATE_BASIC_FORMATTER(el::ElMeshComponent)

template <>
struct fmtquill::formatter<el::ERigidBodyType>
{
	// We don't even need custom parsing here, just use standard {}
	constexpr auto parse(fmtquill::format_parse_context &ctx) { return ctx.begin(); }

	auto format(const el::ERigidBodyType &type, fmtquill::format_context &ctx) const
	{
		switch (type)
		{
		case el::ERigidBodyType::Static:
			return fmtquill::format_to(ctx.out(), "Static");
		case el::ERigidBodyType::Kinematic:
			return fmtquill::format_to(ctx.out(), "Kinematic");
		case el::ERigidBodyType::Dynamic:
			return fmtquill::format_to(ctx.out(), "Dynamic");
		default:
			return fmtquill::format_to(ctx.out(), "Unknown");
		}
	}
};

template <>
struct fmtquill::formatter<el::ElTransformComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElTransformComponent)

	auto format(const el::ElTransformComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElTransformComponent");
		return fmtquill::format_to(ctx.out(), "ElTransformComponent: [Pos({:.2f}, {:.2f}), Rot: {:.2f}]", t.position.x, t.position.y, t.rotation);
	}
};

template <>
struct fmtquill::formatter<el::ElInterpolationComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElInterpolationComponent)

	auto format(const el::ElInterpolationComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElInterpolationComponent");
		return fmtquill::format_to(ctx.out(), "ElInterpolationComponent: [Prev_pos({:.2f}, {:.2f}), Prev_rot: {:.2f}]", t.prev_pos.x, t.prev_pos.y, t.prev_rotation);
	}
};

template <>
struct fmtquill::formatter<el::ElBoxColliderComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElBoxColliderComponent)

	auto format(const el::ElBoxColliderComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElBoxColliderComponent");
		return fmtquill::format_to(ctx.out(), "ElBoxColliderComponent: [Half_Extents({:.2f}, {:.2f}), Center_Offset({:.2f}, {:.2f}), Rotation_Offset({:.2f}), Density({:.2f}), Friction({:.2f}), Restitution({:.2f}), Is_Sensor({})]",
								   t.half_extents.x, t.half_extents.y, t.center_offset.x, t.center_offset.y, t.rotation_offset, t.density, t.friction, t.restitution, t.is_sensor);
	}
};

template <>
struct fmtquill::formatter<el::ElCircleColliderComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElCircleColliderComponent)

	auto format(const el::ElCircleColliderComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElCircleColliderComponent");
		return fmtquill::format_to(ctx.out(), "ElCircleColliderComponent: [Radius({:.2f}), Center_Offset({:.2f}, {:.2f}), Density({:.2f}), Friction({:.2f}), Restitution({:.2f}), Is_Sensor({})]",
								   t.radius, t.center_offset.x, t.center_offset.y, t.density, t.friction, t.restitution, t.is_sensor);
	}
};

template <>
struct fmtquill::formatter<el::ElCapsuleColliderComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElCapsuleColliderComponent)

	auto format(const el::ElCapsuleColliderComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElCapsuleColliderComponent");
		return fmtquill::format_to(ctx.out(), "ElCapsuleColliderComponent: [Half_Length({:.2f}), Radius({:.2f}), Center_Offset({:.2f}, {:.2f}), Rotation_Offset({:.2f}), Density({:.2f}), Friction({:.2f}), Restitution({:.2f}), Is_Sensor({})]",
								   t.half_length, t.radius, t.center_offset.x, t.center_offset.y, t.rotation_offset, t.density, t.friction, t.restitution, t.is_sensor);
	}
};

template <>
struct fmtquill::formatter<el::ElRigidBodyComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElRigidBodyComponent)

	auto format(const el::ElRigidBodyComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElRigidBodyComponent");
		return fmtquill::format_to(ctx.out(), "ElRigidBodyComponent: [Linear_Damping({:.2f}), Angular_Damping({:.2f}), Gravity_Scale({:.2f}), Body_Type({}), Fixed_Rotation({})]",
								   t.linear_damping, t.angular_damping, t.gravity_scale, t.body_type, t.fixed_rotation);
	}
};

template <>
struct fmtquill::formatter<el::ElMotionComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElMotionComponent)

	auto format(const el::ElMotionComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElMotionComponent");
		return fmtquill::format_to(ctx.out(), "ElMotionComponent: [Linear_Velocity({:.2f}, {:.2f}), Angular_Velocity({:.2f})]",
								   t.linear_velocity.x, t.linear_velocity.y, t.angular_velocity);
	}
};

template <>
struct fmtquill::formatter<el::ElPhysicsRuntimeComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElPhysicsRuntimeComponent)

	auto format(const el::ElPhysicsRuntimeComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElPhysicsRuntimeComponent");
		return fmtquill::format_to(ctx.out(), "ElPhysicsRuntimeComponent: [Body_ID({})]", t.physics_body_id);
	}
};

template <>
struct fmtquill::formatter<el::ElForceAccumulatorComponent>
{
	EL_GENERATE_FORMATTER_PARSE(ElForceAccumulatorComponent)

	auto format(const el::ElForceAccumulatorComponent &t, fmtquill::format_context &ctx) const
	{
		if (name_only)
			return fmtquill::format_to(ctx.out(), "ElForceAccumulatorComponent");
		return fmtquill::format_to(ctx.out(), "ElForceAccumulatorComponent: [Net_Force({:.2f}, {:.2f}), Net_Torque({:.2f})]",
								   t.net_force.x, t.net_force.y, t.net_torque);
	}
};