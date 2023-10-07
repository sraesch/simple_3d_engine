cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(EngineSystem)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(libraries
    GLEW::glew
)

#=============================================================================
# Collect sources and headers of this library

file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

source_group("Source Files" FILES ${sources})
source_group("Header Files" FILES ${headers})

#=============================================================================
# Define the library that should be built

add_library(${PROJECT_NAME} SHARED ${sources} ${headers})

#=============================================================================
# Define the include paths
set(includes_public
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#=============================================================================
# Set all includes

target_link_libraries(${PROJECT_NAME} PUBLIC ${libraries})
target_include_directories(${PROJECT_NAME} PUBLIC ${includes_public})