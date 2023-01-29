cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("J-Engine")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("ext/imgui")
add_subdirectory("ext/glfw-3.3.8")
add_subdirectory("ext/zlib-1.2.12")
add_subdirectory("ext/json")

add_compile_definitions(GLEW_STATIC)

set(JENGINE_SOURCES )

# OpenGL Glew files
set(GLEW_SRC
	"ext/include/GL/wglew.h"
	"ext/include/GL/glew.h"
	"ext/src/glew.c"
)
source_group("JEngine/OpenGL/Glew" FILES ${GLEW_SRC})
list(APPEND JENGINE_SOURCES ${GLEW_SRC})

# OpenGL files
set(OPENGL_RENDERING_SRC
	"include/JEngine/Rendering/OpenGL/GLHelpers.h"
	"src/JEngine/Rendering/OpenGL/GLHelpers.cpp"
)
source_group("JEngine/Rendering/OpenGL" FILES ${OPENGL_RENDERING_SRC})
list(APPEND JENGINE_SOURCES ${OPENGL_RENDERING_SRC})

# OpenGL files
set(JENGINE_DEBUG_SRC
	"include/JEngine/Debugging/FPS.h"
	"src/JEngine/Debugging/FPS.cpp"
)
source_group("JEngine/Debug" FILES ${JENGINE_DEBUG_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_DEBUG_SRC})

# JEngine Utility files
set(JENGINE_UTILITY_SRC
	 "include/JEngine/Utility/Version.h"

     "include/JEngine/Utility/Flags.h"

     "include/JEngine/Utility/Action.h"
     "include/JEngine/Utility/Span.h"
     "include/JEngine/Utility/ConstSpan.h"
     "include/JEngine/Utility/JTime.h"
     "src/JEngine/Utility/JTime.cpp"
)
source_group("JEngine/Utility" FILES ${JENGINE_UTILITY_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_UTILITY_SRC})

# JEngine Math files
set(JENGINE_MATH_SRC
     "include/JEngine/Math/Math.h"
     "src/JEngine/Math/Math.cpp"
)
source_group("JEngine/Math" FILES ${JENGINE_MATH_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_MATH_SRC})

# JEngine RNG files
set(JENGINE_MATH_RNG_SRC
	 # RNG
     "include/JEngine/Math/Random/IWeighted.h"
     "include/JEngine/Math/Random/WeightedValue.h"
     "include/JEngine/Math/Random/RNG.h"
     "src/JEngine/Math/Random/RNG.cpp"
)
source_group("JEngine/Math/Random" FILES ${JENGINE_MATH_RNG_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_MATH_RNG_SRC})

# JEngine Math Units
set(JENGINE_MATH_UNITS_SRC
	"include/JEngine/Math/Units/JVector.h"
	
	"include/JEngine/Math/Units/JMatrix.h"
	"src/JEngine/Math/Units/JMatrix.cpp"
	
	"include/JEngine/Math/Units/JRect.h"
)
source_group("JEngine/Math/Units" FILES ${JENGINE_MATH_UNITS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_MATH_UNITS_SRC})

# JEngine Math Graphics
set(JENGINE_MATH_GFX_SRC
	"include/JEngine/Math/Graphics/JVertex.h"

	"include/JEngine/Math/Graphics/JColor.h"
	"src/JEngine/Math/Graphics/JColor.cpp"
	
	"include/JEngine/Math/Graphics/JColor32.h"
	"src/JEngine/Math/Graphics/JColor32.cpp"
	
	"include/JEngine/Math/Graphics/JColor24.h"
	"src/JEngine/Math/Graphics/JColor24.cpp"
)
source_group("JEngine/Math/Graphics" FILES ${JENGINE_MATH_GFX_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_MATH_GFX_SRC})

# JEngine Collection files
set(JENGINE_COLLECTIONS_SRC
	 "include/JEngine/Collections/IReferenceProvider.h"
	 "include/JEngine/Collections/IRef.h"
	 "include/JEngine/Collections/ObjectRef.h"
	 "include/JEngine/Collections/ReferenceVector.h"
)
source_group("JEngine/Collections" FILES ${JENGINE_COLLECTIONS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_COLLECTIONS_SRC})

# JEngine Algorithm files
set(JENGINE_ALGORITHM_SRC
	 "include/JEngine/Algorithm/BinarySearch.h"
)
source_group("JEngine/Algorithm" FILES ${JENGINE_ALGORITHM_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_ALGORITHM_SRC})

