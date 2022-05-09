#ifndef ASCII_ART_MENU_H_7C67FE55E10649288853B541EA3B9220
#define ASCII_ART_MENU_H_7C67FE55E10649288853B541EA3B9220


#include <vector>
#include <ncurses.h>
#include "sources/DataSource.h"
#include "Settings.h"

class Menu {
  public:
    explicit Menu(const std::vector<std::string> & items);
    void show() const;
    void start();

  private:
    std::vector<std::shared_ptr<DataSource>> m_sources;
    size_t m_current_index;
    size_t m_frame_index;
    Settings m_settings;
    WINDOW * m_main_window = nullptr;
    WINDOW * m_status_window = nullptr;

    static constexpr Vector MINIMUM_WINDOW_SIZE = {10,10};
    static constexpr int STATUS_WINDOW_HEIGHT = 3;

    void input_loop();

    void showStatus() const;

    void resize();

    void tryAddSource(const std::string & path, size_t depth = 0);
};


#endif //ASCII_ART_MENU_H_7C67FE55E10649288853B541EA3B9220
