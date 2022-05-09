#include <iostream>
#include "Menu.h"
#include "sources/DataSourceFactory.h"
#include <ncurses.h>
#include <filesystem>

namespace fs = std::filesystem;

Menu::Menu(const std::vector<std::string> & items) : m_current_index(0), m_frame_index(0) {
    for (const auto & item : items) {
        tryAddSource(item);
    }
}


void Menu::showStatus() const {
    size_t index = m_current_index + 1;
    size_t max_index = m_sources.size();
    size_t frame_index = m_frame_index + 1;
    size_t max_frame_index = m_sources[m_current_index]->frameCount();

    size_t char_count = 0;
    const char * format = "index %zu/%zu frame %zu/%zu";
    char_count += std::string(format).length();
    char_count += std::to_string(index).length();
    char_count += std::to_string(max_index).length();
    char_count += std::to_string(frame_index).length();
    char_count += std::to_string(max_frame_index).length();


    size_t middle = static_cast<size_t>(m_status_window->_maxx - m_status_window->_begx) / 2;
    if (middle < char_count / 2) {
        middle = 1;
    } else {
        middle -= char_count / 2;
    }
    wclear(m_status_window);
    box(m_status_window, 0, 0);
    mvwprintw(m_status_window, 1, (int) middle, format, index, max_index, frame_index, max_frame_index);
    wrefresh(m_status_window);
}


#if NCURSES_EXT_FUNCS>= 20181013

int getRoundedColorIndex(Color color) {
    return 16 + 36 * color.getRed() / 51 + 6 * color.getGreen() / 51 + color.getBlue() / 51;
}


int getColorPairId(int foregroundId, int backgroundId) {
    return foregroundId + (backgroundId << 8);
}

void initColorPairs() {
    for (int foregroundId = 0; foregroundId < 255; foregroundId++) {
        for (int backgroundId = 0; backgroundId < 255; backgroundId++) {
            init_extended_pair(
                    getColorPairId(foregroundId, backgroundId),
                    foregroundId,
                    backgroundId);
        }
    }
}

#endif

void Menu::show() const {
    wclear(m_main_window);
    showStatus();
    if (m_current_index >= m_sources.size()) {
        std::cerr << "Index out of range" << std::endl;
        return;
    }
    Frame & frame = m_sources[m_current_index]->getFrame(m_frame_index);
    Vector resolution = frame.getSize();
    Vector window_size = {static_cast<size_t>(m_main_window->_maxx - m_main_window->_begx),
                          static_cast<size_t>(m_main_window->_maxy - m_main_window->_begy)};
    wclear(m_main_window);
    box(m_main_window, 0, 0);
    std::shared_ptr<Frame> frame_ptr;
    if (m_settings.m_dithering) {
        frame_ptr = frame.clone();
    } else {
        frame_ptr = std::shared_ptr<Frame>(&frame, [](Frame *) {});
    }
    for (size_t y = 0; y < resolution.m_y && y < window_size.m_y - 1; y++) {
        for (size_t x = 0; x < resolution.m_x && x < window_size.m_x - 1; x++) {
            Color c = frame_ptr->getPixel({x, y});
            Color original_c = frame.getPixel({x, y});
            char ascii = m_settings.getChar(frame_ptr->getPixel({x, y}));
            Color rounded;
#if NCURSES_EXT_FUNCS>=20211021
            int colorIndex = getColorPairId(getRoundedColorIndex(m_settings.m_color_dithering ? c : original_c) % 255,
                                            COLOR_GREEN);
            rounded = Color{(c.getRed() / 51) * 51, (c.getBlue() / 51) * 51, (c.getBlue() / 51) * 51, c.getAlpha()};
#else
            rounded = m_settings.getRoundedColor(c);
#endif
            rounded = m_settings.getRoundedColor(c);
            if (m_settings.m_dithering) {

                Color diff = c - rounded;

                Vector cords;
                cords = {x + 1, y};
                if (x + 1 < resolution.m_x) {
                    Color c2 = frame_ptr->getPixel(cords);
                    c2 += diff * (7.0 / 16);
                    frame_ptr->setPixel(cords, c2);
                }
                cords = {x - 1, y + 1};
                if (y + 1 < resolution.m_y && x >= 1) {
                    Color c2 = frame_ptr->getPixel(cords);
                    c2 += diff * (3.0 / 16);
                    frame_ptr->setPixel(cords, c2);
                }
                cords = {x, y + 1};
                if (y + 1 < resolution.m_y) {
                    Color c2 = frame_ptr->getPixel(cords);
                    c2 += diff * (5.0 / 16);
                    frame_ptr->setPixel(cords, c2);
                }
                cords = {x + 1, y + 1};
                if (y + 1 < resolution.m_y && x + 1 < resolution.m_x) {
                    Color c2 = frame_ptr->getPixel(cords);
                    c2 += diff * (1.0 / 16);
                    frame_ptr->setPixel(cords, c2);
                }
            }

#if NCURSES_EXT_FUNCS>=20211021
            if (has_colors() && m_settings.m_colors) {
                wattron(m_main_window, COLOR_PAIR(colorIndex));
            }
#endif
            mvwaddch(m_main_window, y + 1, x + 1, ascii);

#if NCURSES_EXT_FUNCS>=20211021
            if (has_colors() && m_settings.m_colors) {
                wattroff(m_main_window, COLOR_PAIR(colorIndex));
            }
#endif
        }
    }
    wrefresh(m_main_window);
}

