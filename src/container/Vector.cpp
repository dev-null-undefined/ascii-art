#include <stdexcept>
#include "Vector.h"

Vector & Vector::operator+=(const Vector & other) {
    m_y += other.m_y;
    m_x += other.m_x;
    return *this;
}


Vector Vector::operator*(size_t factor) const {
    return Vector(*this) *= factor;
}

Vector & Vector::operator*=(size_t factor) {
    m_x *= factor;
    m_y *= factor;
    return *this;
}

Vector Vector::operator-() const {
    return {-m_x, -m_y};
}

Vector & Vector::operator-=(const Vector & other) {
    m_y -= other.m_y;
    m_x -= other.m_x;
    return *this;
}

Vector Vector::operator+(const Vector & other) const {
    return Vector(*this) += other;
}

Vector Vector::operator-(const Vector & other) const {
    return Vector(*this) -= other;
}

Vector & Vector::operator*=(double factor) {
    m_x *= factor;
    m_y *= factor;
    return *this;
}

Vector Vector::operator*(double factor) const {
    return Vector(*this) *= factor;
}

inline size_t size_t_cast(double number) {
    return static_cast<size_t>(number);
}

inline double double_cast(size_t number) {
    return static_cast<double>(number);
}

Vector Vector::resizeWithAspectRation(Vector maximum, Vector original, double aspect_multiplier) {
    if (aspect_multiplier < MINIMUM_SCALE_FACTOR || aspect_multiplier > MAXIMUM_SCALE_FACTOR) {
        throw std::invalid_argument(
                "Scale multiplier must be between " + std::to_string(MINIMUM_SCALE_FACTOR) + " and " +
                std::to_string(MAXIMUM_SCALE_FACTOR));
    }
    double matrix_scale = double_cast(original.m_x) / double_cast(original.m_y) * aspect_multiplier;

    double desired_scale = double_cast(maximum.m_x) / double_cast(maximum.m_y);

    if (desired_scale > matrix_scale) {
        maximum.m_x = size_t_cast(double_cast(maximum.m_y) * matrix_scale);
    } else {
        maximum.m_y = size_t_cast(double_cast(maximum.m_x) / matrix_scale);
    }

    return {maximum.m_x, maximum.m_y};
}

bool Vector::operator==(const Vector & rhs) const {
    return m_x == rhs.m_x &&
           m_y == rhs.m_y;
}

bool Vector::operator!=(const Vector & rhs) const {
    return !(rhs == *this);
}
