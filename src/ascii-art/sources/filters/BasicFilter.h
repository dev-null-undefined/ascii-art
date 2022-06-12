#ifndef ASCII_ART_BASICFILTER_H_D1EB76B78C9E4755BDEDA4AF355E242E
#define ASCII_ART_BASICFILTER_H_D1EB76B78C9E4755BDEDA4AF355E242E

#include "Filter.h"
#include "../Color.h"
#include "../../../container/Vector.h"
#include "../Frame.h"
/**
 * @brief Basic filter for multiplying/adding constants to Color channels.
 */
class BasicFilter : public Filter {

  public:
    BasicFilter(double red_factor=1, double green_factor=1, double blue_factor=1, double alpha_factor=1,double red_offset=0, double green_offset=0, double blue_offset=0, double alpha_offset=0);
    ~BasicFilter() override = default;

    Color apply(Color color, Vector position, Frame & frame) const override;
    Color apply(Color color, Vector position) const override;

    double m_red_factor;
    double m_green_factor;
    double m_blue_factor;
    double m_alpha_factor;

    double m_red_offset;
    double m_green_offset;
    double m_blue_offset;
    double m_alpha_offset;
};


#endif //ASCII_ART_BASICFILTER_H_D1EB76B78C9E4755BDEDA4AF355E242E
