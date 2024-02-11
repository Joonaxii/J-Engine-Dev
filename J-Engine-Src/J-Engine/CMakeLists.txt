set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("J-Engine")
project ("J-Engine-Editor")

add_subdirectory("ext/imgui")
add_subdirectory("ext/YAML")
add_subdirectory("ext/glfw-3.3.8")
add_subdirectory("ext/zlib")
add_subdirectory("ext/spdlog")
add_subdirectory("ext/nlohmann/json")

add_platform_stuff()

set(JE_SOURCES )

# OpenGL Glew files
set(GLEW_SRC
	"ext/include/GL/wglew.h"
	"ext/include/GL/glew.h"
	"ext/src/glew.c"
)
source_group("JEngine/OpenGL/Glew" FILES ${GLEW_SRC})
list(APPEND JE_SOURCES ${GLEW_SRC})

# OpenGL files
set(OPENGL_RENDERING_SRC
	"include/JEngine/Rendering/OpenGL/GLHelpers.h"
	"src/JEngine/Rendering/OpenGL/GLHelpers.cpp"	
)
source_group("JEngine/OpenGL" FILES ${OPENGL_RENDERING_SRC})
list(APPEND JE_SOURCES ${OPENGL_RENDERING_SRC})

# JEngine Debug files
set(JE_DEBUG_SRC
	"include/JEngine/Debug/FPS.h"
	"src/JEngine/Debug/FPS.cpp"
	
	"include/JEngine/Debug/GizmoLayer.h"
	"src/JEngine/Debug/GizmoLayer.cpp"
	"include/JEngine/Debug/Gizmos.h"
	"src/JEngine/Debug/Gizmos.cpp"
)
source_group("JEngine/Debug" FILES ${JE_DEBUG_SRC})
list(APPEND JE_SOURCES ${JE_DEBUG_SRC})

# JEngine Audio files
set(JE_AUDIO_SRC
	"include/JEngine/Audio/AudioEngine.h"
	"src/JEngine/Audio/AudioEngine.cpp"
	
	"include/JEngine/Audio/AudioClip.h"
	"src/JEngine/Audio/AudioClip.cpp"
		
	"include/JEngine/Audio/BeatMap.h"
	"src/JEngine/Audio/BeatMap.cpp"
	
	"src/JEngine/Audio/IAudioSource.cpp"
	"include/JEngine/Audio/IAudioSource.h"
)
source_group("JEngine/Audio" FILES ${JE_AUDIO_SRC})
list(APPEND JE_SOURCES ${JE_AUDIO_SRC})

# JEngine Import Settings files
set(JE_DEBUG_SRC
	"include/JEngine/Assets/Import/ImportSettings.h"
	
	"include/JEngine/Assets/Import/AudioImportSettings.h"
	"src/JEngine/Assets/Import/AudioImportSettings.cpp"
	
	"include/JEngine/Assets/Import/BeatMapImportSettings.h"
	"src/JEngine/Assets/Import/BeatMapImportSettings.cpp"
)
source_group("JEngine/Assets/Import" FILES ${JE_DEBUG_SRC})
list(APPEND JE_SOURCES ${JE_DEBUG_SRC})

# JEngine Utility files
set(JE_UTILITY_SRC
	 "include/JEngine/Utility/HexStr.h"
	 "include/JEngine/Utility/Version.h"
	 "include/JEngine/Utility/SIMD.h"
	 "include/JEngine/Utility/PrintableTypes.h"

     "include/JEngine/Utility/Flags.h"
     "include/JEngine/Utility/EnumUtils.h"
     "src/JEngine/Utility/EnumUtils.cpp"

     "include/JEngine/Utility/Action.h"
     "include/JEngine/Utility/Span.h"
     "include/JEngine/Utility/JTime.h"
     "src/JEngine/Utility/JTime.cpp"
	 
     "include/JEngine/Utility/DataUtilities.h"
     "include/JEngine/Utility/IComparable.h"
     "include/JEngine/Utility/HeapVector.h"
	 
     "include/JEngine/Utility/DataFormatUtils.h"
     "src/JEngine/Utility/DataFormatUtils.cpp"
	 
     "include/JEngine/Utility/StringHelpers.h"
     "src/JEngine/Utility/StringHelpers.cpp"
)
source_group("JEngine/Utility" FILES ${JE_UTILITY_SRC})
list(APPEND JE_SOURCES ${JE_UTILITY_SRC})

