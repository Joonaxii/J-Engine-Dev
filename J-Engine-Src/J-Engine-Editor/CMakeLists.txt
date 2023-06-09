set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("J-Engine-Editor")
set(JENGINE_EDITOR_SOURCES )

set(JENGINE_MAIN_SRC
	"src/JEditorApp.cpp"
)
source_group("JEngine" FILES ${JENGINE_MAIN_SRC})
list(APPEND JENGINE_EDITOR_SOURCES ${JENGINE_MAIN_SRC})

add_executable(J-Engine-Editor ${JENGINE_EDITOR_SOURCES})
target_link_libraries(J-Engine-Editor J-Engine ImGUI Game)

set_target_properties(J-Engine-Editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Editor/ )

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/spdlog/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/YAML/include")


set_target_properties(J-Engine-Editor  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "J-Engine-Editor")
set_target_properties(J-Engine-Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:J-Engine-Editor>"
                                              VS_DEBUGGER_COMMAND           "$<TARGET_FILE:J-Engine-Editor>"
                                              VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")