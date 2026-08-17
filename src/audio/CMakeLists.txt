add_library(el_audio raylib_audio.cpp)
target_link_libraries(el_audio PUBLIC el_includes PRIVATE raylib)