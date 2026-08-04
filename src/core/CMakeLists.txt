add_library(vortex_logger io/logger/vortex_logger.cpp)
target_link_libraries(vortex_logger PUBLIC quill::quill vortex_includes)

add_library(vortex_core 
    utilities/vortex_time.cpp
    io/filesystem/vortex_fstream.cpp
    io/filesystem/vortex_json.cpp
    ${GENERATED_HPP_FILE}
)
target_link_libraries(vortex_core PUBLIC vortex_logger json vortex_includes)