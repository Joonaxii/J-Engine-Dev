#pragma once

#define JENGINE_EXPAND_MACRO(x) x
#define JENGINE_STRINGIFY_MACRO(x) #x

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace JEngine {
    class Log {
    public:
        static void init();
        
        static std::shared_ptr<spdlog::logger>& getCoreLogger()   { return _coreLogger;   };
        static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; };

    private:
        static std::shared_ptr<spdlog::logger> _coreLogger;
        static std::shared_ptr<spdlog::logger> _clientLogger;
    };
}

#ifdef JENGINE_DEBUG
//Core Log Macros
#define JENGINE_CORE_FATAL(...) ::JEngine::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define JENGINE_CORE_ERROR(...) ::JEngine::Log::getCoreLogger()->error(__VA_ARGS__)
#define JENGINE_CORE_WARN(...)  ::JEngine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define JENGINE_CORE_INFO(...)  ::JEngine::Log::getCoreLogger()->info(__VA_ARGS__)
#define JENGINE_CORE_TRACE(...) ::JEngine::Log::getCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define JENGINE_FATAL(...) ::JEngine::Log::getClientLogger()->fatal(__VA_ARGS__)
#define JENGINE_ERROR(...) ::JEngine::Log::getClientLogger()->error(__VA_ARGS__)
#define JENGINE_WARN(...) ::JEngine::Log::getClientLogger()->warn(__VA_ARGS__)
#define JENGINE_INFO(...) ::JEngine::Log::getClientLogger()->info(__VA_ARGS__)
#define JENGINE_TRACE(...) ::JEngine::Log::getClientLogger()->trace(__VA_ARGS__)

#else
//Possibly do different logging when in Release mode?

//Core Log Macros
#define JENGINE_CORE_FATAL(...) ::JEngine::Log::getFileLogger()->fatal(__VA_ARGS__)
#define JENGINE_CORE_ERROR(...) ::JEngine::Log::getFileLogger()->error(__VA_ARGS__)
#define JENGINE_CORE_WARN(...)  ::JEngine::Log::getFileLogger()->warn(__VA_ARGS__)
#define JENGINE_CORE_INFO(...)  ::JEngine::Log::getFileLogger()->info(__VA_ARGS__)
#define JENGINE_CORE_TRACE(...) ::JEngine::Log::getFileLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define JENGINE_FATAL(...) ::JEngine::Log::getFileLogger()->fatal(__VA_ARGS__)
#define JENGINE_ERROR(...) ::JEngine::Log::getFileLogger()->error(__VA_ARGS__)
#define JENGINE_WARN(...) ::JEngine::Log::getFileLogger()->warn(__VA_ARGS__)
#define JENGINE_INFO(...) ::JEngine::Log::getFileLogger()->info(__VA_ARGS__)
#define JENGINE_TRACE(...) ::JEngine::Log::getFileLogger()->trace(__VA_ARGS__)
#endif