#ifndef BANDMATDEF
#define BANDMATDEF
// **********************************************************************
#include <array>
#include "base/matrix.hpp"

namespace solvant {
namespace base {
template <typename T, std::size_t R, std::size_t B>
class BMatrix : public Matrix<T, R, R> {
protected:
    std::array<T, R * B> m_data;

public:
    BMatrix();
    BMatrix(std::array<T, B>&& diagonal_constants);
    virtual ~BMatrix(){};

    void solve(){};
    // wraps protected at()
    T at(std::array<int, 2> ij) {
        if (ij[0] >= R || ij[1] >= R || ij[0] < 0 || ij[1] < 0) {
            throw std::out_of_range("index out of bounds");
        }
        if (std::abs(ij[0] - ij[1]) > (B >> 1)) {
            return 0;
        }
        return at(ij[0], ij[1]);
    }
    void set(std::array<int, 2> ij, const T val) {
        if (ij[0] >= R || ij[1] >= R || ij[0] < 0 || ij[1] < 0) {
            throw std::out_of_range("index out of bounds");
        }
        if (std::abs(ij[0] - ij[1]) <= (B >> 1)) {
            set(ij[0], ij[1], val);
        }
    }

protected:
    // "interal" use only : avoids expensive index checking
    // This is wrapped by operator overload above
    T at(const std::size_t i, const std::size_t j);
    void set(const std::size_t i, const std::size_t j, const T val);
};

template <typename T, std::size_t R, std::size_t B>
BMatrix<T, R, B>::BMatrix() : Matrix<T, R, R>() {}

template <typename T, std::size_t R, std::size_t B>
BMatrix<T, R, B>::BMatrix(std::array<T, B>&& diagonal_constants)
    : Matrix<T, R, R>() {
    for (std::size_t i = 0; i < R; i++) {
        for (std::size_t j = 0; j < B; j++) {
            m_data[i * B + j] = diagonal_constants[j];
        }
    }
}

template <typename T, std::size_t R, std::size_t B>
T BMatrix<T, R, B>::at(const std::size_t i, std::size_t j) {
    // return m_data[i * (2 * B + 1) + (2*B+1)/2  + j - i];
    // 1 multiplicaton
    // 3 bit shifts
    // 2 additions
    return m_data[(((B >> 1) * i) << 1) + j + (B >> 1)];
}

template <typename T, std::size_t R, std::size_t B>
void BMatrix<T, R, B>::set(const std::size_t i, std::size_t j, const T val) {
    m_data[(((B >> 1) * i) << 1) + j + (B >> 1)] = val;
}
}  // namespace base
}  // namespace base
#endif