# JEngine Asset files
set(JENGINE_ASSETS_SRC
	 "include/JEngine/Assets/AssetMetaData.h"
	 "src/JEngine/Assets/AssetMetaData.cpp"
	 "include/JEngine/Assets/AssetType.h"
	 "include/JEngine/Assets/AssetDataType.h"

	 "include/JEngine/Assets/IAsset.h"
     "src/JEngine/Assets/IAsset.cpp"
	 
	 "include/JEngine/Assets/AssetPaths.h"
	 
	 "include/JEngine/Assets/AssetPacking.h"
     "src/JEngine/Assets/AssetPacking.cpp"
)
source_group("JEngine/Assets" FILES ${JENGINE_ASSETS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_ASSETS_SRC})

# JEngine Scene Assets files
set(JENGINE_SCENE_ASSETS_SRC 
	 "include/JEngine/Assets/Scene/Scene.h"
	 "src/JEngine/Assets/Scene/Scene.cpp"
)
source_group("JEngine/Assets/Scene" FILES ${JENGINE_SCENE_ASSETS_SRC})
# list(APPEND JENGINE_SOURCES ${JENGINE_SCENE_ASSETS_SRC})

# JEngine Texture Assets files
set(JENGINE_TEXTURE_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Texture/FilterMode.h"
	 "include/JEngine/Assets/Graphics/Texture/TextureFormat.h"
	 "include/JEngine/Assets/Graphics/Texture/Texture2D.h"
     "src/JEngine/Assets/Graphics/Texture/Texture.cpp"
	 "include/JEngine/Assets/Graphics/Texture/Texture.h"
     "src/JEngine/Assets/Graphics/Texture/Texture2D.cpp"
	 "include/JEngine/Assets/Graphics/Texture/RenderTexture.h"
     "src/JEngine/Assets/Graphics/Texture/RenderTexture.cpp"
)
source_group("JEngine/Assets/Graphics/Texture" FILES ${JENGINE_TEXTURE_ASSETS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_TEXTURE_ASSETS_SRC})

# JEngine Material Assets files
set(JENGINE_MATERIAL_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Material/Material.h"
     "src/JEngine/Assets/Graphics/Material/Material.cpp"
	 "include/JEngine/Assets/Graphics/Material/MaterialProperty.h"
     "src/JEngine/Assets/Graphics/Material/MaterialProperty.cpp"
)
source_group("JEngine/Assets/Graphics/Material" FILES ${JENGINE_MATERIAL_ASSETS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_MATERIAL_ASSETS_SRC})

# JEngine Shader Assets files
set(JENGINE_SHADER_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Shader/Shader.h"
     "src/JEngine/Assets/Graphics/Shader/Shader.cpp"
)
source_group("JEngine/Assets/Graphics/Shader" FILES ${JENGINE_SHADER_ASSETS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_SHADER_ASSETS_SRC})

# JEngine Material Assets files
set(JENGINE_GRAPHICS_ASSETS_SRC 
	 "include/JEngine/Assets/Graphics/Sprite.h"
     "src/JEngine/Assets/Graphics/Sprite.cpp"
)
source_group("JEngine/Assets/Graphics" FILES ${JENGINE_GRAPHICS_ASSETS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_GRAPHICS_ASSETS_SRC})

# JEngine Decoder files
set(JENGINE_GFX_ASSETS_SRC  
	 "include/JEngine/Assets/Decoders/DecoderResult.h"

	 "include/JEngine/Assets/Decoders/BMPDecoder.h"
     "src/JEngine/Assets/Decoders/BMPDecoder.cpp"

	 "include/JEngine/Assets/Decoders/PNGDecoder.h"
     "src/JEngine/Assets/Decoders/PNGDecoder.cpp"
)
source_group("JEngine/Assets/Decoders" FILES ${JENGINE_GFX_ASSETS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_GFX_ASSETS_SRC})

# JEngine Compression files
set(JENGINE_COMPRESSION_SRC
	 "include/JEngine/IO/Compression/ZLibStream.h"
     "src/JEngine/IO/Compression/ZLibStream.cpp"
)
source_group("JEngine/IO/Compression" FILES ${JENGINE_COMPRESSION_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_COMPRESSION_SRC})

# JEngine VFS files
set(JENGINE_VFS_SRC
	 "include/JEngine/IO/VFS/VFS.h"
     "src/JEngine/IO/VFS/VFS.cpp"
	 "include/JEngine/IO/VFS/FileEntry.h"
     "src/JEngine/IO/VFS/FileEntry.cpp"
	 "include/JEngine/IO/VFS/FilePtr.h"
     "src/JEngine/IO/VFS/FilePtr.cpp"
)
source_group("JEngine/IO/VFS" FILES ${JENGINE_VFS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_VFS_SRC})

