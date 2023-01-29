cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Game)
set(GAME_SOURCES )

set(GAME_SRC
	"include/SimpleRenderer.h"
	"src/SimpleRenderer.cpp"
	"src/Main.cpp"
)
source_group("Game" FILES ${GAME_SRC})
list(APPEND GAME_SOURCES ${GAME_SRC})

add_library(Game STATIC ${GAME_SOURCES})
target_link_libraries(Game J-Engine ImGUI)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")

# set_target_properties(Game  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${JENGINE_GAME_NAME})
# set_target_properties(Game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
#                                               VS_DEBUGGER_COMMAND           "$<TARGET_FILE:Game>"
#                                               VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")
