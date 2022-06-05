#ifndef ASCII_ART_FRAME_H_6763F504C1EB4C1C99CDFBB025A0C3A2
#define ASCII_ART_FRAME_H_6763F504C1EB4C1C99CDFBB025A0C3A2


#include <optional>
#include <memory>
#include "Color.h"
#include "../../container/Vector.h"
#include "../../container/Matrix.h"

/**
 * Frame representing a 2D image.
 */
class Frame {
  public:
    virtual ~Frame() = default;

    virtual std::shared_ptr<Frame> clone() const = 0;

    virtual Matrix<Color> getPixels() const = 0;

    /**
     * @brief Returns the size of the frame.
     * If cached returns the cached size, otherwise computes the size using Frame::GetSize() method and caches it.
     * @return size of the frame.
     */
    Vector getSize();

    /**
     * Checks if position is inside the frame.
     * @param position Vector position to check.
     * @return
     */
    bool validPosition(Vector position);

    Color getPixel(Vector position);

    void setPixel(Vector position, Color color);

  protected:
    /**
     * Computes the size of the frame.
     * and caches it.
     */
    virtual void GetSize() = 0;

    virtual Color GetPixel(Vector) = 0;

    virtual void SetPixel(Vector, Color) = 0;

    /**
     * Cached size of the frame.
     */
    std::optional<Vector> m_size;
};


#endif //ASCII_ART_FRAME_H_6763F504C1EB4C1C99CDFBB025A0C3A2
