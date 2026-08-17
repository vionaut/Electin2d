add_library(el_physics_system 
    "physics_system.cpp")
target_link_libraries(el_physics_system PUBLIC el_includes el_core PRIVATE box2d::box2d el_ecs)