# JEngine Math files
set(JE_MATH_SRC
     "include/JEngine/Math/Math.h"
     "src/JEngine/Math/Math.cpp"
)
source_group("JEngine/Math" FILES ${JE_MATH_SRC})
list(APPEND JE_SOURCES ${JE_MATH_SRC})

# JEngine RNG files
set(JE_MATH_RNG_SRC
	 # RNG
     "include/JEngine/Math/Random/IWeighted.h"
     "include/JEngine/Math/Random/WeightedValue.h"
     "include/JEngine/Math/Random/RNG.h"
     "src/JEngine/Math/Random/RNG.cpp"
)
source_group("JEngine/Math/Random" FILES ${JE_MATH_RNG_SRC})
list(APPEND JE_SOURCES ${JE_MATH_RNG_SRC})

# JEngine Math Units
set(JE_MATH_UNITS_SRC
	"include/JEngine/Math/Units/JVector.h"
	
	"include/JEngine/Math/Units/JMatrix.h"
	"src/JEngine/Math/Units/JMatrix.cpp"
	
	"include/JEngine/Math/Units/JRect.h"
)
source_group("JEngine/Math/Units" FILES ${JE_MATH_UNITS_SRC})
list(APPEND JE_SOURCES ${JE_MATH_UNITS_SRC})

# JEngine Math Graphics
set(JE_MATH_GFX_SRC
	"include/JEngine/Math/Graphics/JVertex.h"

	"include/JEngine/Math/Graphics/JColor.h"
	"src/JEngine/Math/Graphics/JColor.cpp"
	
	"include/JEngine/Math/Graphics/JColor32.h"
	"src/JEngine/Math/Graphics/JColor32.cpp"
	
	"include/JEngine/Math/Graphics/JColor24.h"
	"src/JEngine/Math/Graphics/JColor24.cpp"
	
	"include/JEngine/Math/Graphics/JColor555.h"
	"src/JEngine/Math/Graphics/JColor555.cpp"
	
	"include/JEngine/Math/Graphics/JColor565.h"
	"src/JEngine/Math/Graphics/JColor565.cpp"
	
	"include/JEngine/Math/Graphics/JColor4444.h"
	"src/JEngine/Math/Graphics/JColor4444.cpp"
	
	"include/JEngine/Math/Graphics/JColor48.h"
	"src/JEngine/Math/Graphics/JColor48.cpp"
	
	"include/JEngine/Math/Graphics/JColor64.h"
	"src/JEngine/Math/Graphics/JColor64.cpp"
)
source_group("JEngine/Math/Graphics" FILES ${JE_MATH_GFX_SRC})
list(APPEND JE_SOURCES ${JE_MATH_GFX_SRC})

# JEngine Collection files
set(JE_COLLECTIONS_SRC
	 "include/JEngine/Collections/IReferenceProvider.h"
	 "include/JEngine/Collections/IRef.h"
	 "include/JEngine/Collections/ObjectRef.h"
	 "include/JEngine/Collections/StackVector.h"
	 "include/JEngine/Collections/Vector.h"	
	 
	 "include/JEngine/Collections/Bitset.h"
	 "src/JEngine/Collections/Bitset.cpp"
	 
	 "include/JEngine/Collections/PoolVector.h"
	 "include/JEngine/Collections/PoolAllocator.h"
	 "include/JEngine/Collections/SubCollection.h"
	 "include/JEngine/Collections/ReferenceVector.h"
	 "include/JEngine/Collections/IndexStack.h"
)
source_group("JEngine/Collections" FILES ${JE_COLLECTIONS_SRC})
list(APPEND JE_SOURCES ${JE_COLLECTIONS_SRC})

