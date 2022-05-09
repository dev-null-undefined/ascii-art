#include "Image.h"
#include <stdexcept>
size_t Image::frameCount() const {
    return 1;
}

Frame & Image::GetFrame(size_t) {
    if(!m_frame) Load();
#ifdef DEBUG
    if(!m_frame) {
        throw std::runtime_error("Image::getFrame: frame not loaded");
    }
#endif
    return *(*m_frame); // double dereference first for optional second for shared_ptr
}

SourceType Image::type() const {
    return SourceType::IMAGE;
}