add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(input)
add_subdirectory(renderer)
add_subdirectory(systems)
add_subdirectory(audio)

# UMBRELLA TARGET
add_library(vortex INTERFACE)

# Create an alias so consumers can use vortex::vortex
add_library(vortex::vortex ALIAS vortex)

target_link_libraries(vortex INTERFACE
    vortex_includes
    vortex_core
    vortex_input
    vortex_ecs
    vortex_renderer
    vortex_physics_system
    vortex_logger
    vortex_audio
)