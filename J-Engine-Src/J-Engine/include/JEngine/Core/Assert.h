#pragma once
#include <JEngine/Core/Log.h>
#include <filesystem>

#ifdef JE_DEBUG
#ifdef JE_WINDOWS
#define JE_DEBUGBREAK() __debugbreak()
#else
    #define JE_DEBUGBREAK()
#endif
#define JE_ENABLE_ASSERTS
#else
#define JE_DEBUGBREAK()
#endif

#ifdef JE_ENABLE_ASSERTS
    
#define JE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { JE##type##ERROR(msg, __VA_ARGS__); JE_DEBUGBREAK(); } }
#define JE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) JE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define JE_INTERNAL_ASSERT_NO_MSG(type, check, ...) JE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", JE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define JE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define JE_INTERNAL_ASSERT_GET_MACRO(...) JE_EXPAND_MACRO(JE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, JE_INTERNAL_ASSERT_WITH_MSG, JE_INTERNAL_ASSERT_NO_MSG))

#define JE_ASSERT(...) JE_EXPAND_MACRO(JE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CLIENT_, __VA_ARGS__))
#define JE_CORE_ASSERT(...) JE_EXPAND_MACRO(JE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else
#define JE_ASSERT(...)
#define JE_CORE_ASSERT(...) 
#endif

#define JE_CORE_RET_IF_FALSE(INPUT, RET_VAL) if(!(INPUT)) { JE_CORE_ERROR_DBG("Parameter \"{0}\" evaluated to false!", STR(PTR)); return RET_VAL; }
#define JE_CORE_RET_IF_FALSE_MSG(INPUT, MSG, RET_VAL) if(!(INPUT)) { JE_CORE_ERROR_DBG(MSG); return RET_VAL; }
#define JE_CORE_RET_COND_MSG(INPUT, MSG, RET_VAL_FAIL) if(!(INPUT)) { JE_CORE_ERROR_DBG(MSG); return RET_VAL_FAIL; } else { return INPUT; }
