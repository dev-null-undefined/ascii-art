#include "Frame.h"
#include <stdexcept>

Vector Frame::getSize() {
    if (!m_size) GetSize();
#ifdef DEBUG
    if(!m_size) throw std::runtime_error("Frame::GetSize() failed to get size and set it");
#endif
    return m_size.value();

}

bool Frame::validPosition(Vector position) {
    getSize();
    return position.m_x < m_size->m_x && position.m_y < m_size->m_y;
}

Color Frame::getPixel(Vector position) {

#ifdef DEBUG
    if(!validPosition(position)) throw std::invalid_argument("Position out of bounds");
#endif
    return GetPixel(position);
}

void Frame::setPixel(Vector position, Color color) {

#ifdef DEBUG
    if(!validPosition(position)) throw std::invalid_argument("Position out of bounds");
#endif
    SetPixel(position, color);
}

