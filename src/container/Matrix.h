#ifndef ASCII_ART_MATRIX_H_9B32B0C3FC5045D9BCA62F147A05BEEA
#define ASCII_ART_MATRIX_H_9B32B0C3FC5045D9BCA62F147A05BEEA

#include <vector>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include "Vector.h"

template<typename T>
class Matrix {
  public:
    typedef size_t number;

    Matrix(number rows, number cols);

    Matrix(Vector size);

    Matrix(const Matrix & other);

    Matrix(Matrix && other) noexcept;

    Matrix & operator=(const Matrix & other);

    Matrix & operator=(Matrix && other) noexcept;

    ~Matrix() = default;

    number rows() const;

    number cols() const;

    T & at(number row, number col);

    const T & at(number row, number col) const;

    T & at(Vector position);

    const T & at(Vector position) const;

    Matrix<T> resize(Vector new_size, double scale_multiplier = 1.0) const;

  private:
    std::vector<T> m_data;
    number m_width;
    number m_height;
};

template<typename T>
Matrix<T>::Matrix(number rows, number cols) {
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    m_width = cols;
    m_height = rows;
    m_data.resize(rows * cols);
}

template<typename T>
Matrix<T>::Matrix(Vector size) {
    if (size.m_y == 0 || size.m_x == 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
    m_width = size.m_x;
    m_height = size.m_y;
    m_data.resize(size.m_x * size.m_y);
}

template<typename T>
Matrix<T>::Matrix(const Matrix & other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_data = other.m_data;
}

template<typename T>
Matrix<T>::Matrix(Matrix && other) noexcept {
    m_width = other.m_width;
    m_height = other.m_height;
    m_data = std::move(other.m_data);
    other.m_width = 0;
    other.m_height = 0;
}

template<typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix & other) {
    if (this == &other)return *this;
    m_width = other.m_width;
    m_height = other.m_height;
    m_data = other.m_data;
    return *this;
}

template<typename T>
Matrix<T> & Matrix<T>::operator=(Matrix && other) noexcept {
    if (this == &other)return *this;
    m_width = other.m_width;
    m_height = other.m_height;
    m_data = std::move(other.m_data);
    other.m_width = 0;
    other.m_height = 0;
    return *this;
}


template<typename T>
T & Matrix<T>::at(number col, number row) {
    if (row >= m_height || col >= m_width) {
        throw std::out_of_range("Matrix index out of range");
    }
    return m_data[row * m_width + col];
}

template<typename T>
const T & Matrix<T>::at(number col, number row) const {
    if (row >= m_height || col >= m_width) {
        throw std::out_of_range("Matrix index out of range");
    }
    return m_data[row * m_width + col];
}

template<typename T>
typename Matrix<T>::number Matrix<T>::rows() const {
    return m_height;
}

template<typename T>
typename Matrix<T>::number Matrix<T>::cols() const {
    return m_width;
}

template<typename T>
T & Matrix<T>::at(Vector position) {
    return at(position.m_x, position.m_y);
}

template<typename T>
const T & Matrix<T>::at(Vector position) const {
    return at(position.m_x, position.m_y);
}

inline double double_cast(size_t number) {
    return static_cast<double>(number);
}

template<typename T>
Matrix<T> Matrix<T>::resize(Vector new_size, double scale_multiplier) const {

    new_size = Vector::resizeWithAspectRation(new_size, {m_width, m_height}, scale_multiplier);

    Matrix<T> newMatrix(new_size);
    double realScale = static_cast<double>(new_size.m_x) / static_cast<double>(m_width);
    if (realScale <= 1) {
        double box_size = 1 / realScale;
        double box_rounded = floor(box_size);
        double remainder = box_size - static_cast<double>(box_rounded);
        for (size_t x = 0; x < new_size.m_x; x++) {
            for (size_t y = 0; y < new_size.m_y; y++) {
                double dx = double_cast(x);
                double dy = double_cast(y) * scale_multiplier;
                T value{};
                double count = 0;
                for (size_t j = 0; double_cast(j) < box_rounded && double_cast(j) + dx * box_size < m_width; ++j) {
                    for (size_t k = 0; double_cast(k) < box_rounded && double_cast(k) + dy * box_size < m_height; ++k) {
                        value += at(static_cast<size_t>(dx * box_size + double_cast(j)),
                                    static_cast<size_t>(dy * box_size + double_cast(k)));
                        count++;
                    }
                    if (box_rounded + dy * box_size < m_height) {
                        value += at(static_cast<size_t>(dx * box_size + double_cast(j)),
                                    static_cast<size_t>(dy * box_size + box_rounded)) * remainder;
                        count += remainder;
                    }
                }
                for (size_t j = 0; double_cast(j) < box_rounded && double_cast(j) + dy * box_size < m_height &&
                                   dx * box_size + box_rounded < m_width; ++j) {
                    value += at(static_cast<size_t>(dx * box_size + box_rounded),
                                static_cast<size_t>(dy * box_size + double_cast(j))) * remainder;
                    count += remainder;
                }
                if (count > 0) {
                    value = value * static_cast<double>(1 / static_cast<double>(count));
                }
                newMatrix.at(x, y) = value;
            }
        }
    } else {
        for (size_t x = 0; x < new_size.m_x; x++) {
            for (size_t y = 0; y < new_size.m_y; y++) {
                double dx = double_cast(x);
                double dy = double_cast(y) * scale_multiplier;
                T value = at(static_cast<size_t>(dx / realScale), static_cast<size_t>(dy / realScale));
                newMatrix.at(x, y) = value;
            }
        }
    }
    return newMatrix;
}

#endif //ASCII_ART_MATRIX_H_9B32B0C3FC5045D9BCA62F147A05BEEA
