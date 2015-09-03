#include "Log.h"

namespace Engine
{
    static Logger gLogger;

    void Log::setLogger(Logger&& logger)
    {
        gLogger = std::move(logger);
    }

    void Log::write(LogLevel level, const std::ostream& stream)
    {
        if (gLogger)
            gLogger(level, static_cast<const std::ostringstream&>(stream).str());
    }
}
