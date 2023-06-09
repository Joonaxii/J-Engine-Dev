#include <JEngine/Core/Log.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace JEngine {
    std::shared_ptr<spdlog::logger> Log::_coreLogger;
    std::shared_ptr<spdlog::logger> Log::_clientLogger;

    void Log::init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/Log.log", 1048576 * 5, 3, true);

        std::vector<spdlog::sink_ptr> sinksCore { std::make_shared<spdlog::sinks::stderr_color_sink_mt>(), fileSink};
        std::vector<spdlog::sink_ptr> sinksClient{ std::make_shared<spdlog::sinks::stderr_color_sink_mt>(), fileSink };

  
        _coreLogger = std::make_shared<spdlog::logger>("J-Engine [Core]", begin(sinksCore), end(sinksCore));
        spdlog::initialize_logger(_coreLogger);
        _coreLogger->set_level(spdlog::level::trace);

        _clientLogger = std::make_shared<spdlog::logger>("J-Engine [Game]", begin(sinksClient), end(sinksClient));
        spdlog::initialize_logger(_clientLogger);
        _clientLogger->set_level(spdlog::level::trace);

        
    }
}