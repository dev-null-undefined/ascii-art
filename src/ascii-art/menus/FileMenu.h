#ifndef ASCII_ART_FILEMENU_H_4A383FBE43AF4A9CB8D4334DC667F489
#define ASCII_ART_FILEMENU_H_4A383FBE43AF4A9CB8D4334DC667F489

#include <ncurses.h>
#include <form.h>
#include <set>
#include "Menu.h"
#include "../sources/Color.h"

#include <filesystem>
namespace fs = std::filesystem;

class FileMenu : public Menu {
  public:
    explicit FileMenu(const std::string & m_regex = "~/Images/.*(png|jpg|jpeg)$");

    ~FileMenu() override;

    void show(Vector initial_size) override;

    void resize(Vector size) override;

    void hide() override;

    void update() const override;

    bool input(int input, bool & handled) override;

    void selectFile(const std::string_view & view);

    const std::set<fs::path> & getSelectedFiles() const;

    std::string getRegex() const;

  private:

    void update_index();

    void update_files(const std::string & regex);

    bool key_down();

    bool key_up();

    std::set<fs::path> m_selected_files;
    std::set<fs::path> m_files;
    size_t m_index = 0; //!< index of selected row from window
    size_t m_scroll = 0;

    std::string m_regex;
    size_t m_regex_index;

    WINDOW * m_window = nullptr;
    Vector m_window_size;

    static constexpr size_t MINIMAL_GAP = 5;

    static const Color SELECTED_COLOR;

    bool handle_mouse();
};


#endif //ASCII_ART_FILEMENU_H_4A383FBE43AF4A9CB8D4334DC667F489
