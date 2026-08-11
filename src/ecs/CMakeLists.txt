add_library(vortex_ecs 
    entity_manager.cpp
)
target_link_libraries(vortex_ecs PUBLIC vortex_includes vortex_core)