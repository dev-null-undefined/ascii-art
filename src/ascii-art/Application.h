#ifndef ASCII_ART_APPLICATION_H_96E1EAAEA1344C3C87CBFEF534286D2B
#define ASCII_ART_APPLICATION_H_96E1EAAEA1344C3C87CBFEF534286D2B


#include <vector>
#include <ncurses.h>
#include <chrono>
#include "sources/DataSource.h"
#include "Settings.h"
#include "menus/Menu.h"

// TODO: instance counter to prevent multiple application from running (delete copy constructor as well)
class Application {
  public:
    explicit Application(const std::vector<std::string> & args);
    void start();

  private:
    std::shared_ptr<std::vector<std::shared_ptr<DataSource>>> m_sources;
    std::shared_ptr<Menu> m_current_menu;

    std::shared_ptr<Settings> m_settings;
    std::optional<std::chrono::steady_clock::time_point> m_last_resize;

    void input_loop();

    void tryAddSource(const std::string & path, size_t depth = 0);

    static Vector getResolution();
};

#if NCURSES_EXT_FUNCS >= 20181013
#define NCURSES_WIDE_COLOR_SUPPORT 1
int getRoundedColorIndex(Color color);
#endif

#endif //ASCII_ART_APPLICATION_H_96E1EAAEA1344C3C87CBFEF534286D2B
