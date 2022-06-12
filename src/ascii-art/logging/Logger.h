#ifndef ASCII_ART_LOGGER_H_FC483115340C444FBBF48CE89298B8AF
#define ASCII_ART_LOGGER_H_FC483115340C444FBBF48CE89298B8AF

#include <string>
#include <optional>
#include <memory>
#include <fstream>
#include "LogLevel.h"
/**
 * @brief Logger for logging into file or std::cout.
 * Singleton class Logger for project wide logging based on the minimum LogLevel set.
 */
class Logger {
  public:
    /**
     * @brief Log method to log message.
     * Static method to log text using singleton Logger instance into set stream.
     * @param message Message to log.
     * @param logLevel LogLevel to use. If lover than the LOG_LEVEL it will not log the message.
     */
    static void log(const std::string & message, LogLevel logLevel = LogLevel::INFO);

    ~Logger();

    Logger(const Logger &) = delete;

    Logger & operator=(const Logger &) = delete;

    /**
     * Get the singleton Logger instance
     * @return Logger instance
     */
    static Logger & getInstance();

    /**
     * @brief set path to log to
     * Set path for the Logger instance used for logging messages.
     * @param path file path to log to.
     */
    static void setPath(const std::string & path);

  private:

    void SetPath(const std::string & path);

    void print(const std::string & message);

    /**
     * Private constructor to prevent from creating multiple instances of Logger.
     * @param path File to log to if non it will default to std::cout.
     */
    explicit Logger(const std::optional<std::string> & path = std::nullopt);

    std::optional<std::ofstream> m_file;

    static std::unique_ptr<Logger> instance;
};


#endif //ASCII_ART_LOGGER_H_FC483115340C444FBBF48CE89298B8AF