# JEngine GUI files
set(JE_GUI_SRC
	 "include/JEngine/GUI/Gui.h"
	 "src/JEngine/GUI/Gui.cpp"
)
source_group("JEngine/GUI" FILES ${JE_GUI_SRC})
list(APPEND JE_SOURCES ${JE_GUI_SRC})

# JEngine Algorithm files
set(JE_ALGORITHM_SRC
	 "include/JEngine/Algorithm/BinarySearch.h"
)
source_group("JEngine/Algorithm" FILES ${JE_ALGORITHM_SRC})
list(APPEND JE_SOURCES ${JE_ALGORITHM_SRC})

# JEngine Asset files
set(JE_ASSETS_SRC
	 "include/JEngine/Assets/AssetType.h"
	 "include/JEngine/Assets/AssetDataType.h"
	 "src/JEngine/Assets/AssetDataType.cpp"

	 "include/JEngine/Assets/IAsset.h"
     "src/JEngine/Assets/IAsset.cpp"
	 
	 "include/JEngine/Assets/DataAsset.h"
	 "src/JEngine/Assets/DataAsset.cpp"
	 
	 "include/JEngine/Assets/IAssetSerializer.h"
	
	 "include/JEngine/Assets/AssetPacking.h"
     "src/JEngine/Assets/AssetPacking.cpp"
	
	 "include/JEngine/Assets/SceneAsset.h"
     "src/JEngine/Assets/SceneAsset.cpp"
)
source_group("JEngine/Assets" FILES ${JE_ASSETS_SRC})
list(APPEND JE_SOURCES ${JE_ASSETS_SRC})

# JEngine Algorithm files
set(JE_ASSET_SERIALIZER_SRC
	 "include/JEngine/Assets/Serializers/DataAssetSerializer.h"
	 "src/JEngine/Assets/Serializers/DataAssetSerializer.cpp"
	 
	 "include/JEngine/Assets/Serializers/ShaderSerializer.h"
	 "src/JEngine/Assets/Serializers/ShaderSerializer.cpp"
	 
	 "include/JEngine/Assets/Serializers/AudioClipSerializer.h"
	 "src/JEngine/Assets/Serializers/AudioClipSerializer.cpp"
	 
	 "include/JEngine/Assets/Serializers/BeatMapSerializer.h"
	 "src/JEngine/Assets/Serializers/BeatMapSerializer.cpp"
)
source_group("JEngine/Assets/Serializers" FILES ${JE_ASSET_SERIALIZER_SRC})
list(APPEND JE_SOURCES ${JE_ASSET_SERIALIZER_SRC})


# JEngine Scene Assets files
set(JE_SCENE_ASSETS_SRC 
	 "include/JEngine/Assets/Scene/Scene.h"
	 "src/JEngine/Assets/Scene/Scene.cpp"
)
source_group("JEngine/Assets/Scene" FILES ${JE_SCENE_ASSETS_SRC})
# list(APPEND JE_SOURCES ${JE_SCENE_ASSETS_SRC})

# JEngine Texture Assets files
set(JE_TEXTURE_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Texture/Texture2D.h"
     "src/JEngine/Assets/Graphics/Texture/Texture.cpp"
	 "include/JEngine/Assets/Graphics/Texture/Texture.h"
     "src/JEngine/Assets/Graphics/Texture/Texture2D.cpp"
	 "include/JEngine/Assets/Graphics/Texture/RenderTexture.h"
     "src/JEngine/Assets/Graphics/Texture/RenderTexture.cpp"
)
source_group("JEngine/Assets/Graphics/Texture" FILES ${JE_TEXTURE_ASSETS_SRC})
list(APPEND JE_SOURCES ${JE_TEXTURE_ASSETS_SRC})

# JEngine Material Assets files
set(JE_MATERIAL_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Material/Material.h"
     "src/JEngine/Assets/Graphics/Material/Material.cpp"
	 "include/JEngine/Assets/Graphics/Material/MaterialProperty.h"
     "src/JEngine/Assets/Graphics/Material/MaterialProperty.cpp"
)
source_group("JEngine/Assets/Graphics/Material" FILES ${JE_MATERIAL_ASSETS_SRC})
list(APPEND JE_SOURCES ${JE_MATERIAL_ASSETS_SRC})

