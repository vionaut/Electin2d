add_library(el_input
    input_manager.cpp
    raylib_input.cpp
)
target_link_libraries(el_input PUBLIC el_includes PRIVATE raylib el_core)
