#ifndef ASCII_ART_VIDEO_H_1AF628C987B2432397ED8B77404CC7EE
#define ASCII_ART_VIDEO_H_1AF628C987B2432397ED8B77404CC7EE


#include "DataSource.h"

class Video: public DataSource {
  public:
    ~Video() override = default;

    SourceType type() const override;

};


#endif //ASCII_ART_VIDEO_H_1AF628C987B2432397ED8B77404CC7EE
