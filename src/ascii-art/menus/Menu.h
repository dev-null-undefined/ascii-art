#ifndef ASCII_ART_MENU_H_C5098B7A070F4C7DA3A521D26B32DDFF
#define ASCII_ART_MENU_H_C5098B7A070F4C7DA3A521D26B32DDFF


#include "../../container/Vector.h"

class Menu {
public:
    virtual ~Menu() = default;

    virtual void show(Vector initial_size) = 0;
    virtual void resize(Vector size) = 0;
    virtual void hide() = 0;
    virtual void update() const = 0;
    virtual bool input(int input) = 0;

};

#endif //ASCII_ART_MENU_H_C5098B7A070F4C7DA3A521D26B32DDFF
