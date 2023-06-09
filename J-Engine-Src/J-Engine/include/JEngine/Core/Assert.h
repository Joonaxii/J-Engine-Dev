#pragma once
#include <JEngine/Core/Log.h>
#include <filesystem>

#ifdef JENGINE_DEBUG
#ifdef JENGINE_PLATFORM_WINDOWS
#define JENGINE_DEBUGBREAK() __debugbreak()
#else
    #define JENGINE_DEBUGBREAK()
#endif
#define JENGINE_ENABLE_ASSERTS
#else
#define JENGINE_DEBUGBREAK()
#endif

#ifdef JENGINE_ENABLE_ASSERTS
    
#define JENGINE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { JENGINE##type##ERROR(msg, __VA_ARGS__); JENGINE_DEBUGBREAK(); } }
#define JENGINE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) JENGINE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define JENGINE_INTERNAL_ASSERT_NO_MSG(type, check, ...) JENGINE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", JENGINE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define JENGINE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define JENGINE_INTERNAL_ASSERT_GET_MACRO(...) JENGINE_EXPAND_MACRO(JENGINE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, JENGINE_INTERNAL_ASSERT_WITH_MSG, JENGINE_INTERNAL_ASSERT_NO_MSG))

#define JENGINE_ASSERT(...) JENGINE_EXPAND_MACRO(JENGINE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CLIENT_, __VA_ARGS__))
#define JENGINE_CORE_ASSERT(...) JENGINE_EXPAND_MACRO(JENGINE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else
#define JENGINE_ASSERT(...)
#define JENGINE_CORE_ASSERT(...) 
#endif
