#include <iostream>
#include "Application.h"
#include "sources/DataSourceFactory.h"
#include "sources/ImageFrame.h"
#include "menus/Gallery.h"
#include "./logging/Logger.h"
#include "menus/FileMenu.h"
#include <ncurses.h>
#include <set>

#include <filesystem>

constexpr int ctrl(int key) {
    return key & 0x1f;
}

namespace fs = std::filesystem;

// TODO: Add argument flags to change settings
Application::Application(const std::vector<std::string> & args) : m_sources(
        std::make_shared<std::vector<std::shared_ptr<DataSource>>>()), m_settings(std::make_shared<Settings>()) {
    bool arguments = true;
    ConfigKey key = ConfigKey::NONE;
    for (const auto & item : args) {
        Logger::log("Argument: " + item, LogLevel::INFO);
        if (key != ConfigKey::NONE) {
            bool valid = m_settings->parseConfigField(key, item);
            if (!valid) {
                Logger::log("Invalid argument: " + item + ", for field: " + to_string(key), LogLevel::ERROR);
            }
            key = ConfigKey::NONE;
            continue;
        }
        if (item == "--") {
            arguments = false;
            continue;
        }
        if (arguments && item.size() > 1) {
            if (item[0] == '-') {
                key = Settings::getConfigKey(item.substr(1));
                if (key == ConfigKey::NONE) {
                    Logger::log("Unknown argument: " + item, LogLevel::ERROR);
                }
                continue;
            }
        }
        tryAddSource(item);
    }
    if (m_sources->empty()) {
        m_current_menu = std::shared_ptr<Menu>(new FileMenu());
    } else {
        m_current_menu = std::make_shared<Gallery>(m_sources, m_settings);
    }
}


#ifdef  NCURSES_WIDE_COLOR_SUPPORT

int getRoundedColorIndex(Color color) {
    return 16 + (36 * (color.getRed() / 51) + 6 * (color.getGreen() / 51) + (color.getBlue() / 51));
}

int min(int a, int b) {
    return a < b ? a : b;
}

void initColorPairs() {
    // TODO: check for maximal number of colors and color pairs
    for (int red = 0; red <= 5; ++red) {
        for (int green = 0; green <= 5; ++green) {
            for (short blue = 0; blue <= 5; ++blue) {
                int foregroundId = getRoundedColorIndex(Color(red * 51, green * 51, blue * 51));
                init_extended_color(foregroundId, min(red * 51 * 4, 999), min(green * 51 * 4, 999),
                                    min(blue * 51 * 4, 999));
                init_extended_pair(foregroundId, COLOR_BLACK, foregroundId);
            }
        }
    }
}

#endif


void Application::input_loop() {
    while (true) {
        bool update = false;
        flushinp();
        int c = getch();

        Logger::log("Got key: " + std::to_string(c), LogLevel::TRACE);
        // Priority keys
        if (c == KEY_RESIZE) {
            Logger::log("Resize event", LogLevel::TRACE);

            m_last_resize = std::chrono::steady_clock::now();

            timeout(m_settings->m_resize_timeout);

            continue;
        }
        if (m_last_resize) {
            long duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - m_last_resize.value()).count();
            if (duration >= m_settings->m_resize_timeout) {
                Logger::log("Resize event at time: " + std::to_string(duration), LogLevel::TRACE);
                m_current_menu->resize(getResolution());
                m_current_menu->update();
                m_last_resize.reset();
                timeout(-1);
            } else {
                timeout(m_settings->m_resize_timeout - duration);
            }
        }
        if (c == ERR) {
            continue;
        }

        if (c == 4)
            return;
        bool handled = false;
        update = m_current_menu->input(c, handled);
        auto * file_menu = dynamic_cast<FileMenu *>(m_current_menu.get());
        if (!handled && !update) {
            switch (c) {
                case 'q':
                    return;
                case 'r':
                    update = true;
                    break;
                case ctrl('o'):
                    if (file_menu) {
                        auto files = file_menu->getSelectedFiles();
                        m_regex_save = file_menu->getRegex();
                        for (const auto & item : files)
                            tryAddSource(item);
                        if (!m_sources->empty()) {
                            m_current_menu = std::make_shared<Gallery>(m_sources, m_settings);
                            m_current_menu->show(getResolution());
                        }
                    } else {
                        if (m_regex_save) {
                            file_menu = new FileMenu(*m_regex_save);
                        } else {
                            file_menu = new FileMenu();
                        }

                        m_current_menu = std::shared_ptr<FileMenu>(file_menu);
                        for (const auto & item : *m_sources)
                            file_menu->selectFile(item->filename());
                        m_current_menu->show(getResolution());
                    }
                    update = true;
                    break;
                default:
                    break;
            }
        }

        if (update) {
            m_current_menu->update();
        }
        if (m_last_resize) {
            long duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - m_last_resize.value()).count();
            if (duration >= m_settings->m_resize_timeout) {
                Logger::log("Resize event at time: " + std::to_string(duration), LogLevel::TRACE);
                m_current_menu->resize(getResolution());
                m_current_menu->update();
                m_last_resize.reset();
                timeout(-1);
            } else {
                timeout(m_settings->m_resize_timeout - duration);
            }
        }
    }

}

void Application::start() {
    initscr();
    noecho(); // Don't repeat input
    cbreak(); // disable line buffering (waiting for newline)

    keypad(stdscr, TRUE); // enable keypad globally
    mouseinterval(0);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    Logger::log("Mouse support: "+std::to_string(has_mouse()), LogLevel::TRACE);
#ifdef NCURSES_WIDE_COLOR_SUPPORT
    if (has_colors()) {
        start_color();
        initColorPairs();
        Logger::log("Colors enabled: " + std::to_string(COLORS) + " Color pairs:" + std::to_string(COLOR_PAIRS),
                    LogLevel::TRACE);
    }
#endif

    Vector resolution = getResolution();
    m_current_menu->show(resolution); // Show the first frame
    m_current_menu->update(); // Update the first frame
    input_loop();

    m_current_menu = nullptr;
    endwin();
}

void Application::tryAddSource(const std::string & path, size_t depth) {
    if (depth > m_settings->m_max_depth)
        return;
    if (fs::is_directory(path)) {
        std::set<fs::path> sorted_by_name;
        try {
            for (const auto & entry : fs::directory_iterator(path))
                sorted_by_name.insert(entry.path());
            for (auto & filename : sorted_by_name)
                tryAddSource(filename, depth + 1);
        } catch (fs::filesystem_error & e) {
            Logger::log(e.what(), LogLevel::INFO);
        }
    } else {
        try {
            std::shared_ptr<DataSource> dataSource = DataSourceFactory::getDataSource(path);
            dataSource->getFrame(0);
            m_sources->push_back(dataSource);
            Logger::log("Added: " + path, LogLevel::TRACE);
        } catch (std::exception & e) {
            Logger::log(e.what(), LogLevel::INFO);
        }
    }
}

Vector Application::getResolution() {
    Vector resolution{};
    getmaxyx(stdscr, resolution.m_y, resolution.m_x);
    return resolution;
}
