#include "Gallery.h"
#include "../sources/ImageFrame.h"
#include "../Application.h"
#include "../logging/Logger.h"
#include <iostream>
#include <utility>

void Gallery::show(Vector initial_size) {
    curs_set(0);
    if (initial_size.m_x < MINIMUM_WINDOW_SIZE.m_x && initial_size.m_y < MINIMUM_WINDOW_SIZE.m_y) {
        Logger::log("Terminal too small", LogLevel::FATAL);
        throw std::runtime_error("Terminal too small!");
    }
    m_main_window_size = {initial_size.m_x, initial_size.m_y - STATUS_WINDOW_HEIGHT};
    WINDOW * window = newwin((int) m_main_window_size.m_y, (int) m_main_window_size.m_x, 0, 0);
    box(window, 0, 0); // 0, 0 gives default characters for the vertical and horizontal lines
    refresh();
    keypad(window, true); // enable arrow keys
    wrefresh(window);
    m_main_window = window;

    WINDOW * status_window = newwin(STATUS_WINDOW_HEIGHT, (int) m_main_window_size.m_x, (int) m_main_window_size.m_y,
                                    0);
    box(status_window, 0, 0);
    keypad(status_window, true);
    wrefresh(status_window);
    m_status_window = status_window;
}

void Gallery::resize(Vector size) {
    delwin(m_status_window);
    delwin(m_main_window);

    if (size.m_x < MINIMUM_WINDOW_SIZE.m_x && size.m_y < MINIMUM_WINDOW_SIZE.m_y) {
        endwin();
        std::cerr << "Terminal too small" << std::endl;
        exit(1);
    }
    m_main_window_size = {size.m_x, size.m_y - STATUS_WINDOW_HEIGHT};
    // TODO: fix memleaks
    WINDOW * window = newwin((int) m_main_window_size.m_y, (int) m_main_window_size.m_x, 0, 0);
    box(window, 0, 0); // 0, 0 gives default characters for the vertical and horizontal lines
    refresh();
    keypad(window, true); // enable arrow keys
    wrefresh(window);
    m_main_window = window;

    WINDOW * status_window = newwin(STATUS_WINDOW_HEIGHT, (int) m_main_window_size.m_x, (int) m_main_window_size.m_y,
                                    0);
    box(status_window, 0, 0);
    keypad(status_window, true);
    wrefresh(status_window);
    m_status_window = status_window;
}

void Gallery::showStatus() const {
    // TODO: modal status bar
    size_t index = m_current_index + 1;
    size_t max_index = m_sources->size();
    size_t frame_index = m_frame_index + 1;
    size_t max_frame_index = m_sources->at(m_current_index)->frameCount();
    std::string_view file_name = m_sources->at(m_current_index)->filename();

    size_t char_count = 0;
    const char * format = "%s %zu/%zu frame %zu/%zu %d %d";
    char_count += std::string(format).length();
    char_count += file_name.length();
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
    mvwprintw(m_status_window, 1, (int) middle, format, std::string(file_name).c_str(), index, max_index, frame_index,
              max_frame_index, m_settings->m_dithering, m_settings->m_color_dithering);
    wrefresh(m_status_window);
}

void Gallery::hide() {
    curs_set(1);
    delwin(m_status_window);
    delwin(m_main_window);
}


