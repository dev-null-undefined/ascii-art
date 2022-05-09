#include "ImageFrame.h"

std::shared_ptr<Frame> ImageFrame::clone() const {
    return std::shared_ptr<Frame>(new ImageFrame(*this));
}

void ImageFrame::GetSize() {
    m_size = {m_pixels.cols(), m_pixels.rows()};
}

Color ImageFrame::GetPixel(Vector vector) {
    return m_pixels.at(vector);
}

void ImageFrame::SetPixel(Vector vector, Color color) {
    m_pixels.at(vector) = color;
}

ImageFrame::ImageFrame(Matrix<Color> pixels): m_pixels(std::move(pixels)) {

}
