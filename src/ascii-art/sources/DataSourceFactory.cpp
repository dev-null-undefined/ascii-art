#include <stdexcept>
#include "DataSourceFactory.h"
#include "image-formats/JPEGImage.h"
#include "image-formats/PNGImage.h"

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::shared_ptr<DataSource> DataSourceFactory::getDataSource(const std::string & filename) {
    if(ends_with(filename,".jpg") || ends_with(filename,".jpeg")) {
        return std::make_shared<JPEGImage>(filename);
    } else if(ends_with(filename,".png")) {
        return std::make_shared<PNGImage>(filename);
    } else {
        throw std::invalid_argument("Unknown image format: " + filename);
    }
}
