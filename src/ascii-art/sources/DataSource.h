#ifndef ASCII_ART_DATASOURCE_H_ED81DB7392E44697809728F876932C09
#define ASCII_ART_DATASOURCE_H_ED81DB7392E44697809728F876932C09


#include "Frame.h"
#include "SourceType.h"
#include "Filter.h"
#include <string>


class DataSource {

public:
    virtual ~DataSource() = default;
    virtual size_t frameCount() const = 0;
    virtual std::string_view filename() const = 0;
    Frame & getFrame(size_t index);
    virtual SourceType type() const = 0;
    void applyFilter(const Filter & filter);
  protected:
    virtual Frame & GetFrame(size_t index) = 0;
};


#endif //ASCII_ART_DATASOURCE_H_ED81DB7392E44697809728F876932C09
