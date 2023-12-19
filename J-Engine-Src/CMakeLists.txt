cmake_minimum_required (VERSION 2.4...3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

project(JEngine-Dev)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DJENGINE_DEBUG")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Platform stuff
IF (WIN32)
	add_compile_definitions(WIN32_LEAN_AND_MEAN)
	add_compile_definitions(NOMINMAX)
	add_compile_definitions(JENGINE_PLATFORM_WINDOWS)
ELSE()
  # other platforms?
ENDIF(WIN32)

function(add_lib_for lib_name srcs purpose_name)
    add_library(${lib_name} STATIC ${srcs})
    target_compile_definitions(${lib_name} PRIVATE
        -Dfoo -Dbar # Common definitions
        -D${purpose_name} # Specific definition
    )
endfunction()

add_subdirectory("J-Engine")
add_subdirectory("Game")
add_subdirectory("J-Editor")
add_subdirectory("J-Player")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT JE-Editor)
