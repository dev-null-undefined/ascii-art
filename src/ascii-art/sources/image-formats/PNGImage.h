#ifndef ASCII_ART_PNGIMAGE_H_49F706B05EC3464D9BAFC737C0CA415F
#define ASCII_ART_PNGIMAGE_H_49F706B05EC3464D9BAFC737C0CA415F


#include "../Image.h"
#include <string>

class PNGImage : public Image{

  public:
    explicit PNGImage(std::string filename);
    ~PNGImage() override = default;

  protected:
    std::string m_filename;
    void Load() override;

  public:
    std::string_view filename() const override;
};


#endif //ASCII_ART_PNGIMAGE_H_49F706B05EC3464D9BAFC737C0CA415F
