#ifndef ASCII_ART_FILTER_H_83871D30DE3040108B811AC25379E177
#define ASCII_ART_FILTER_H_83871D30DE3040108B811AC25379E177


#include "../Frame.h"
#include "../../../container/Vector.h"
#include "../Color.h"

class Filter {
public:
    virtual ~Filter() = default;
    virtual Color apply(Color color, Vector position, Frame & frame) const = 0;
    virtual Color apply(Color color, Vector position) const = 0;
};


#endif //ASCII_ART_FILTER_H_83871D30DE3040108B811AC25379E177
