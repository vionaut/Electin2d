add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(input)
add_subdirectory(renderer)
add_subdirectory(physics)
add_subdirectory(audio)

# UMBRELLA TARGET
add_library(electin INTERFACE)

# Create an alias so consumers can use el::electin
add_library(el::electin ALIAS electin)

target_link_libraries(electin INTERFACE
    el_includes
    el_core
    el_input
    el_ecs
    el_renderer
    el_physics_system
    el_logger
    el_audio
)

# PERMANENT FIX: Force all C++ compilation targets to wait for the Python script
add_dependencies(el_core GenerateConfig)
add_dependencies(el_ecs GenerateConfig)
add_dependencies(el_input GenerateConfig)
add_dependencies(el_renderer GenerateConfig)
add_dependencies(el_physics_system GenerateConfig)
add_dependencies(el_logger GenerateConfig)
add_dependencies(el_audio GenerateConfig)