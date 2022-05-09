#ifndef ASCII_ART_DATASOURCEFACTORY_H_6C8A155CAF7743F493B993B3DEAEB08F
#define ASCII_ART_DATASOURCEFACTORY_H_6C8A155CAF7743F493B993B3DEAEB08F


#include <memory>
#include "DataSource.h"

class DataSourceFactory {

  public:
    static std::shared_ptr<DataSource> getDataSource(const std::string & filename);
};


#endif //ASCII_ART_DATASOURCEFACTORY_H_6C8A155CAF7743F493B993B3DEAEB08F
