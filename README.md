# Vortex Engine Namespace Guide

This document outlines the strict namespace architecture used throughout the Vortex Engine to ensure clean separation of concerns and AAA decoupling.

`namespace vortex {}` # The root namespace for the entire engine. Contains all major engine subsystems and modules.

## Engine Sub-Namespaces

- `namespace vortex::components {}` # Contains all standard ECS data components (e.g., Transform, RigidBody, HitBox, Sprite).
- `namespace vortex::ecs {}` # Contains the Data-Oriented Entity Component System framework (e.g., Registry, Entity).
- `namespace vortex::renderer {}` # Contains the hardware abstraction layer for graphics and drawing (e.g., RendererAPI, Color, Texture2D).
- `namespace vortex::systems {}` # Contains the core logic systems that operate on components (e.g., PhysicsSystem, RenderSystem).

## Core Sub-Namespaces

- `namespace vortex::containers {}` # Contains custom, highly-optimized data structures and collections (e.g., HashMap, Array).
- `namespace vortex::input {}` # Contains the hardware abstraction layer for input processing (Hardware key mapping).
- `namespace vortex::math {}` # Contains mathematical primitives and geometry operations (e.g., Vector2, Rectangle).
- `namespace vortex::utils {}` # Contains generic helper functions, algorithms, and engine utilities.


# Naming Conventions followed throughout the project

`Classes & Structs` : PascalCase	
`Functions & Methods`	: camelCase	
`Local Variables`	: snake_case  {For struct members too}
`Member Variables` : m_camelCase
`Constants & Macros` : UPPER_SNAKE_CASE
`Enums` : PascalCase {Enum name as well as Values}
`Namespaces` : snake_case