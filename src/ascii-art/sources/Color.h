#ifndef ASCII_ART_COLOR_H_39B874B790A248EA92EF15643CDFBA78
#define ASCII_ART_COLOR_H_39B874B790A248EA92EF15643CDFBA78


class Color {
  public:
    typedef int number;

    explicit Color(number m_r = 0, number m_g = 0, number m_b = 0, number m_a = 0);

    Color & operator+=(const Color & other);

    Color operator*(number factor)const;

    Color operator*(double factor) const;

    Color & operator*=(number factor);

    Color & operator*=(double factor);

    Color operator-() const;

    Color & operator-=(const Color & other);

    Color operator+(const Color & other) const;

    Color operator-(const Color & other) const;

    number getRed() const;

    void setRed(number red);

    number getGreen() const;

    void setGreen(number green);

    number getBlue() const;

    void setBlue(number blue);

    number getAlpha() const;

    void setAlpha(number alpha);

    /**
     * Normalize each part of color to the max/min 0-255 values
     * @return new normalized color.
     */
    Color normalize() const;

  private:
    number m_r, m_g, m_b, m_a;

};


#endif //ASCII_ART_COLOR_H_39B874B790A248EA92EF15643CDFBA78