void Gallery::update() const {
    werase(m_main_window);
    showStatus();
    if (m_current_index >= m_sources->size()) {
        std::cerr << "Index out of range" << std::endl;
        return;
    }
    Frame & frame = m_sources->at(m_current_index)->getFrame(m_frame_index);
    box(m_main_window, 0, 0);
    std::shared_ptr<Frame> frame_ptr = frame.clone();

    Matrix<Color> pixels = frame_ptr->getPixels();

    auto resized = std::shared_ptr<Frame>(new ImageFrame(pixels.resize(m_main_window_size-Vector{2,2})));
    auto original = std::shared_ptr<Frame>(new ImageFrame(pixels.resize(m_main_window_size-Vector{2,2})));
// TODO: dithering and color dithering before resize
    Vector resolution = original->getSize();
    for (size_t y = 0; y < resolution.m_y && y < m_main_window_size.m_y - 2; y++) {
        for (size_t x = 0; x < resolution.m_x && x < m_main_window_size.m_x - 2; x++) {
            Color color = resized->getPixel({x, y});
            Color original_c = original->getPixel({x, y});
            char ascii = m_settings->getChar(resized->getPixel({x, y}).normalize());
            Color rounded;

#ifdef NCURSES_WIDE_COLOR_SUPPORT
            Color used = (m_settings->modifiedColor(m_settings->m_color_dithering ? color : original_c));

            int colorIndex = getRoundedColorIndex(used.normalize());
            rounded = Color{(color.getRed() / 51) * 51, (color.getGreen() / 51) * 51, (color.getBlue() / 51) * 51,
                            color.getAlpha()};
#else
            rounded = m_settings.getRoundedColor(color);
#endif
            if (m_settings->m_dithering) {

                Color diff = color - rounded;

                Vector cords;
                cords = {x + 1, y};
                if (x + 1 < resolution.m_x) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (7.0 / 16);
                    resized->setPixel(cords, c2);
                }
                cords = {x - 1, y + 1};
                if (y + 1 < resolution.m_y && x >= 1) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (3.0 / 16);
                    resized->setPixel(cords, c2);
                }
                cords = {x, y + 1};
                if (y + 1 < resolution.m_y) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (5.0 / 16);
                    resized->setPixel(cords, c2);
                }
                cords = {x + 1, y + 1};
                if (y + 1 < resolution.m_y && x + 1 < resolution.m_x) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (1.0 / 16);
                    resized->setPixel(cords, c2);
                }
            }

#ifdef NCURSES_WIDE_COLOR_SUPPORT
            if (has_colors() && m_settings->m_colors) {
                wattron(m_main_window, COLOR_PAIR(colorIndex));
            }
#endif
            mvwaddch(m_main_window, y + 1, x + 1, ascii);

#ifdef NCURSES_WIDE_COLOR_SUPPORT
            if (has_colors() && m_settings->m_colors) {
                wattroff(m_main_window, COLOR_PAIR(colorIndex));
            }
#endif
        }
    }
    wrefresh(m_main_window);
}

bool Gallery::input(int input) {
    bool update = false;
    switch (input) {
        case KEY_LEFT:
            if (m_frame_index > 0) {
                m_frame_index--;
                update = true;
            } else if (m_current_index > 0) {
                m_current_index--;
                m_frame_index = m_sources->at(m_current_index)->frameCount() - 1;
                update = true;
            }

            break;
        case KEY_RIGHT:
#ifdef DEBUG
            if (!m_sources->at(m_current_index)->frameCount()) {
                std::cerr << "No frames" << std::endl;
                exit(0);
            }
#endif
            if (m_frame_index < m_sources->at(m_current_index)->frameCount() - 1) {
                m_current_index++;
                update = true;
            } else if (m_current_index < m_sources->size() - 1) {
                m_current_index++;
                m_frame_index = 0;
                update = true;
            }
            break;
        case 'd':
            m_settings->m_dithering = !m_settings->m_dithering;
            update = true;
            break;
        case 'c':
            m_settings->m_colors = !m_settings->m_colors;
            update = true;
            break;
        case 'x':
            m_settings->m_color_dithering = !m_settings->m_color_dithering;
            update = true;
            break;
        case 'i':
            m_settings->m_color_filter.m_red_offset += 10;
            update = true;
            break;
        case 'j':
            m_settings->m_color_filter.m_red_offset -= 10;
            update = true;
            break;
        case 'o':
            m_settings->m_color_filter.m_green_offset += 10;
            update = true;
            break;
        case 'k':
            m_settings->m_color_filter.m_green_offset -= 10;
            update = true;
            break;
        case 'p':
            m_settings->m_color_filter.m_blue_offset += 10;
            update = true;
            break;
        case 'l':
            m_settings->m_color_filter.m_blue_offset -= 10;
            update = true;
            break;
        case 's':
            m_settings->m_debug = !m_settings->m_debug;
            update = true;
            break;
        default:
            break;
    }
    return update;
}

Gallery::Gallery(std::shared_ptr<std::vector<std::shared_ptr<DataSource>>> m_sources,
                 std::shared_ptr<Settings> m_settings) : m_sources(std::move(m_sources)),
                                                         m_settings(std::move(m_settings)), m_current_index(0),
                                                         m_frame_index(0) {
}

Gallery::~Gallery() {
    Gallery::hide();
}
