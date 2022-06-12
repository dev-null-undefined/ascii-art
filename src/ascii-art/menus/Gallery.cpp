#include "Gallery.h"
#include "../sources/ImageFrame.h"
#include "../Application.h"
#include "../logging/Logger.h"
#include <iostream>
#include <utility>

WINDOW * boxed_window(int height, int width, int y, int x) {
    WINDOW * status_window = newwin(height, width, y, x);
    box(status_window, 0, 0);
    keypad(status_window, true);
    wrefresh(status_window);
    return status_window;
}

void Gallery::show(Vector initial_size) {
    curs_set(0);
    if (initial_size.m_x < MINIMUM_WINDOW_SIZE.m_x && initial_size.m_y < MINIMUM_WINDOW_SIZE.m_y) {
        Logger::log("Terminal too small", LogLevel::FATAL);
        throw std::runtime_error("Terminal too small!");
    }
    m_main_window_size = {initial_size.m_x, initial_size.m_y - STATUS_WINDOW_HEIGHT};
    m_main_window = boxed_window(static_cast<int>(m_main_window_size.m_y), static_cast<int>(m_main_window_size.m_x), 0,
                                 0);
    m_status_window = boxed_window(STATUS_WINDOW_HEIGHT, static_cast<int>(m_main_window_size.m_x),
                                   static_cast<int>(m_main_window_size.m_y), 0);
    m_image_size = m_main_window_size;
    refresh();
}

void Gallery::resize(Vector size) {
    if (size.m_x < MINIMUM_WINDOW_SIZE.m_x && size.m_y < MINIMUM_WINDOW_SIZE.m_y) {
        Logger::log("Terminal too small", LogLevel::FATAL);
        throw std::runtime_error("Terminal too small!");
    }

    werase(m_main_window);

    m_main_window_size = {size.m_x, size.m_y - STATUS_WINDOW_HEIGHT};
    wresize(m_main_window, (int) size.m_y - STATUS_WINDOW_HEIGHT, (int) size.m_x);

    mvwin(m_status_window, m_main_window_size.m_y, 0);
    wresize(m_status_window, STATUS_WINDOW_HEIGHT, (int) m_main_window_size.m_x);
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
    werase(m_status_window);
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
    Frame & frame = m_sources->at(m_current_index)->getFrame(
            m_frame_index); // TODO: handle exception with invalid format while loading
    box(m_main_window, 0, 0);

    Matrix<Color> pixels = frame.getPixels();

    Vector maximum_size = (m_main_window_size - Vector{2, 2}) * m_image_scale;
    auto resized = std::shared_ptr<Frame>(
            new ImageFrame(pixels.resize(maximum_size, m_settings->m_image_scale_factor)));
    auto original = resized->clone();
    m_image_size = resized->getSize();
    // TODO: dithering and color dithering before resize
    Vector resolution = original->getSize();
    for (size_t y = 0, img_y = m_image_position.m_y;
         img_y < resolution.m_y && y < m_main_window_size.m_y - 2; y++, img_y++) {
        for (size_t x = 0, img_x = m_image_position.m_x;
             img_x < resolution.m_x && x < m_main_window_size.m_x - 2; x++, img_x++) {
            Color color = resized->getPixel({img_x, img_y});
            Color original_c = original->getPixel({img_x, img_y});
            char ascii = m_settings->getChar(resized->getPixel({img_x, img_y}).normalize());
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
                cords = {img_x + 1, img_y};
                if (img_x + 1 < resolution.m_x) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (7.0 / 16);
                    resized->setPixel(cords, c2);
                }
                cords = {img_x - 1, img_y + 1};
                if (img_y + 1 < resolution.m_y && img_x >= 1) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (3.0 / 16);
                    resized->setPixel(cords, c2);
                }
                cords = {img_x, img_y + 1};
                if (img_y + 1 < resolution.m_y) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (5.0 / 16);
                    resized->setPixel(cords, c2);
                }
                cords = {img_x + 1, img_y + 1};
                if (img_y + 1 < resolution.m_y && img_x + 1 < resolution.m_x) {
                    Color c2 = resized->getPixel(cords);
                    c2 += diff * (1.0 / 16);
                    resized->setPixel(cords, c2);
                }
            }

#ifdef NCURSES_WIDE_COLOR_SUPPORT
            if (has_colors() && m_settings->m_colors && m_settings->m_supports_colors) {
                wattron(m_main_window, COLOR_PAIR(colorIndex));
            }
#endif
            mvwaddch(m_main_window, y + 1, x + 1, ascii);

#ifdef NCURSES_WIDE_COLOR_SUPPORT
            if (has_colors() && m_settings->m_colors && m_settings->m_supports_colors) {
                wattroff(m_main_window, COLOR_PAIR(colorIndex));
            }
