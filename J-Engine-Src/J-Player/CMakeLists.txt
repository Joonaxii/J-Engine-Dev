cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (JE-Player)
set(GAME_SOURCES )

set(PLAYER_SRC
	"src/main.cpp"
)
list(APPEND GAME_SOURCES ${PLAYER_SRC})

add_executable(JE-Player ${GAME_SOURCES})
target_link_libraries(JE-Player J-Engine-Player ImGUI Game-Player)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/spdlog/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/YAML/include")

set_target_properties(JE-Player PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Game/ )

set_target_properties(JE-Player  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "J-Player")
set_target_properties(JE-Player PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:JE-Player>"
                                              VS_DEBUGGER_COMMAND           "$<TARGET_FILE:JE-Player>"
                                              VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")
