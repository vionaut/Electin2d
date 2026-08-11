add_library(vortex_physics_system 
    physics_system.cpp
 "pencil_renderer.cpp")
target_link_libraries(vortex_physics_system PUBLIC vortex_includes vortex_core PRIVATE box2d::box2d vortex_ecs)
