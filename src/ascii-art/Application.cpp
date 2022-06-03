#include <iostream>
#include "Application.h"
#include "sources/DataSourceFactory.h"
#include "sources/ImageFrame.h"
#include "menus/Gallery.h"
#include "./logging/Logger.h"
#include <ncurses.h>
#include <set>

#include <filesystem>

namespace fs = std::filesystem;

// TODO: Add argument flags to change settings
Application::Application(const std::vector<std::string> & args) : m_sources(
        std::make_shared<std::vector<std::shared_ptr<DataSource>>>()), m_settings(std::make_shared<Settings>()) {
    bool arguments = true;
    ConfigKey key = ConfigKey::NONE;
    for (const auto & item : args) {
        Logger::log("Argument: " + item, LogLevel::INFO);
        if (key != ConfigKey::NONE) {
            m_settings->parseConfigField(key, item);
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
                continue;
            }
        }
        tryAddSource(item);
    }
    m_current_menu = std::make_shared<Gallery>(m_sources, m_settings);
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
        switch (c) {
            case 'q':
                return;
            case 'r':
                update = true;
                break;
            case KEY_RESIZE:
                // TODO: smart resizing with timeouts
                m_current_menu->resize(getResolution());
                update = true;
                break;
            default:
                update = m_current_menu->input(c);
                break;
        }
        if (update) {
            m_current_menu->update();
        }
    }
}

void Application::start() {
    initscr();
    noecho(); // Don't repeat input
    cbreak(); // disable line buffering (waiting for newline)

    keypad(stdscr, TRUE); // enable keypad globally
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
    if (fs::is_directory(path) && depth < m_settings->m_max_depth) {
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
