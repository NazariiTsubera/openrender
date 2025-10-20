cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.hpp" "src/*.h")

add_executable(demo ${SOURCE_FILES})


target_link_libraries(demo PUBLIC openrenderer)
target_link_libraries(demo PRIVATE glfw)