# JEngine IO Helper files
set(JENGINE_IO_HELPERS_SRC
	 "include/JEngine/IO/Helpers/FileExtensions.h"
     "src/JEngine/IO/Helpers/FileExtensions.cpp"
)
source_group("JEngine/IO/Helpers" FILES ${JENGINE_IO_HELPERS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_IO_HELPERS_SRC})

# JEngine Serialization files
set(JENGINE_SERIALZIATION_SRC
	 "include/JEngine/IO/Serialization/ISerializable.h"
	 "include/JEngine/IO/Serialization/Serialization.h"
     "src/JEngine/IO/Serialization/Serialization.cpp"
)
source_group("JEngine/IO/Serialization" FILES ${JENGINE_SERIALZIATION_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_SERIALZIATION_SRC})

# JEngine Cryptography files
set(JENGINE_CRYPTOGRAPHY_SRC
     "include/JEngine/Cryptography/CRC32.h"
     "include/JEngine/Cryptography/FAH16.h"
     "src/JEngine/Cryptography/FAH16.cpp"
)
source_group("JEngine/Cryptography" FILES ${JENGINE_CRYPTOGRAPHY_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_CRYPTOGRAPHY_SRC})

# JEngine Object files
set(JENGINE_GLOBAL_SRC
     "include/JEngine/Global/UUIDFactory.h"
)
source_group("JEngine/Global" FILES ${JENGINE_GLOBAL_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_GLOBAL_SRC})

# JEngine ImGui files
set(JENGINE_IMGUI_SRC
     "include/JEngine/Rendering/ImGui/IImGuiDrawable.h"
)
source_group("JEngine/Rendering/ImGui" FILES ${JENGINE_IMGUI_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_IMGUI_SRC})

# JEngine Object files
set(JENGINE_HELPERS_SRC
     "include/JEngine/Helpers/EnumMacros.h"
     "include/JEngine/Helpers/Helpers.h"
     "src/JEngine/Helpers/Helpers.cpp"
     "include/JEngine/Helpers/StringConsts.h"
     "include/JEngine/Helpers/StringExtensions.h"
     "src/JEngine/Helpers/StringExtensions.cpp"
     "include/JEngine/Helpers/TypeHelpers.h"
     "src/JEngine/Helpers/TypeHelpers.cpp"
     "include/JEngine/Helpers/NoCopy.h"
)
source_group("JEngine/Helpers" FILES ${JENGINE_HELPERS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_HELPERS_SRC})

# JEngine Core files
set(JENGINE_CORE_SRC
	 "include/JEngine/Core/LayerMask.h"
	 "src/JEngine/Core/LayerMask.cpp"
)
source_group("JEngine/Core" FILES ${JENGINE_CORE_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_CORE_SRC})

# JEngine Rendering files
set(JENGINE_RENDERING_CORE_SRC
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
source_group("JEngine/Rendering" FILES ${JENGINE_RENDERING_CORE_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_RENDERING_CORE_SRC})

# JEngine Buffers files
set(JENGINE_RENDERING_BUFFERS_SRC
	 # Buffers
	 "include/JEngine/Rendering/Buffers/VertexBuffer.h"
	 "src/JEngine/Rendering/Buffers/VertexBuffer.cpp"
	 "include/JEngine/Rendering/Buffers/IndexBuffer.h"
	 "src/JEngine/Rendering/Buffers/IndexBuffer.cpp"
	 
	 "include/JEngine/Rendering/Buffers/BufferLayout.h"
	 "include/JEngine/Rendering/Buffers/VertexArray.h"
	 "src/JEngine/Rendering/Buffers/VertexArray.cpp"
)
source_group("JEngine/Rendering/Buffers" FILES ${JENGINE_RENDERING_BUFFERS_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_RENDERING_BUFFERS_SRC})

# JEngine Input files
set(JENGINE_INPUT_SRC
	 "include/JEngine/Input/Input.h"
	 "src/JEngine/Input/Input.cpp"
)
source_group("JEngine/Input" FILES ${JENGINE_INPUT_SRC})
list(APPEND JENGINE_SOURCES ${JENGINE_INPUT_SRC})

add_library(J-Engine STATIC ${JENGINE_SOURCES})

find_package(OpenGL REQUIRED)
target_link_libraries(J-Engine XInput OpenGL::GL glfw ImGUI zlibstatic nlohmann_json)

include_directories("include")
include_directories("ext/include")
include_directories("ext/imgui")
include_directories("include/JEngine")
include_directories("ext/json/include")
include_directories("ext/zlib-1.2.12")
include_directories(BEFORE "${CMAKE_CURRENT_BINARY_DIR}/ext/zlib-1.2.12")