void Menu::input_loop() {
    while (true) {
        bool update = false;
        int c = wgetch(m_main_window);
        switch (c) {
            case KEY_LEFT:
                if (m_frame_index > 0) {
                    m_frame_index--;
                    update = true;
                } else if (m_current_index > 0) {
                    m_current_index--;
                    m_frame_index = m_sources[m_current_index]->frameCount() - 1;
                    update = true;
                }

                break;
            case KEY_RIGHT:
#ifdef DEBUG
                if (!m_sources[m_current_index]->frameCount()) {
                    std::cerr << "No frames" << std::endl;
                    exit(0);
                }
#endif
                if (m_frame_index < m_sources[m_current_index]->frameCount() - 1) {
                    m_current_index++;
                    update = true;
                } else if (m_current_index < m_sources.size() - 1) {
                    m_current_index++;
                    m_frame_index = 0;
                    update = true;
                }
                break;
            case 'd':
                m_settings.m_dithering = !m_settings.m_dithering;
                update = true;
                break;
            case 'c':
                m_settings.m_colors = !m_settings.m_colors;
                update = true;
                break;
            case 'x':
                m_settings.m_color_dithering = !m_settings.m_color_dithering;
                update = true;
                break;
            case 'q':
                return;
            case KEY_RESIZE:
                resize();
                update = true;
                break;
            case 'i':
                m_settings.m_red_offset += 10;
                update = true;
                break;
            case 'j':
                m_settings.m_red_offset -= 10;
                update = true;
                break;
            case 'o':
                m_settings.m_green_offset += 10;
                update = true;
                break;
            case 'k':
                m_settings.m_green_offset -= 10;
                update = true;
                break;
            case 'p':
                m_settings.m_blue_offset += 10;
                update = true;
                break;
            case 'l':
                m_settings.m_blue_offset -= 10;
                update = true;
                break;
            default:
                break;
        }
        if (update) {
            show();
        }
    }
}

void Menu::start() {
    if (m_sources.empty()) {
        throw std::runtime_error("0 sources found");
    }

    initscr();
    noecho(); // Don't repeat input
    cbreak(); // disable line buffering (waiting for newline)

#if NCURSES_EXT_FUNCS>=20211021
    if (has_colors()) {
        start_color();
        initColorPairs();
    }
#endif

    Vector resolution{};
    getmaxyx(stdscr, resolution.m_y, resolution.m_x);
    if (resolution.m_x < MINIMUM_WINDOW_SIZE.m_x && resolution.m_y < MINIMUM_WINDOW_SIZE.m_y) {
        endwin();
        std::cerr << "Terminal too small" << std::endl;
        exit(1);
    }
    Vector main_window = {resolution.m_x, resolution.m_y - STATUS_WINDOW_HEIGHT};
    WINDOW * window = newwin((int) main_window.m_y, (int) main_window.m_x, 0, 0);
    box(window, 0, 0); // 0, 0 gives default characters for the vertical and horizontal lines
    refresh();
    keypad(window, true); // enable arrow keys
    wrefresh(window);
    m_main_window = window;

    WINDOW * status_window = newwin(STATUS_WINDOW_HEIGHT, (int) main_window.m_x, (int) main_window.m_y, 0);
    box(status_window, 0, 0);
    keypad(status_window, true);
    wrefresh(status_window);
    m_status_window = status_window;

    show(); // Show the first frame
    input_loop();

    delwin(m_status_window);
    delwin(m_main_window);
    endwin();
}

void Menu::resize() {
    delwin(m_status_window);
    delwin(m_main_window);

    Vector resolution{};
    getmaxyx(stdscr, resolution.m_y, resolution.m_x);
    if (resolution.m_x < MINIMUM_WINDOW_SIZE.m_x && resolution.m_y < MINIMUM_WINDOW_SIZE.m_y) {
        endwin();
        std::cerr << "Terminal too small" << std::endl;
        exit(1);
    }
    Vector main_window = {resolution.m_x, resolution.m_y - STATUS_WINDOW_HEIGHT};
    WINDOW * window = newwin((int) main_window.m_y, (int) main_window.m_x, 0, 0);
    box(window, 0, 0); // 0, 0 gives default characters for the vertical and horizontal lines
    refresh();
    keypad(window, true); // enable arrow keys
    wrefresh(window);
    m_main_window = window;

    WINDOW * status_window = newwin(STATUS_WINDOW_HEIGHT, (int) main_window.m_x, (int) main_window.m_y, 0);
    box(status_window, 0, 0);
    keypad(status_window, true);
    wrefresh(status_window);
    m_status_window = status_window;

}

void Menu::tryAddSource(const std::string & path, size_t depth) {
    if (fs::is_directory(path) && depth < MAX_DEPTH) {
        for (const auto & entry : fs::directory_iterator(path))
            tryAddSource(entry.path(), depth + 1);
    } else {
        try {
            std::shared_ptr<DataSource> dataSource = DataSourceFactory::getDataSource(path);
            m_sources.push_back(dataSource);
            std::cout << "Added " << path << std::endl;
        } catch (std::exception & e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
