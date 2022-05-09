#include <stdexcept>
#include "DataSource.h"

void DataSource::applyFilter(const Filter & filter) {
    size_t frames = frameCount();
    for (size_t i = 0; i < frames; ++i){
        Frame & frame = getFrame(i);
        Vector frameSize = frame.getSize();
        for (size_t j = 0; j < frameSize.m_y; ++j){
            for (size_t k = 0; k < frameSize.m_x; ++k){
                Vector pos{k, j};
                frame.setPixel(pos, filter.apply(frame.getPixel(pos),pos,frame));
            }
        }
    }
}

Frame & DataSource::getFrame(size_t index) {
    size_t frame_count = frameCount();
    if(index < 0 || index >= frame_count) {
        throw std::out_of_range("getFrame: index out of range "+std::to_string(index)+"/"+std::to_string(frame_count));
    }
    return GetFrame(index);
}
