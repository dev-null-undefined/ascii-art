#include <cctype>
#include <cstring>
#include <string>
#include "FileMenu.h"
#include "../../FileManager.h"

constexpr int ctrl(int key) {
    return (key) & 0x1f;
}

void FileMenu::show(Vector initial_size) {
    curs_set(1);
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    // TODO: don't use forms
    m_window = newwin((int) initial_size.m_y, (int) initial_size.m_x, 0, 0);
    m_window_size = initial_size - Vector{2, 2};
    box(m_window, 0, 0);

    refresh();
    update();

}

void FileMenu::resize(Vector size) {
    // TODO: minimal size
    m_window_size = size - Vector{2, 2};
    wclear(m_window);
    wresize(m_window, (int) size.m_y, (int) size.m_x);
    box(m_window, 0, 0);

    refresh();
    update();
}

void FileMenu::hide() {
    delwin(m_window);
}

void FileMenu::update() const {
    mvwprintw(m_window, 0, 1, "File Menu:");
    int i = 1;
    mvwprintw(m_window, m_window_size.m_y + 1, i,
              "^A all,");
    i += 6 + 2;
    mvwprintw(m_window, m_window_size.m_y + 1, i,
              "^R none,");
    i += 7 + 2;
    mvwprintw(m_window, m_window_size.m_y + 1, i,
              "^I inverse,");
    i += 10 + 2;
    mvwprintw(m_window, m_window_size.m_y + 1, i,
              "ENTER select,");
    i += 12 + 2;
    mvwprintw(m_window, m_window_size.m_y + 1, i,
              "^O open,");
    i += 7 + 2;
    mvwprintw(m_window, m_window_size.m_y + 1, i,
              "^D quit");
    char * spaces = new char[m_window_size.m_x];
    memset(spaces, ' ', m_window_size.m_x);
    spaces[m_window_size.m_x - 1] = '\0';
    for (size_t i = 3; i <= m_window_size.m_y; ++i) {
        mvwprintw(m_window, (int) i, 1, "%s", spaces);
    }
    delete[] spaces;

    auto iter = m_files.begin();
    auto iter_selected = m_selected_files.begin();

    for (size_t i = 0;
         i < m_scroll && (iter != m_files.end() || iter_selected != m_selected_files.end()); ++i) {
        if (iter_selected == m_selected_files.end()) {
            ++iter;
        } else {
            ++iter_selected;
        }
    }

    size_t iter_index = 0;
    for (size_t i = 3;
         i <= m_window_size.m_y && (iter != m_files.end() || iter_selected != m_selected_files.end()); ++i) {
        if (i == m_index + 3) {
            wattron(m_window, A_UNDERLINE);
        }
        if (iter_selected == m_selected_files.end()) {
            mvwprintw(m_window, (int) i, 1, "%s", iter->c_str());
            ++iter;
            ++iter_index;
        } else {
            wattron(m_window, A_BOLD); // TODO: use color?
            mvwprintw(m_window, (int) i, 1, "%s", iter_selected->c_str());
            wattroff(m_window, A_BOLD);
            ++iter_selected;
        }
        if (i == m_index + 3) {
            wattroff(m_window, A_UNDERLINE);
        }
    }
    mvwprintw(m_window, 1, 1, "%s", (m_regex + " ").c_str());
    wmove(m_window, 1, (int) m_regex_index + 1);
    wrefresh(m_window);
}


