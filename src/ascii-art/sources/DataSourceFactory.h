#ifndef ASCII_ART_DATASOURCEFACTORY_H_6C8A155CAF7743F493B993B3DEAEB08F
#define ASCII_ART_DATASOURCEFACTORY_H_6C8A155CAF7743F493B993B3DEAEB08F


#include <memory>
#include "DataSource.h"

class DataSourceFactory {

  public:
    /**
     * @brief Returns shared_ptr to DataSource based on filename extension.
     * This method doesn't check if file is actually valid and can be loaded etc.
     * @param filename file path to get DataSource from.
     * @return DataSource based on the filename extension.
     */
    static std::shared_ptr<DataSource> getDataSource(const std::string & filename);
};


#endif //ASCII_ART_DATASOURCEFACTORY_H_6C8A155CAF7743F493B993B3DEAEB08F
