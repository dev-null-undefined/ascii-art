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
