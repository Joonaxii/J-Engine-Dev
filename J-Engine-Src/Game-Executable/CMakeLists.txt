cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Game-Executable)
set(BOOTSTRAP_SOURCES )

set(BOOTSTRAP_SRC
	"src/Main.cpp"
)
list(APPEND BOOTSTRAP_SOURCES ${BOOTSTRAP_SRC})

add_executable(Game-Executable ${GAME_SOURCES})
target_link_libraries(Game-Executable J-Engine Game)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
# include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
# include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")

set_target_properties(Game-Executable PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Game/ )

set_target_properties(Game-Executable  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Game")
set_target_properties(Game-Executable PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game-Executable>"
                                              VS_DEBUGGER_COMMAND           "$<TARGET_FILE:Game-Executable>"
                                              VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")
