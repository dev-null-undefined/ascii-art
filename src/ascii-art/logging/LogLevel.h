#ifndef ASCII_ART_LOGLEVEL_H_D4B07D87413A4CE99D71099FD03E2FBF
#define ASCII_ART_LOGLEVEL_H_D4B07D87413A4CE99D71099FD03E2FBF

#include <string>

/**
 * Log levels in increasing order
 * Used for setting the level of verbosity of the Logger.
 */
enum class LogLevel {
    TRACE = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    FATAL = 4
};

std::string to_string(LogLevel level);

#endif //ASCII_ART_LOGLEVEL_H_D4B07D87413A4CE99D71099FD03E2FBF
