add_library(vortex_renderer
    raylib_renderer.cpp
)
target_link_libraries(vortex_renderer PUBLIC vortex_includes vortex_core PRIVATE raylib)
