#ifndef ASCII_ART_VECTOR_H_D13310CEAEA74CF888F78AA5F355AAD8
#define ASCII_ART_VECTOR_H_D13310CEAEA74CF888F78AA5F355AAD8

#include <cstddef>

/**
 * @brief A vector class.
 * Represents 2D mathematical vector.
 */
struct Vector {
    size_t m_x{};
    size_t m_y{};

    Vector & operator+=(const Vector & other);

    Vector operator*(size_t factor) const;

    Vector & operator*=(size_t factor);

    Vector operator*(double factor) const;

    Vector & operator*=(double factor);

    Vector operator-() const;

    Vector & operator-=(const Vector & other);

    Vector operator+(const Vector & other) const;

    Vector operator-(const Vector & other) const;

    bool operator==(const Vector & rhs) const;

    bool operator!=(const Vector & rhs) const;

    constexpr Vector(size_t m_x = 0, size_t m_y = 0) : m_x(m_x), m_y(m_y) {}

    static constexpr double MINIMUM_SCALE_FACTOR = 0.2; //!< Minimum aspectMultiplier for the Vector::resizeWithAspectRation method
    static constexpr double MAXIMUM_SCALE_FACTOR = 4; //!< Maximum aspectMultiplier for the Vector::resizeWithAspectRation method

    /**
     * @brief Calculate maximum Vector size that would fit into the _maximum_ Vector param.
     * Calculate maximum Vector while keeping the same sides ratios as the _original_ Vector multiplied by the _aspectMultiplier_.
     * @param maximum Maximum size for the new Vector possible.
     * @param original Vector to resize.
     * @param aspectMultiplier Aspect ratio multiplier to multiply the ratio by.
     * @return Resized Vector not bigger than the _maximum_ with same ratio as _original_.
     */
    static Vector resizeWithAspectRation(Vector maximum, Vector original, double aspectMultiplier = 1.0);
};


#endif //ASCII_ART_VECTOR_H_D13310CEAEA74CF888F78AA5F355AAD8
