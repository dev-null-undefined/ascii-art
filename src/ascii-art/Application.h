#ifndef ASCII_ART_APPLICATION_H_96E1EAAEA1344C3C87CBFEF534286D2B
#define ASCII_ART_APPLICATION_H_96E1EAAEA1344C3C87CBFEF534286D2B


#include <vector>
#include <ncurses.h>
#include <chrono>
#include "sources/DataSource.h"
#include "Settings.h"
#include "menus/Menu.h"

// TODO: instance counter to prevent multiple application from running (delete copy constructor as well)
/**
 * @brief The Application class controls ncurses and menus.
 * @details Application is responsible for starting and stopping ncurses, and handling basic input keys.
 *         It also handles argument parsing and data source loading.
 *         And manages menus and their display.
 *         It is not multi-threaded, but it does simulate a multi-threaded like environment using timeout on blocking ncurses calls.
 */
class Application {
  public:
    /**
     * @brief Application constructor.
     * @details Initializes ncurses and creates a menu.
     * if arguments are provided, it will try to load the data source from the given file.
     * or if argument starts with -, it will try to parse it as a command line argument. Used for changing settings.
     * if you want to disable argument parsing, use -- that will disable argument parsing. for all following argument.
     * @param args Arguments passed to the main function of the program.
     */
    explicit Application(const std::vector<std::string> & args);

    /**
     * @brief Initializes ncurses and creates starts input loop.
     * initializes ncurses and displays the menu. if colors are enabled, it will also initialize color pairs.
     */
    void start();

  private:
    /**
     * Source files to be used for Gallery menu.
     */
    std::shared_ptr<std::vector<std::shared_ptr<DataSource>>> m_sources;
    /**
     * Current menu being displayed and used for input handling.
     */
    std::shared_ptr<Menu> m_current_menu;

    /**
     * Settings for the application. Can be changed by command line arguments.
     */
    std::shared_ptr<Settings> m_settings;
    /**
     * Regex save from previous usage of FileMenu.
     */
    std::optional<std::string> m_regex_save;
    /**
     * Last time resize was called.
     * Used to prevent resize from being called too often.
     */
    std::optional<std::chrono::steady_clock::time_point> m_last_resize;

    /**
     * Input handling loop.
     * Handles input and calls the current menu's Menu::input(int input, bool & handled) function.
     * based on the result calls Menu::update() function to re-render the menu.
     */
    void input_loop();

    /**
     * @brief Recursively adds sources from given path and all subdirectories.
     * @details traverses given path and all subdirectories and adds all files that are supported by DataSourceFactory.
     * this function doesn't test if the file is a valid source file. Only if the file is supported by DataSourceFactory.
     * @param path base path to start traversing.
     * @param depth current depth of the recursion.
     */
    void tryAddSource(const std::string & path, size_t depth = 0);

    /**
     * Returns size of the terminal.
     * @return size of the terminal.
     */
    static Vector getResolution();
};

#if NCURSES_EXT_FUNCS >= 20181013
#define NCURSES_WIDE_COLOR_SUPPORT 1

/**
 * Round color to the nearest available color.
 * @param color color to round.
 * @return color pair index for closest color to given color.
 */
int getRoundedColorIndex(Color color);

#endif

#endif //ASCII_ART_APPLICATION_H_96E1EAAEA1344C3C87CBFEF534286D2B
