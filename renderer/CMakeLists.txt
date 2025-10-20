cmake_minimum_required(VERSION 3.14)

# Gather all source files recursively inside src/
file(GLOB_RECURSE RENDERER_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Glad library (single C source + headers)
add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src/glad.c")

target_include_directories(glad PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include"
)

# Openrenderer static library
add_library(openrenderer STATIC
        ${RENDERER_SOURCES}
)

target_include_directories(openrenderer
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(openrenderer PRIVATE glad)

set_target_properties(openrenderer PROPERTIES LINKER_LANGUAGE CXX)
