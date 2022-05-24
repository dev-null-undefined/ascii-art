#ifndef ASCII_ART_VECTOR_H_D13310CEAEA74CF888F78AA5F355AAD8
#define ASCII_ART_VECTOR_H_D13310CEAEA74CF888F78AA5F355AAD8

#include <cstddef>

struct Vector {
    size_t m_x{};
    size_t m_y{};
    constexpr Vector(size_t m_x = 0, size_t m_y = 0) : m_x(m_x), m_y(m_y) {}
};



#endif //ASCII_ART_VECTOR_H_D13310CEAEA74CF888F78AA5F355AAD8
