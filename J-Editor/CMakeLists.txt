set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("JE-Editor")
set(JE_EDITOR_SOURCES )

add_platform_stuff()

add_compile_definitions(JE_EDITOR_RES_PATH="${CMAKE_SOURCE_DIR}/J-Editor/res")
add_compile_definitions(JE_RES_PATH="${CMAKE_SOURCE_DIR}/J-Engine/res")
add_compile_definitions(GAME_RES_PATH="${CMAKE_SOURCE_DIR}/Game/res")

add_compile_definitions(JE_EDITOR_DB_PATH="${CMAKE_SOURCE_DIR}/AssetDB/J-Editor/res")
add_compile_definitions(JE_DB_PATH="${CMAKE_SOURCE_DIR}/AssetDB/J-Engine/res")
add_compile_definitions(GAME_DB_PATH="${CMAKE_SOURCE_DIR}/AssetDB/Game/res")

add_compile_definitions(JE_USER_DATA_PATH="${CMAKE_SOURCE_DIR}/Userdata")

add_compile_definitions(JE_EDITOR)

set(JE_MAIN_SRC
	"src/main.cpp"
	"include/JEditor/JEditor.h"
	"src/JEditor/JEditor.cpp"
)
source_group("J-Editor" FILES ${JE_MAIN_SRC})
list(APPEND JE_EDITOR_SOURCES ${JE_MAIN_SRC})

set(JE_MAIN_SRC
	"include/JEditor/Core/Selection.h"
	"src/JEditor/Core/Selection.cpp"
)
source_group("J-Editor/Core" FILES ${JE_MAIN_SRC})
list(APPEND JE_EDITOR_SOURCES ${JE_MAIN_SRC})

set(JE_MAIN_SRC
	"include/JEditor/GUI/MainWindow.h"
	"src/JEditor/GUI/MainWindow.cpp"
)
source_group("J-Editor/GUI" FILES ${JE_MAIN_SRC})
list(APPEND JE_EDITOR_SOURCES ${JE_MAIN_SRC})

set(JE_MAIN_SRC
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
source_group("J-Editor/GUI/Panels" FILES ${JE_MAIN_SRC})
list(APPEND JE_EDITOR_SOURCES ${JE_MAIN_SRC})

add_executable(JE-Editor ${JE_EDITOR_SOURCES})
target_link_libraries(JE-Editor J-Engine-Editor ImGUI Game-Editor)

set_target_properties(JE-Editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Editor/ )

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/spdlog/include")
include_directories("${CMAKE_SOURCE_DIR}/J-Engine/ext/YAML/include")


set_target_properties(JE-Editor ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "J-Editor")
set_target_properties(JE-Editor PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:JE-Editor>"
                                              VS_DEBUGGER_COMMAND           "$<TARGET_FILE:JE-Editor>"
                                              VS_DEBUGGER_ENVIRONMENT       "PATH=%PATH%;${CMAKE_PREFIX_PATH}/bin")