cmake_minimum_required (VERSION 2.4...3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

project(JEngine-Game)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

add_subdirectory("J-Engine")
add_subdirectory("Game")
add_subdirectory("Game-Bootstrap")
add_subdirectory("J-Engine-Editor")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT J-Engine-Editor)
