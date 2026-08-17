add_library(el_renderer
    raylib_renderer.cpp
    pencil_renderer.cpp
    render_system.cpp
)
target_link_libraries(el_renderer PUBLIC el_includes el_core PRIVATE raylib el_ecs)