# JEngine Shader Assets files
set(JE_SHADER_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Shader/Shader.h"
     "src/JEngine/Assets/Graphics/Shader/Shader.cpp"
	 "include/JEngine/Assets/Graphics/Shader/ShaderLib.h"
     "src/JEngine/Assets/Graphics/Shader/ShaderLib.cpp"
)
source_group("JEngine/Assets/Graphics/Shader" FILES ${JE_SHADER_ASSETS_SRC})
list(APPEND JE_SOURCES ${JE_SHADER_ASSETS_SRC})

# JEngine Graphics Assets files
set(JE_GRAPHICS_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Sprite.h"
     "src/JEngine/Assets/Graphics/Sprite.cpp"
)
source_group("JEngine/Assets/Graphics" FILES ${JE_GRAPHICS_ASSETS_SRC})
list(APPEND JE_SOURCES ${JE_GRAPHICS_ASSETS_SRC})

# JEngine Compression files
set(JE_COMPRESSION_SRC
	 "include/JEngine/IO/Compression/ZLib.h"
     "src/JEngine/IO/Compression/ZLib.cpp"
)
source_group("JEngine/IO/Compression" FILES ${JE_COMPRESSION_SRC})
list(APPEND JE_SOURCES ${JE_COMPRESSION_SRC})

# JEngine VFS files
set(JE_VFS_SRC
	 "include/JEngine/IO/VFS/VFS.h"
     "src/JEngine/IO/VFS/VFS.cpp"
	 "include/JEngine/IO/VFS/FileEntry.h"
     "src/JEngine/IO/VFS/FileEntry.cpp"
	 "include/JEngine/IO/VFS/FilePtr.h"
     "src/JEngine/IO/VFS/FilePtr.cpp"
)
source_group("JEngine/IO/VFS" FILES ${JE_VFS_SRC})
list(APPEND JE_SOURCES ${JE_VFS_SRC})

# JEngine IO Helper files
set(JE_IO_HELPERS_SRC
	 "include/JEngine/IO/Helpers/IOUtils.h"
     "src/JEngine/IO/Helpers/IOUtils.cpp"
)
source_group("JEngine/IO/Helpers" FILES ${JE_IO_HELPERS_SRC})
list(APPEND JE_SOURCES ${JE_IO_HELPERS_SRC})

# JEngine IO Helper files
set(JE_IO_SRC
	 "include/JEngine/IO/Base64.h"
     "src/JEngine/IO/Base64.cpp"
	 
	 "include/JEngine/IO/Stream.h"

	 "include/JEngine/IO/FileStream.h"
     "src/JEngine/IO/FileStream.cpp"
	 
	 "include/JEngine/IO/MemoryStream.h"
     "src/JEngine/IO/MemoryStream.cpp"

	 "include/JEngine/IO/BitStream.h"
     "src/JEngine/IO/BitStream.cpp"
	 
	 "include/JEngine/IO/Image.h"
     "src/JEngine/IO/Image.cpp"
	 
	 "include/JEngine/IO/ImageUtils.h"
     "src/JEngine/IO/ImageUtils.cpp"
	 
	 "include/JEngine/IO/Audio.h"
     "src/JEngine/IO/Audio.cpp"
	 
	 "include/JEngine/IO/AudioUtils.h"
     "src/JEngine/IO/AudioUtils.cpp"
)
source_group("JEngine/IO" FILES ${JE_IO_SRC})
list(APPEND JE_SOURCES ${JE_IO_SRC})


