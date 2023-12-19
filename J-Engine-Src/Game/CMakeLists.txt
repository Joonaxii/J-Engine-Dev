set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Game-Editor)
project (Game-Player)
set(GAME_SOURCES )
set(GAME_SOURCES_EDITOR )

set(GAME_SRC
	"include/SimpleRenderer.h"
	"src/SimpleRenderer.cpp"
	"src/Main.cpp"
)
source_group("Game" FILES ${GAME_SRC})
list(APPEND GAME_SOURCES ${GAME_SRC})

# Custom GUI related stuff goes here
set(GAME_SRC )
list(APPEND GAME_SOURCES_EDITOR ${GAME_SOURCES})

add_lib_for(Game-Editor "${GAME_SOURCES_EDITOR}" JENGINE_EDITOR)
add_lib_for(Game-Player "${GAME_SOURCES}" JENGINE_PLAYER)

target_link_libraries(Game-Player J-Engine-Player ImGUI)
target_link_libraries(Game-Editor J-Engine-Editor ImGUI)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/spdlog/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/YAML/include")

# set_target_properties(Game  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${JENGINE_GAME_NAME})
# set_target_properties(Game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
#                                               VS_DEBUGGER_COMMAND           "$<TARGET_FILE:Game>"
#                                               VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")
