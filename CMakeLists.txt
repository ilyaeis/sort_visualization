cmake_minimum_required(VERSION 3.10)
project(sort_visualization CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/Code/*.cpp)
add_executable(sort_visualization ${SOURCES})
target_link_libraries(sort_visualization PRIVATE sfml-graphics sfml-window sfml-system)

# The program loads "font/Ruda-Medium.ttf" relative to the working directory.
add_custom_command(TARGET sort_visualization POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Code/font $<TARGET_FILE_DIR:sort_visualization>/font)
