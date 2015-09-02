
#pragma once
#include <string>
#include <functional>
#include <memory>
#include <sstream>

namespace Engine
{
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
    };

    using Logger = std::function<void(LogLevel, const std::string&)>;

    namespace Log
    {
        void setLogger(Logger&& logger);
        void write(LogLevel level, const std::ostream& stream);
    }
}

#define Z_TRACE(message) (::Engine::Log::write(::Engine::LogLevel::Trace, std::ostringstream() << message))
#define Z_LOGD(message) (::Engine::Log::write(::Engine::LogLevel::Debug, std::ostringstream() << message))
#define Z_LOGI(message) (::Engine::Log::write(::Engine::LogLevel::Info, std::ostringstream() << message))
#define Z_LOGW(message) (::Engine::Log::write(::Engine::LogLevel::Warning, std::ostringstream() << message))
#define Z_LOGE(message) (::Engine::Log::write(::Engine::LogLevel::Error, std::ostringstream() << message))
