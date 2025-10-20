cmake_minimum_required(VERSION 3.14)

project(openrender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(renderer)

option(BUILD_EDITOR "Build OpenRenderer Editor" OFF)

if (BUILD_EDITOR)
    add_subdirectory(editor)
endif()