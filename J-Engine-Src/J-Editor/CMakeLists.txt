set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("J-Editor")
set(JENGINE_EDITOR_SOURCES )

add_compile_definitions(JEDITOR_RES_PATH="${CMAKE_SOURCE_DIR}/J-Editor/res")
add_compile_definitions(GAME_RES_PATH="${CMAKE_SOURCE_DIR}/Game/res")

set(JENGINE_MAIN_SRC
	"src/JEditor/JEditorApp.cpp"
)
source_group("J-Editor" FILES ${JENGINE_MAIN_SRC})
list(APPEND JENGINE_EDITOR_SOURCES ${JENGINE_MAIN_SRC})

set(JENGINE_MAIN_SRC
	"include/JEditor/Core/Selection.h"
	"src/JEditor/Core/Selection.cpp"
)
source_group("J-Editor/Core" FILES ${JENGINE_MAIN_SRC})
list(APPEND JENGINE_EDITOR_SOURCES ${JENGINE_MAIN_SRC})

set(JENGINE_MAIN_SRC
	"include/JEditor/GUI/MainWindow.h"
	"src/JEditor/GUI/MainWindow.cpp"
)
source_group("J-Editor/GUI" FILES ${JENGINE_MAIN_SRC})
list(APPEND JENGINE_EDITOR_SOURCES ${JENGINE_MAIN_SRC})

set(JENGINE_MAIN_SRC
	"include/JEditor/GUI/Panels/InspectorPanel.h"
	"src/JEditor/GUI/Panels/InspectorPanel.cpp"
	
	"include/JEditor/GUI/Panels/HierarchyPanel.h"
	"src/JEditor/GUI/Panels/HierarchyPanel.cpp"
	
	"include/JEditor/GUI/Panels/ProjectPanel.h"
	"src/JEditor/GUI/Panels/ProjectPanel.cpp"
	
	"include/JEditor/GUI/Panels/GameViewPanel.h"
	"src/JEditor/GUI/Panels/GameViewPanel.cpp"
	
	"include/JEditor/GUI/Panels/SceneViewPanel.h"
	"src/JEditor/GUI/Panels/SceneViewPanel.cpp"
	
	"include/JEditor/GUI/Panels/ConsolePanel.h"
	"src/JEditor/GUI/Panels/ConsolePanel.cpp"
)
source_group("J-Editor/GUI/Panels" FILES ${JENGINE_MAIN_SRC})
list(APPEND JENGINE_EDITOR_SOURCES ${JENGINE_MAIN_SRC})

add_executable(J-Editor ${JENGINE_EDITOR_SOURCES})
target_link_libraries(J-Editor J-Engine ImGUI Game)

set_target_properties(J-Editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Editor/ )

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/spdlog/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/YAML/include")


set_target_properties(J-Editor  ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "J-Editor")
set_target_properties(J-Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:J-Editor>"
                                              VS_DEBUGGER_COMMAND           "$<TARGET_FILE:J-Editor>"
                                              VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")