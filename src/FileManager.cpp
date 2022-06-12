#include "FileManager.h"
#include "ascii-art/logging/Logger.h"
#include <unistd.h>
#include <pwd.h>
#include <regex>

std::optional<std::string> FileManager::home;

std::string & FileManager::get_home() {
    if (FileManager::home.has_value()) return FileManager::home.value();
    const char * homedir;

    if ((homedir = getenv("HOME")) == nullptr) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    FileManager::home = homedir;
    return FileManager::home.value();
}

bool FileManager::is_separator(char c) {
    return c == PATH_SEPARATOR;
}

std::string FileManager::normalize_path(const std::string_view & path, bool remove_home) {
    std::string result;
    bool last_was_separator = false;
    for (char i : path) {
        if (is_separator(i)) {
            if (!last_was_separator)
                result += PATH_SEPARATOR;
            last_was_separator = true;
            continue;
        }
        last_was_separator = false;
        if (i == '~' && remove_home) {
            result += get_home();
            continue;
        }
        result += i;
    }
    return result;

}

std::set<fs::path> FileManager::find_files(const std::string_view & regex) {
    std::string path_regex = normalize_path(regex);
    size_t visited = 0;
    std::set<fs::path> files;
    std::set<fs::path> folders;
    std::set<fs::path> next_folders;
    if (path_regex.size() < 2) return files; // TODO: throw exception
    if (path_regex[0] == PATH_SEPARATOR) {
        folders.insert("/");
        path_regex = path_regex.substr(1);
    } else if (path_regex[0] == '~' && path_regex[1] == PATH_SEPARATOR) {
        path_regex = path_regex.substr(2);
        folders.insert(get_home());
    } else {
        folders.insert(fs::current_path());
    }

    size_t pos = 0;
    std::string token;
    while (!path_regex.empty()) {
        pos = path_regex.find(PATH_SEPARATOR);
        if (pos == std::string::npos) {
            token = path_regex;
            path_regex.clear();
        } else {
            token = path_regex.substr(0, pos);
            path_regex.erase(0, pos + 1);
        }
        if (!token.empty() && token[0] == '?') {
            next_folders.insert(folders.begin(), folders.end());
            token = token.substr(1);
        }
        std::regex token_regex;
        try {
            token_regex = token;
        } catch (std::regex_error & e) {
            Logger::log("Invalid regex: " + token, LogLevel::ERROR);
            return files;
        }
        for (const auto & folder : folders) {
            if (!path_regex.empty() && (token == "\\.\\." || token == ".." || token == "\\.." || token == ".\\.")) {
                next_folders.insert(folder.parent_path());
                continue;
            }
            if (!path_regex.empty() && (token == "\\." || token == ".")) {
                next_folders.insert(folder);
                continue;
            }
            try {
                fs::directory_iterator it(folder);
                for (auto & entry : it) {
                    visited++;
                    if (visited >= MAXIMUM_VISITED_FILES) {
                        Logger::log("Maximum files visited:" + std::to_string(visited), LogLevel::WARN);
                        return files;
                    }
                    if (fs::is_directory(entry)) {
                        if (!path_regex.empty() && std::regex_match(entry.path().filename().string(), token_regex)) {
                            next_folders.insert(entry);
                        }
                    } else if (path_regex.empty()) {
                        if (std::regex_match(entry.path().filename().string(), token_regex)) {
                            files.insert(entry);
                        }
                    }
                }
            } catch (std::exception & exception) {
                Logger::log("Error while trying to itarate trough " + folder.string() + ", e:" + exception.what(),
                            LogLevel::WARN);
            }
        }
        folders = std::move(next_folders);
        next_folders = std::set<fs::path>();
    }
    return files;
}

void FileManager::removeRegexPart(std::string & regex, size_t & current_position) {
    size_t first_after = regex.find(FileManager::PATH_SEPARATOR, current_position);
    size_t last_before = regex.rfind(PATH_SEPARATOR, current_position);
    last_before = last_before == std::string::npos ? 0 : last_before;
    regex.erase(last_before, first_after - last_before);
    current_position = last_before;
}
