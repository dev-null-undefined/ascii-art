#ifndef ASCII_ART_MATRIX_H_9B32B0C3FC5045D9BCA62F147A05BEEA
#define ASCII_ART_MATRIX_H_9B32B0C3FC5045D9BCA62F147A05BEEA

#include <vector>
#include <cstdlib>
#include <stdexcept>

template<typename T>
class Matrix {
  public:
    typedef size_t number;

    Matrix(number rows, number cols);

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
    return *this;
}


template<typename T>
T & Matrix<T>::at(number col, number row) {
    if (row < 0 || row >= m_height || col < 0 || col >= m_width) {
        throw std::out_of_range("Matrix index out of range");
    }
    return m_data[row * m_width + col];
}

template<typename T>
const T & Matrix<T>::at(number col, number row) const {
    if (row < 0 || row >= m_height || col < 0 || col >= m_width) {
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


#endif //ASCII_ART_MATRIX_H_9B32B0C3FC5045D9BCA62F147A05BEEA
