#include <stdexcept>
#include "DataSourceFactory.h"
#include "image-formats/JPEGImage.h"
#include "image-formats/PNGImage.h"

std::shared_ptr<DataSource> DataSourceFactory::getDataSource(const std::string & filename) {
    if(filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos) {
        return std::make_shared<JPEGImage>(filename);
    } else if(filename.find(".png") != std::string::npos) {
        return std::make_shared<PNGImage>(filename);
    } else {
        throw std::invalid_argument("Unknown image format");
    }
}
