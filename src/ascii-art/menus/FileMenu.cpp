#include <cctype>
#include <cstring>
#include <string>
#include "FileMenu.h"
#include "../../FileManager.h"

#define ctrl(x)           ((x) & 0x1f)

void FileMenu::show(Vector initial_size) {
    curs_set(1);
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    // TODO: don't use forms
    m_window = newwin(initial_size.m_y, initial_size.m_x, 0, 0);
    m_window_size = initial_size - Vector{2, 2};
    box(m_window, 0, 0);

    refresh();
    update();

}

void FileMenu::resize(Vector size) {
    // TODO: minimal size
    m_window_size = size - Vector{2, 2};
    wclear(m_window);
    wresize(m_window, size.m_y, size.m_x);
    box(m_window, 0, 0);

    refresh();
    update();
}

void FileMenu::hide() {
    delwin(m_window);
}

void FileMenu::update() const {
    char * spaces = new char[m_window_size.m_x];
    memset(spaces, ' ', m_window_size.m_x);
    spaces[m_window_size.m_x - 1] = '\0';
    for (size_t i = 3; i < m_window_size.m_y - 1; ++i) {
        mvwprintw(m_window, (int) i, 1, "%s", spaces);
    }
    auto iter = m_files.begin();
    auto iter_selected = m_selected_files.begin();
    size_t iter_index = 0;
    for (size_t i = 3;
         i < m_window_size.m_y - 1 && (iter != m_files.end() || iter_selected != m_selected_files.end()); ++i) {
        if (iter_selected == m_selected_files.end()) {
            if (iter_index == m_index) {
                wattron(m_window, A_UNDERLINE);
            }
            mvwprintw(m_window, (int) i, 1, "%s", iter->c_str());
            if (iter_index == m_index) {
                wattroff(m_window, A_UNDERLINE);
            }
            ++iter;
            ++iter_index;
        } else {
            wattron(m_window, A_BOLD); // TODO: use color?
            mvwprintw(m_window, (int) i, 1, "%s", iter_selected->c_str());
            wattroff(m_window, A_BOLD);
            ++iter_selected;
        }
    }
    mvwprintw(m_window, 1, 1, "%s", (m_regex + " ").c_str());
    wmove(m_window, 1, m_regex_index + 1);
    wrefresh(m_window);
}


bool FileMenu::input(int input, bool & handled) {
    handled = true;
    std::set<fs::path> new_selected_files;
    switch (input) {
        case KEY_LEFT:
            if (m_regex_index > 0) {
                m_regex_index--;
                return true;
            }
            break;
        case KEY_RIGHT:
            if (m_regex_index <= m_regex.size()) {
                m_regex_index++;
                return true;
            }
            break;
        case KEY_UP:
            if (m_index > 0) {
                m_index--;
                return true;
            }
            break;
        case KEY_HOME:
            if (m_regex_index != 0) {
                m_regex_index = 0;
                return true;
            }
            break;
        case KEY_DOWN:
            if (m_index < m_files.size()) {
                m_index++;
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
            if (m_index < m_files.size() && !m_files.empty()) {
                std::string path = *std::next(m_files.begin(), m_index);
                if (m_selected_files.count(path) == 0) {
                    m_selected_files.insert(std::move(path));
                } else {
                    m_selected_files.erase(std::move(path));
                }
                return true;
            }
        case ctrl('a'):
            m_selected_files.insert(m_files.begin(),m_files.end());
            return true;
        case ctrl('r'):
            m_selected_files.clear();
            return true;
        case ctrl('i'):
            new_selected_files.insert(m_files.begin(),m_files.end());
            for (const auto & item : m_selected_files)
                new_selected_files.erase(item);
            m_selected_files = std::set<fs::path>(new_selected_files);
            return true;
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
    if (m_index >= m_files.size()) m_index = m_files.size();
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
