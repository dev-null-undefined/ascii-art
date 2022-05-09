#ifndef ASCII_ART_IMAGE_H_527B3A09FA9B46B5BD0D8B45BAD22980
#define ASCII_ART_IMAGE_H_527B3A09FA9B46B5BD0D8B45BAD22980

#include "DataSource.h"

class Image : public DataSource {
  public:
    ~Image() override = default;

    size_t frameCount() const override;

    SourceType type() const override;
  protected:
    Frame & GetFrame(size_t index) override;
    virtual void Load() = 0;

    std::optional<std::shared_ptr<Frame>> m_frame;
};


#endif //ASCII_ART_IMAGE_H_527B3A09FA9B46B5BD0D8B45BAD22980
