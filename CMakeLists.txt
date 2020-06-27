cmake_minimum_required(VERSION 3.0.0)
project(game VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON )

include(CTest)
enable_testing()

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

#add_subdirectory(third-party/ImGuiFileDialog/)
add_subdirectory(third-party/nfd/)
include_directories (
    third-party/
    third-party/imgui
    third-party/FastNoise
    third-party/nfd/src/include
    #third-party/ImGuiFileDialog/
    include/
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

set(IMGUI_SRC
    third-party/imgui/examples/imgui_impl_sdl.cpp
    third-party/imgui/examples/imgui_impl_opengl3.cpp
    third-party/imgui/imgui_widgets.cpp
    third-party/imgui/imgui_draw.cpp
    third-party/imgui/imgui.cpp
    #third-party/ImGuiFileDialog/ImGuiFileDialog/ImGuiFileDialog.cpp
    )

set(SRCS 
    src/cam.cpp
    src/game.cpp
    src/tile.cpp
    src/tile_array.cpp
    src/chunk.cpp
    src/map.cpp
    src/map_obj.cpp
    src/material.cpp
    src/materialistic.cpp
    src/nameable.cpp
    third-party/FastNoise/FastNoise.cpp
    )

add_executable(game main.cpp 
    ${SRCS}
    ${IMGUI_SRC}
    )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native")

target_link_libraries (game PRIVATE
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    #ImGuiFileDialog
    pthread
    nfd
    )
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
