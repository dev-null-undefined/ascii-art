#ifndef ASCII_ART_MENU_H_C5098B7A070F4C7DA3A521D26B32DDFF
#define ASCII_ART_MENU_H_C5098B7A070F4C7DA3A521D26B32DDFF


#include "../../container/Vector.h"
/**
 * @brief Virtual class representing single ncurses scene/menu.
 */
class Menu {
  public:
    virtual ~Menu() = default;

    /**
     * Show method used to initialize ncurses windows and private fields of the class.
     * @param initial_size Initial maximum screen size.
     */
    virtual void show(Vector initial_size) = 0;

    /**
     * Resize method invoked on terminal resize.
     * @param size new maximum terminal size.
     */
    virtual void resize(Vector size) = 0;

    /**
     * Clear all ncurses windows and private fields.
     */
    virtual void hide() = 0; // TODO: add assert if hidden/shown twice in row
    /**
     * Render new frame of scene.
     */
    virtual void update() const = 0;

    /**
     * @brief Handle key input.
     * This method handles any scene specific key shortcuts.
     * If shortcut was handled it will set the _handled_ bit to true.
     * If the scene also needs redraw after the key stroke it will return true.
     * Returning true also means the key was handled automatically.
     * @param input Key code that was pressed.
     * @param[out] handled out parameter signalizing if the key was handled by the scene.
     * @return True if update/redraw is needed, otherwise false.
     */
    virtual bool input(int input, bool & handled) = 0;

};

#endif //ASCII_ART_MENU_H_C5098B7A070F4C7DA3A521D26B32DDFF
