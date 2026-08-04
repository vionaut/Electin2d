add_library(vortex_input
    input_manager.cpp
    raylib_input.cpp
)
target_link_libraries(vortex_input PUBLIC vortex_includes PRIVATE raylib vortex_core)