# JEngine Serialization files
set(JE_SERIALZIATION_SRC
	 "include/JEngine/IO/Serialization/SerializationConsts.h"
	 "include/JEngine/IO/Serialization/SerializationUtils.h"
	 "src/JEngine/IO/Serialization/SerializationUtils.cpp"
	 "include/JEngine/IO/Serialization/Serializable.h"
	 "include/JEngine/IO/Serialization/Serialize.h"	 
	 "include/JEngine/IO/Serialization/SerializedItem.h"
	 "src/JEngine/IO/Serialization/SerializedItem.cpp"
)
source_group("JEngine/IO/Serialization" FILES ${JE_SERIALZIATION_SRC})
list(APPEND JE_SOURCES ${JE_SERIALZIATION_SRC})

# JEngine ImGui files
set(JE_IMGUI_SRC
     "include/JEngine/Rendering/ImGui/IImGuiDrawable.h"
     "include/JEngine/Rendering/ImGui/ImGuiUtils.h"
)
source_group("JEngine/Rendering/ImGui" FILES ${JE_IMGUI_SRC})
list(APPEND JE_SOURCES ${JE_IMGUI_SRC})

# JEngine Helper files
set(JE_HELPERS_SRC
     "include/JEngine/Helpers/Helpers.h"
     "src/JEngine/Helpers/Helpers.cpp"
     "include/JEngine/Helpers/TypeHelpers.h"
     "src/JEngine/Helpers/TypeHelpers.cpp"
)
source_group("JEngine/Helpers" FILES ${JE_HELPERS_SRC})
list(APPEND JE_SOURCES ${JE_HELPERS_SRC})

# JEngine files
set(JE_MAIN_SRC
	 "include/JEngine.h" 	 
	 "include/JEngine/Core.h"
	 "include/JEngine/Platform.h"
)
source_group("JEngine" FILES ${JE_MAIN_SRC})
list(APPEND JE_SOURCES ${JE_MAIN_SRC})

# JEngine Core files
set(JE_CORE_SRC
	 "include/JEngine/Core/String.h"
	 "src/JEngine/Core/String.cpp"
	 
	 "include/JEngine/Core/LayerMask.h"
	 "src/JEngine/Core/LayerMask.cpp"
	 
	 "include/JEngine/Core/Application.h"
	 "src/JEngine/Core/Application.cpp"
	 "include/JEngine/Core/EntryPoint.h"
	 
	 "include/JEngine/Core/Log.h"
	 "include/JEngine/Core/Assert.h"
	 
	 "include/JEngine/Core/IObject.h"
	
	 "include/JEngine/Core/Memory.h"
     "src/JEngine/Core/Memory.cpp"
	 
	 "include/JEngine/Core/GameObject.h"
     "src/JEngine/Core/GameObject.cpp"
	 
	 "include/JEngine/Core/Scene.h"
     "src/JEngine/Core/Scene.cpp"
	 
	 "include/JEngine/Core/AssetDB.h"
     "src/JEngine/Core/AssetDB.cpp"
	 
	 "include/JEngine/Core/SceneManager.h"
     "src/JEngine/Core/SceneManager.cpp"

	 "include/JEngine/Core/Ref.h"
	 "src/JEngine/Core/Ref.cpp"

	 "include/JEngine/Core/UndoRedo.h"
	 "src/JEngine/Core/UndoRedo.cpp"

	 "src/JEngine/Core/Log.cpp"
)
source_group("JEngine/Core" FILES ${JE_CORE_SRC})
list(APPEND JE_SOURCES ${JE_CORE_SRC})

# JEngine Component files
set(JE_COMP_SRC
	 "src/JEngine/Components/Component.cpp" 	 
	 "include/JEngine/Components/Component.h"
	 
	 "src/JEngine/Components/ComponentFactory.cpp" 	 
	 "include/JEngine/Components/ComponentFactory.h"
	 
	 "src/JEngine/Components/CTransform.cpp" 	 
	 "include/JEngine/Components/CTransform.h"
	 
)
source_group("JEngine/Components" FILES ${JE_COMP_SRC})
list(APPEND JE_SOURCES ${JE_COMP_SRC})


