#ifndef ASCII_ART_FILTER_H_83871D30DE3040108B811AC25379E177
#define ASCII_ART_FILTER_H_83871D30DE3040108B811AC25379E177


#include "../Frame.h"
#include "../../../container/Vector.h"
#include "../Color.h"

class Filter {
  public:
    virtual ~Filter() = default;

    /**
     * @brief Apply filter to color.
     * Abstract class method for applying any kind of filter to pixel map. (based on position,color,etc.)
     * @param color Color on _position_ to apply filter to.
     * @param position Position of the _color_.
     * @param frame Whole frame that the _color_ came from in case of complicated filters for edge detection etc.
     * @return new Color to be placed instead of the original one.
     */
    virtual Color apply(Color color, Vector position, Frame & frame) const = 0;

    /**
     * @brief Apply filter to color.
     * @param color Color on _position_ to apply filter to.
     * @param position Position of the _color_.
     * @return new Color after filter applied.
     */
    virtual Color apply(Color color, Vector position) const = 0;
};


#endif //ASCII_ART_FILTER_H_83871D30DE3040108B811AC25379E177
