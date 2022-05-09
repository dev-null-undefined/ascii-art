#ifndef ASCII_ART_FRAME_H_6763F504C1EB4C1C99CDFBB025A0C3A2
#define ASCII_ART_FRAME_H_6763F504C1EB4C1C99CDFBB025A0C3A2


#include <optional>
#include <memory>
#include "../../container/Vector.h"
#include "Color.h"

class Frame {
  public:
    virtual ~Frame() = default;

    virtual std::shared_ptr<Frame> clone() const = 0;

    Vector getSize();

    bool validPosition(Vector position);

    Color getPixel(Vector position);

    void setPixel(Vector position, Color color);

  protected:
    std::optional<Vector> m_size;

    virtual void GetSize() = 0;

    virtual Color GetPixel(Vector) = 0;

    virtual void SetPixel(Vector, Color) = 0;
};


#endif //ASCII_ART_FRAME_H_6763F504C1EB4C1C99CDFBB025A0C3A2
