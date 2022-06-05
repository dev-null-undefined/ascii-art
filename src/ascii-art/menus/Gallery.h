#ifndef ASCII_ART_GALLERY_H_69FBC133ED3E46619843327809288C39
#define ASCII_ART_GALLERY_H_69FBC133ED3E46619843327809288C39


#include <ncurses.h>
#include <memory>
#include <vector>
#include "Menu.h"
#include "../sources/DataSource.h"
#include "../Settings.h"

class Gallery : public Menu {
  public:
    Gallery(std::shared_ptr<std::vector<std::shared_ptr<DataSource>>> m_sources,
            std::shared_ptr<Settings> m_settings);

    ~Gallery() override;

    void show(Vector initial_size) override;

    void resize(Vector size) override;

    void hide() override;

    void update() const override;

    bool input(int input, bool & handled) override;
  private:
    std::shared_ptr<const std::vector<std::shared_ptr<DataSource>>> m_sources;
    std::shared_ptr<Settings> m_settings;

    void showStatus() const;

    size_t m_current_index;
    size_t m_frame_index;


    WINDOW * m_main_window = nullptr;
    WINDOW * m_status_window = nullptr;

    Vector m_main_window_size;

    Vector m_last_mouse_position = {0, 0};
    Vector m_image_position;
    Vector m_image_size;
    double m_image_scale = 1.0;

    static constexpr int STATUS_WINDOW_HEIGHT = 3;
    static constexpr int COLOR_FILTER_ADDITION = 10;

    static constexpr double MINIMUM_ZOOM = 0.5;
    static constexpr double MAXIMUM_ZOOM = 10.0;
    static constexpr Vector MINIMUM_WINDOW_SIZE = {10, 10};

    void zoom(int x, int y, double zoom);

    bool handle_mouse();
};


#endif //ASCII_ART_GALLERY_H_69FBC133ED3E46619843327809288C39
