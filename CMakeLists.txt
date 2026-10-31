cmake_minimum_required(VERSION 3.16)
project(sort_visualization CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# SFML 2.6.2 is downloaded and built as a static library. No manual install.
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)  # SFML 2.6 declares an old minimum version
FetchContent_Declare(SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/2.6.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON)
FetchContent_MakeAvailable(SFML)

# Embed the font in the executable, so it works from any working directory.
file(READ ${CMAKE_SOURCE_DIR}/Code/font/Ruda-Medium.ttf FONT_HEX HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," FONT_ARRAY "${FONT_HEX}")
file(WRITE ${CMAKE_BINARY_DIR}/font_data.cpp
    "extern const unsigned char font_data[] = {${FONT_ARRAY}};\n"
    "extern const unsigned long font_data_size = sizeof(font_data);\n")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Code/font/Ruda-Medium.ttf)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/Code/*.cpp)
add_executable(sort_visualization ${SOURCES} ${CMAKE_BINARY_DIR}/font_data.cpp)
target_link_libraries(sort_visualization PRIVATE sfml-graphics sfml-window sfml-system)
if(WIN32)
    target_compile_definitions(sort_visualization PRIVATE SFML_STATIC)
endif()
