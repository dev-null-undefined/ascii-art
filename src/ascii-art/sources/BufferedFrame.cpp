#include "BufferedFrame.h"

std::shared_ptr<Frame> BufferedFrame::clone() const {
    return std::shared_ptr<Frame>(new BufferedFrame(*this));
}

void BufferedFrame::GetSize() {
    m_size = {m_pixels.cols(), m_pixels.rows()};
}

Color BufferedFrame::GetPixel(Vector vector) {
    return m_pixels.at(vector);
}

void BufferedFrame::SetPixel(Vector vector, Color color) {
    m_pixels.at(vector) = color;
}

BufferedFrame::BufferedFrame(Matrix<Color> pixels) : m_pixels(std::move(pixels)) {

}

Matrix<Color> BufferedFrame::getPixels() const {
    return Matrix<Color>(m_pixels);
}