bool FileMenu::input(int input, bool & handled) {
    handled = true;
    std::set<fs::path> new_selected_files;
    size_t actual_index = m_index + m_scroll;
    switch (input) {
        case KEY_UP:
            return key_up();
        case KEY_DOWN:
            return key_down();
        case KEY_LEFT:
            if (m_regex_index > 0) {
                m_regex_index--;
                return true;
            }
            break;
        case KEY_RIGHT:
            if (m_regex_index < m_regex.size()) {
                m_regex_index++;
                return true;
            }
            break;
        case KEY_HOME:
            if (m_regex_index != 0) {
                m_regex_index = 0;
                return true;
            }
            break;
        case KEY_END:
            if (m_regex_index != m_regex.size()) {
                m_regex_index = m_regex.size();
                return true;
            }
            break;
        case KEY_BACKSPACE:
        case 127:
            // Delete the char before cursor
            if (!m_regex.empty() && m_regex_index > 0) {
                m_regex.erase(m_regex_index - 1, 1);
                m_regex_index--;
                update_files(m_regex);
                return true;
            }
            break;
        case KEY_DC:
            // Delete the char under the cursor
            if (!m_regex.empty() && m_regex_index < m_regex.size()) {
                m_regex.erase(m_regex_index, 1);
                update_files(m_regex);
                return true;
            }
            break;
        case '\n':
            if (actual_index < m_files.size() + m_selected_files.size() && !m_files.empty()) {
                std::string path;
                if (actual_index < m_selected_files.size()) {
                    path = *std::next(m_selected_files.begin(), (long) actual_index);
                } else {
                    path = *std::next(m_files.begin(), (long) (actual_index - m_selected_files.size()));
                }
                if (m_selected_files.count(path) == 0) {
                    m_selected_files.insert(std::move(path));
                    key_down();
                } else {
                    m_selected_files.erase(std::move(path));
                    key_up();
                    update_index();
                }
                return true;
            }
            break;
        case ctrl(' '):
            m_regex.insert(m_regex_index, ".*");
            m_regex_index += 2;
            update_files(m_regex);
            return true;
        case ctrl('a'):
            m_selected_files.insert(m_files.begin(), m_files.end());
            return true;
        case ctrl('r'):
            m_selected_files.clear();
            return true;
        case ctrl('i'):
            new_selected_files.insert(m_files.begin(), m_files.end());
            for (const auto & item : m_selected_files)
                new_selected_files.erase(item);
            m_selected_files = std::set<fs::path>(new_selected_files);
            return true;
        case KEY_MOUSE:
            return handle_mouse();
        default:
            if (isprint(input)) {
                m_regex.insert(m_regex_index, 1, (char) input);
                m_regex_index++;
                update_files(m_regex);
                return true;
            } else {
                handled = false;
            }
            break;
    }
    return false;
}

FileMenu::~FileMenu() {
    FileMenu::hide();
}

void FileMenu::update_files(const std::string & regex) {
    m_files = FileManager::find_files(regex);
    update_index();
}

void FileMenu::selectFile(const std::string_view & view) {
    m_selected_files.insert(view);
}

const std::set<fs::path> & FileMenu::getSelectedFiles() const {
    return m_selected_files;
}

std::string FileMenu::getRegex() const {
    return m_regex;
}

FileMenu::FileMenu(const std::string & m_regex) : m_regex(m_regex), m_regex_index(m_regex.size()) {
    update_files(m_regex);
}

bool FileMenu::key_down() {
    size_t actual_index = m_index + m_scroll;
    if (m_index < m_window_size.m_y) {
        if (m_index < m_window_size.m_y - MINIMAL_GAP - 2) {
            if (actual_index < m_files.size() + m_selected_files.size() - 1) {
                m_index++;
                return true;
            }
        } else {
            if (actual_index < m_files.size() + m_selected_files.size() - 1) {
                m_scroll++;
                return true;
            }
        }
    }
    return false;
}

bool FileMenu::key_up() {
    if (m_index > 0) {
        if (m_index >= MINIMAL_GAP) {
            m_index--;
            return true;
        } else {
            if (m_scroll > 0) {
                m_scroll--;
            } else {
                m_index--;
            }
            return true;
        }
    }
    return false;
}

void FileMenu::update_index() {
    if (m_index + m_scroll >= m_files.size() + m_selected_files.size()) {
        if (m_scroll >= m_files.size() + m_selected_files.size()) {
            m_scroll = 0;
        }
        m_index = m_files.size() +
                  m_selected_files.size() - 1 - m_scroll;
    }
}

bool FileMenu::handle_mouse() {
    MEVENT event;
    if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_PRESSED) {
            int actual_index = (int) (event.y + m_scroll) - 3;
            if (event.y < m_window_size.m_y && event.y > 1 && actual_index < m_files.size() + m_selected_files.size() &&
                !m_files.empty()) {
                std::string path;
                if (actual_index < m_selected_files.size()) {
                    path = *std::next(m_selected_files.begin(), (long) actual_index);
                } else {
                    path = *std::next(m_files.begin(), (long) (actual_index - m_selected_files.size()));
                }
                if (m_selected_files.count(path) == 0) {
                    m_selected_files.insert(std::move(path));
                    key_down();
                } else {
                    m_selected_files.erase(std::move(path));
                    key_up();
                    update_index();
                }
                return true;

            } else if (event.y == 1 && event.x < m_window_size.m_x) {
                if (event.x - 1 <= m_regex.size()) {
                    m_regex_index = event.x - 1;
                    return true;
                } else {
                    m_regex_index = m_regex.size();
                    return true;
                }

            }

        }
    }
    return false;
}
