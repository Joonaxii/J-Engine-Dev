#pragma once
#include <JEngine/Platform.h>

#define JE_EXPAND_MACRO(x) x
#define JE_STRINGIFY_MACRO(x) #x

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/chrono.h>
#pragma warning(pop)

namespace JEngine {
    class Log {
    public:
        static void init();
        
        static std::shared_ptr<spdlog::logger> getCoreLogger() { return _coreLogger; }
        static std::shared_ptr<spdlog::logger> getClientLogger() { return _clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> _coreLogger;
        static std::shared_ptr<spdlog::logger> _clientLogger;
    };
}
//Core Log Macros

#define JE_CORE_FATAL(...) ::JEngine::Log::getCoreLogger()->critical(__VA_ARGS__) 
#define JE_CORE_ERROR(...) ::JEngine::Log::getCoreLogger()->error(__VA_ARGS__) 
#define JE_CORE_WARN(...)  ::JEngine::Log::getCoreLogger()->warn(__VA_ARGS__) 
#define JE_CORE_INFO(...)  ::JEngine::Log::getCoreLogger()->info(__VA_ARGS__) 
#define JE_CORE_TRACE(...) ::JEngine::Log::getCoreLogger()->trace(__VA_ARGS__) 

//Client Log Macros

#define JE_FATAL(...) ::JEngine::Log::getClientLogger()->critical(__VA_ARGS__) 
#define JE_ERROR(...) ::JEngine::Log::getClientLogger()->error(__VA_ARGS__) 
#define JE_WARN(...)  ::JEngine::Log::getClientLogger()->warn(__VA_ARGS__) 
#define JE_INFO(...)  ::JEngine::Log::getClientLogger()->info(__VA_ARGS__) 
#define JE_TRACE(...) ::JEngine::Log::getClientLogger()->trace(__VA_ARGS__) 

#ifdef JE_DEBUG
//Core Debug Log Macros

#define JE_CORE_FATAL_DBG(...) JE_CORE_FATAL(__VA_ARGS__) 
#define JE_CORE_ERROR_DBG(...) JE_CORE_ERROR(__VA_ARGS__) 
#define JE_CORE_WARN_DBG(...)  JE_CORE_WARN(__VA_ARGS__) 
#define JE_CORE_INFO_DBG(...)  JE_CORE_INFO(__VA_ARGS__)  
#define JE_CORE_TRACE_DBG(...) JE_CORE_TRACE(__VA_ARGS__) 

//Client Debug Log Macros

#define JE_FATAL_DBG(...) JE_FATAL(__VA_ARGS__) 
#define JE_ERROR_DBG(...) JE_ERROR(__VA_ARGS__) 
#define JE_WARN_DBG(...)  JE_WARN(__VA_ARGS__) 
#define JE_INFO_DBG(...)  JE_INFO(__VA_ARGS__) 
#define JE_TRACE_DBG(...) JE_TRACE(__VA_ARGS__) 

#else
//Core Debug Log Macros

#define JE_CORE_FATAL_DBG(...)  
#define JE_CORE_ERROR_DBG(...)  
#define JE_CORE_WARN_DBG(...)   
#define JE_CORE_INFO_DBG(...)   
#define JE_CORE_TRACE_DBG(...)  

//Client Debug Log Macros

#define JE_FATAL_DBG(...) 
#define JE_ERROR_DBG(...) 
#define JE_WARN_DBG(...)  
#define JE_INFO_DBG(...)  
#define JE_TRACE_DBG(...) 

#endif