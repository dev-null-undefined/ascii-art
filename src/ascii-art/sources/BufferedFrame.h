#ifndef ASCII_ART_BUFFEREDFRAME_H_C0348B53176F421FA6CF61F07108A0AE
#define ASCII_ART_BUFFEREDFRAME_H_C0348B53176F421FA6CF61F07108A0AE


#include "Frame.h"
#include "../../container/Matrix.h"
/**
 * @brief loaded pixel frame in memory.
 */
class BufferedFrame : public Frame {
  public:
    explicit BufferedFrame(Matrix<Color> pixels);

    ~BufferedFrame() override = default;

    std::shared_ptr<Frame> clone() const override;


    Matrix<Color> getPixels() const override;

  protected:
    Matrix<Color> m_pixels;

    void GetSize() override;

    Color GetPixel(Vector vector) override;

    void SetPixel(Vector vector, Color color) override;

};


#endif //ASCII_ART_BUFFEREDFRAME_H_C0348B53176F421FA6CF61F07108A0AE