#endif
        }
    }
    wrefresh(m_main_window);
}

bool Gallery::input(int input, bool & handled) {
    bool update = false;
    switch (input) {
        case KEY_LEFT:
            update = true;
            if (m_frame_index > 0) {
                m_frame_index--;
            } else if (m_current_index > 0) {
                m_current_index--;
                m_frame_index = m_sources->at(m_current_index)->frameCount() - 1;
            }
            break;
        case KEY_RIGHT:
#ifdef DEBUG
            if (!m_sources->at(m_current_index)->frameCount()) {
                std::cerr << "No frames" << std::endl;
                exit(0);
            }
#endif
            update = true;
            if (m_frame_index < m_sources->at(m_current_index)->frameCount() - 1) {
                m_current_index++;
            } else if (m_current_index < m_sources->size() - 1) {
                m_current_index++;
                m_frame_index = 0;
            }
            break;
        case 'u':
            if (m_settings->m_image_scale_factor + 0.1 < Vector::MAXIMUM_SCALE_FACTOR) {
                m_settings->m_image_scale_factor += 0.1;
                Logger::log("Image scale factor change +" + std::to_string(m_settings->m_image_scale_factor),
                            LogLevel::TRACE);
                update = true;
            }
            break;
        case 'h':
            if (m_settings->m_image_scale_factor - 0.1 > Vector::MINIMUM_SCALE_FACTOR) {
                m_settings->m_image_scale_factor -= 0.1;
                Logger::log("Image scale factor change -" + std::to_string(m_settings->m_image_scale_factor),
                            LogLevel::TRACE);
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
            m_settings->m_color_filter.m_red_offset += COLOR_FILTER_ADDITION;
            update = true;
            break;
        case 'j':
            m_settings->m_color_filter.m_red_offset -= COLOR_FILTER_ADDITION;
            update = true;
            break;
        case 'o':
            m_settings->m_color_filter.m_green_offset += COLOR_FILTER_ADDITION;
            update = true;
            break;
        case 'k':
            m_settings->m_color_filter.m_green_offset -= COLOR_FILTER_ADDITION;
            update = true;
            break;
        case 'p':
            m_settings->m_color_filter.m_blue_offset += COLOR_FILTER_ADDITION;
            update = true;
            break;
        case 'l':
            m_settings->m_color_filter.m_blue_offset -= COLOR_FILTER_ADDITION;
            update = true;
            break;
        case 's':
            m_settings->m_debug = !m_settings->m_debug;
            update = true;
            break;
        case KEY_MOUSE:
            update = handle_mouse();
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


void Gallery::move(int x, int y) {
    Vector image_size = m_image_size * m_image_scale; // use image size instead
    size_t sx = x > 0 ? x : 0;
    size_t sy = y > 0 ? y : 0;
    m_image_position.m_x = sx < image_size.m_x / 2 ? sx : image_size.m_x / 2;
    m_image_position.m_y = sy < image_size.m_y / 2 ? sy : image_size.m_y / 2;
}

void Gallery::zoom(int x, int y, double zoom) {
    double zoom_save = m_image_scale;
    Vector image_size_before = m_image_size;
    m_image_scale = m_image_scale * zoom;
    if (m_image_scale < MINIMUM_ZOOM) {
        m_image_scale = MINIMUM_ZOOM;
    } else if (m_image_scale > MAXIMUM_ZOOM) {
        m_image_scale = MAXIMUM_ZOOM;
    }
    Vector image_size_after = m_image_size * (m_image_scale / zoom_save);
    int delta_x = ((int) image_size_after.m_x - (int) image_size_before.m_x);
    int delta_y = ((int) image_size_after.m_y - (int) image_size_before.m_y);

    double new_x = (double) m_image_position.m_x + x / (double) m_main_window_size.m_x * delta_x;
    double new_y = (double) m_image_position.m_y + y / (double) m_main_window_size.m_y * delta_y;
    move(new_x, new_y);
}

bool Gallery::handle_mouse() {
    MEVENT event;
    if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_PRESSED) {
            m_last_mouse_position = {static_cast<size_t>(event.x), static_cast<size_t>(event.y)};
        } else if (event.bstate & BUTTON1_RELEASED) {
            Vector pos = m_image_position + m_last_mouse_position;
            move((int) pos.m_x - event.x, (int) pos.m_y - event.y);
            return true;
        } else if (event.bstate & 65536) {
            zoom(event.x, event.y, 1.1);
            return true;
        } else if (event.bstate & 2097152) {
            zoom(event.x, event.y, 0.9);
            return true;
        }
    }
    return false;
}