# JEngine Rendering files
set(JE_RENDERING_CORE_SRC
     "include/JEngine/Rendering/RenderingConsts.h"
     "include/JEngine/Rendering/Window.h"
     "src/JEngine/Rendering/Window.cpp"

	 "include/JEngine/Rendering/Renderer.h"
	 "src/JEngine/Rendering/Renderer.cpp"
	 
	 "include/JEngine/Rendering/IRenderable.h"
	 "include/JEngine/Rendering/IRenderer.h"
	
	 "include/JEngine/Rendering/SortingLayer.h"
	 "src/JEngine/Rendering/SortingLayer.cpp"
	 
	 "include/JEngine/Rendering/DynamicBatch.h"
	 
	 "include/JEngine/Rendering/ICamera.h"
	 "src/JEngine/Rendering/ICamera.cpp"
	 
	 "include/JEngine/Rendering/RenderTarget.h"
	 "src/JEngine/Rendering/RenderTarget.cpp"
	 
	 "include/JEngine/Rendering/FrameBuffer.h"
	 "src/JEngine/Rendering/FrameBuffer.cpp"
)
source_group("JEngine/Rendering" FILES ${JE_RENDERING_CORE_SRC})
list(APPEND JE_SOURCES ${JE_RENDERING_CORE_SRC})

# JEngine Buffers files
set(JE_RENDERING_BUFFERS_SRC
	 # Buffers
	 "include/JEngine/Rendering/Buffers/VertexBuffer.h"
	 "src/JEngine/Rendering/Buffers/VertexBuffer.cpp"
	 "include/JEngine/Rendering/Buffers/IndexBuffer.h"
	 "src/JEngine/Rendering/Buffers/IndexBuffer.cpp"
	 
	 "include/JEngine/Rendering/Buffers/BufferLayout.h"
	 "include/JEngine/Rendering/Buffers/VertexArray.h"
	 "src/JEngine/Rendering/Buffers/VertexArray.cpp"
)
source_group("JEngine/Rendering/Buffers" FILES ${JE_RENDERING_BUFFERS_SRC})
list(APPEND JE_SOURCES ${JE_RENDERING_BUFFERS_SRC})

# JEngine Input files
set(JE_INPUT_SRC
	 "include/JEngine/Input/Input.h"
	 "src/JEngine/Input/Input.cpp"
)
source_group("JEngine/Input" FILES ${JE_INPUT_SRC})
list(APPEND JE_SOURCES ${JE_INPUT_SRC})


set(JEDITOR_SOURCES )
list(APPEND JEDITOR_SOURCES "${JE_SOURCES}")


# JEngine Editor IO files
set(JE_IO_SRC
	 "include/Editor/IO/DirectoryMonitor.h"
	 "src/Editor/IO/DirectoryMonitor.cpp"
)
source_group("Editor/IO" FILES ${JE_IO_SRC})
list(APPEND JEDITOR_SOURCES ${JE_IO_SRC})


# JEngine Editor Core files
set(JE_IO_SRC
	 "include/Editor/Core/EditorEssentials.h"
	 "src/Editor/Core/EditorEssentials.cpp"
)
source_group("Editor/Core" FILES ${JE_IO_SRC})
list(APPEND JEDITOR_SOURCES ${JE_IO_SRC})

add_lib_for(J-Engine-Editor "${JEDITOR_SOURCES}" JE_EDITOR)
add_lib_for(J-Engine-Player "${JE_SOURCES}" JE_PLAYER)

target_compile_options(
    J-Engine-Editor PUBLIC
    /external:W0
)

target_compile_options(
    J-Engine-Player PUBLIC
    /external:W0
)

find_package(OpenGL REQUIRED)
target_link_libraries(J-Engine-Player spdlog XInput OpenGL::GL glfw ImGUI zlibstatic yaml-cpp nlohmann_json XAudio2)
target_link_libraries(J-Engine-Editor spdlog XInput OpenGL::GL glfw ImGUI zlibstatic yaml-cpp nlohmann_json XAudio2)

include_directories("include")
include_directories("ext/include")
include_directories("ext/imgui")
include_directories("ext/YAML/include")
include_directories("include/JEngine")
include_directories("ext/nlohmann/json/include")
include_directories("ext/zlib")
include_directories("ext/spdlog/include")
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/ext/zlib")