add_library(el_ecs 
    entity_manager.cpp
)
target_link_libraries(el_ecs PUBLIC el_includes el_core)