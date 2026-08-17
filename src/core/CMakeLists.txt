add_library(el_logger io/logger/el_logger.cpp)
target_link_libraries(el_logger PUBLIC quill::quill el_includes)

add_library(el_core 
    utilities/el_time.cpp
    io/filesystem/el_fstream.cpp
    io/filesystem/el_json.cpp
    assets/asset_manager.cpp
    ${GENERATED_HPP_FILE}
)
target_link_libraries(el_core PUBLIC el_logger json el_includes)