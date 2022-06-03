#include <string>
#include "Settings.h"
#include "logging/Logger.h"
#include "../FileManager.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

double Settings::maxValue() const {
    return colorValue(Color{255, 255, 255, 255});
}

double Settings::colorValue(const Color & color) const {
    double value = 0;
    Color modified = m_brightness_filter.apply(m_color_filter.apply(color,{}), {}).normalize();
    value += modified.getRed();
    value += modified.getGreen();
    value += modified.getBlue();
    value += modified.getAlpha();
    return value < 0 ? 0 : value;
}

Color Settings::modifiedColor(const Color & color) const {
    return m_color_filter.apply(color, {});
}

#define array_size(array) (sizeof(array) / sizeof(array[0]))

Settings::Settings() {
    std::string chars = "    .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    m_chars = std::vector<char>(chars.begin(), chars.end());
    for (int i = array_size(CONFIG_FILE_PATHS) - 1; i >= 0; --i) {
        parseConfigFile(FileManager::normalize_path(CONFIG_FILE_PATHS[i]));
    }
}

char Settings::getChar(const Color & color) const {
    double value = colorValue(color.normalize());
    double max = maxValue();
    auto index = (size_t) (value * (double) (m_chars.size() - 1) / max);
#ifdef DEBUG
    if (index >= m_chars.size()) {
        Logger::log("Index out of range: " + std::to_string(index) + " " + std::to_string(m_chars.size()),
                    LogLevel::ERROR);
        index = m_chars.size() - 1;
    }
#endif
    return m_chars.at(index);
}

Color Settings::getRoundedColor(Color color) const {
    double value = colorValue(color.normalize());
    double max = maxValue();
    auto index = (size_t) (value * (double) (m_chars.size() - 1) / max);
#ifdef DEBUG
    if (index >= m_chars.size()) {
        Logger::log("Index out of range: " + std::to_string(index) + " " + std::to_string(m_chars.size()),
                    LogLevel::ERROR);
        index = m_chars.size() - 1;
    }
#endif
    int remainder = static_cast<int>(value - static_cast<double>(index) * max / (double) (m_chars.size() - 1));

    Color roundedColor{color.getRed() - remainder / 3, color.getGreen() - remainder / 3,
                       color.getBlue() - (remainder / 3 + remainder % 3), color.getAlpha()};
    return roundedColor;

}


bool equals(const std::string & a, const std::string & b) {
    size_t a_index = 0;
    size_t b_index = 0;
    while (a_index < a.size() && b_index < b.size()) {
        if (std::isspace(a[a_index])) {
            ++a_index;
            continue;
        }
        if (std::isspace(b[b_index])) {
            ++b_index;
            continue;
        }
        if (toupper(a[a_index]) != toupper(b[b_index])) {
            return false;
        }
        ++a_index;
        ++b_index;
    }

    while (a_index < a.size()) {
        if (!std::isspace(a[a_index])) {
            return false;
        }
        ++a_index;
    }

    while (b_index < b.size()) {
        if (!std::isspace(b[b_index])) {
            return false;
        }
        ++b_index;
    }
    return a_index == a.size() && b_index == b.size();
}

const std::map<std::string, ConfigKey> Settings::CONFIG_KEYS = {
        {"RECURSIVE_DEPTH", ConfigKey::RECURSIVE_DEPTH},
        {"RESIZE_TIMEOUT",  ConfigKey::RESIZE_TIMEOUT},
        {"CHARS",           ConfigKey::CHARS},
        {"DITHERING",       ConfigKey::DITHERING},
        {"COLOR",           ConfigKey::COLOR}
};

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string & s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string & s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string & s) {
    return rtrim(ltrim(s));
}


ConfigKey Settings::getConfigKey(const std::string & key) {
    std::string modified_key = trim(key);
    std::transform(modified_key.begin(), modified_key.end(), modified_key.begin(), ::toupper);

    auto it = CONFIG_KEYS.find(key);
    if (it == CONFIG_KEYS.end()) {
        bool found = false;
        ConfigKey configKey = ConfigKey::NONE;
        for (const auto & item : CONFIG_KEYS) {
            if (item.first.rfind(modified_key, 0) == 0) {
                if (!found) {
                    configKey = item.second;
                    found = true;
                } else {
                    return ConfigKey::NONE;
                }
            }
        }
        return configKey;
    }
    return it->second;
}

bool getBool(const std::string & value, bool & valid) {
    if (equals(value, "true") || equals(value, "1") || equals(value, "on")) {
        valid = true;
        return true;
    } else if (equals(value, "false") || equals(value, "0") || equals(value, "off")) {
        valid = true;
        return false;
    } else {
        valid = false;
        return false;
    }
}

int getInt(const std::string & value, bool & valid) {
    try {
        valid = true;
        return std::stoi(value);
    } catch (std::invalid_argument & e) {
        valid = false;
        return 0;
    }
}

bool Settings::parseConfigField(ConfigKey key, const std::string & value) {
    bool valid;
    bool b_temp;
    int i_temp;
    switch (key) {
        case ConfigKey::RECURSIVE_DEPTH:
            i_temp = getInt(value, valid);
            if (valid) {
                m_max_depth = i_temp;
            }
            break;
        case ConfigKey::RESIZE_TIMEOUT:
            i_temp = getInt(value, valid);
            if (i_temp < 0) {
                valid = false;
            }
            if (valid) {
                m_resize_timeout = i_temp;
            }
            break;
        case ConfigKey::CHARS:
            m_chars = std::vector<char>(value.begin(), value.end());
            valid = true;
            break;
        case ConfigKey::COLOR:
            b_temp = getBool(value, valid);
            if (valid) {
                m_colors = b_temp;
            }
            break;
        case ConfigKey::DITHERING:
            b_temp = getBool(value, valid);
            if (valid) {
                m_dithering = b_temp;
            }
            break;
        case ConfigKey::NONE:
            valid = false;
            break;
    }

    std::string message = "Parsing config: ";
    bool found = key == ConfigKey::NONE;
    if (found) {
        message += "Invalid";
    }
    for (const auto & item : CONFIG_KEYS) {
        if (item.second == key) {
            message += item.first;
            found = true;
            break;
        }
    }
    if (!found) {
        message += "Unknown key: " + std::to_string((int) key);
    }
    message += " = ";
    message += value;

    if (valid) {
        Logger::log(message, LogLevel::INFO);
    } else {
        message += " (invalid)";
        if (key == ConfigKey::NONE) {
            message += " (unknown key)";
        }
        Logger::log(message, LogLevel::WARN);
    }
    return valid;
}

void Settings::parseConfigFile(const std::string_view & path) {
    if (!fs::exists(path)) {
        Logger::log("Config file does not exist: " + std::string(path), LogLevel::WARN);
        return;
    }
    Logger::log("Parsing config file: " + std::string(path), LogLevel::TRACE);
    std::ifstream file(path.data());
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                ConfigKey enumKey = getConfigKey(key);
                parseConfigField(enumKey, value);
            } else {
                Logger::log("Invalid config line: " + line, LogLevel::WARN);
            }
        } else {
            Logger::log("Invalid config line: " + line, LogLevel::WARN);
        }
    }
}
