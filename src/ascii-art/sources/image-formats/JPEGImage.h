#ifndef ASCII_ART_JPEGIMAGE_H_81CA250E46474F5386FA07E2FEA3CB1E
#define ASCII_ART_JPEGIMAGE_H_81CA250E46474F5386FA07E2FEA3CB1E

#include "../Image.h"
#include <string>

class JPEGImage : public Image {
    std::string m_filename;
  public:
    explicit JPEGImage(std::string filename);
    ~JPEGImage() override = default;

  protected:
    void Load() override;

  public:
    std::string_view filename() const override;
};

#endif //ASCII_ART_JPEGIMAGE_H_81CA250E46474F5386FA07E2FEA3CB1E
