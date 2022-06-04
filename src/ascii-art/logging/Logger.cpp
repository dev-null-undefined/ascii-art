#include "Logger.h"
#include "LogLevel.h"

#include <iostream>
#include <iomanip>
#include <ctime>

#include <filesystem>
namespace fs = std::filesystem;

std::unique_ptr<Logger> Logger::instance = std::unique_ptr<Logger>(nullptr);

#ifndef LOG_LEVEL
#define LOG_LEVEL LogLevel::TRACE
#endif

void Logger::log(const std::string & message, LogLevel logLevel) {
    if (static_cast<int>(logLevel) >= static_cast<int>(LOG_LEVEL)) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");

        getInstance().print( "[" + to_string(logLevel) + "]("+oss.str()+"): " + message);
    }
}


Logger::~Logger() {
    if (m_file && m_file->is_open()) {
        m_file->flush();
        m_file->close();
    }
}

Logger::Logger(const std::optional<std::string> & path) : m_file(std::nullopt) {
    if (path) {
        SetPath(path.value());
    }
}

void Logger::print(const std::string & message) {
    if (m_file) {
        *m_file << message << std::endl;
    } else {
        std::cout << message << std::endl;
    }
}

Logger & Logger::getInstance() {
    if(instance == nullptr) {
        instance = std::unique_ptr<Logger>(new Logger(std::string("log.txt")));
//        instance = std::unique_ptr<Logger>(new Logger());
    }
    return *instance;
}

void Logger::setPath(const std::string & path) {
    getInstance().SetPath(path);
}

void Logger::SetPath(const std::string & path) {
    if (fs::exists(path)) {
        auto status = fs::status(path);
        if (status.type() == fs::file_type::regular) {
            std::ofstream ofstream{path};
            if(ofstream && ofstream.is_open()) {
                m_file = std::move(ofstream);
            }
        }
    } else {
        std::ofstream ofstream{path};
        if (ofstream.is_open()) {
            m_file = std::move(ofstream);
        }
    }
}
