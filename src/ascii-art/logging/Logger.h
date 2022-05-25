#ifndef ASCII_ART_LOGGER_H_FC483115340C444FBBF48CE89298B8AF
#define ASCII_ART_LOGGER_H_FC483115340C444FBBF48CE89298B8AF

#include <string>
#include <optional>
#include <memory>
#include <fstream>
#include "LogLevel.h"

class Logger {
  public:
    static void log(const std::string & message, LogLevel logLevel = LogLevel::INFO);

    ~Logger();

    Logger(const Logger &) = delete;

    Logger & operator=(const Logger &) = delete;

    static Logger & getInstance();

    static void setPath(const std::string & path);

  private:

    void SetPath(const std::string & path);

    void print(const std::string & message);

    explicit Logger(const std::optional<std::string> & path = std::nullopt);

    std::optional<std::ofstream> m_file;

    static std::unique_ptr<Logger> instance;
};


#endif //ASCII_ART_LOGGER_H_FC483115340C444FBBF48CE89298B8AF
