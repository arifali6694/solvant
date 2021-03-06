#ifndef BANDMATDEF
#define BANDMATDEF
// **********************************************************************
#include <array>
namespace solvant {
namespace base {
template <typename T, std::size_t R, std::size_t B>
class banded_matrix {
protected:
    // First R entries store the lowest sub-band
    // and entires start at B and finish at R
    // ...
    // last R entries store the highest sub-band
    // etc
    std::array<T, R * B> m_data;
public:
    banded_matrix(std::array<T, B>&& diagonal_constants);
    virtual ~banded_matrix(){};

    // move assignment operator
    banded_matrix& operator=(banded_matrix&& other) noexcept {
        if(this!=&other) {
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    const T* data() const {return m_data.data(); }
    
    std::size_t bands() const { return B; }

    T at(const std::size_t i, const std::size_t j) {
        if (i >= R || j >= R || i < 0 || j < 0) {
            throw std::out_of_range("index out of bounds");
        }
        if (std::abs(long(i - j)) > (B >> 1)) {
            return 0;
        }
        return (*this)(i,j);
    }
    void set(const std::size_t i, const std::size_t j, const T val) {
        if (i >= R || j >= R || i < 0 || j < 0) {
            throw std::out_of_range("index out of bounds");
        }
        if (std::abs(long(i - j)) <= (B >> 1)) {
            (*this)(i,j) = val;
        }
    }

    // using band-major form
    T operator[](const std::size_t i) const { return m_data[i];}

    // standard (i,j) access
    T operator()(const std::size_t i, const std::size_t j) const{
        return m_data[((j - i) + (B >> 1))*R + j];
    }

    T& operator()(const std::size_t i, const std::size_t j) {
        return m_data[((j - i) + (B >> 1))*R + j];
    }
};  // namespace base

template <typename T, std::size_t R, std::size_t B>
banded_matrix<T, R, B>::banded_matrix(std::array<T, B>&& diagonal_constants) {
    for (std::size_t i = 0; i < R; i++) {
        for (std::size_t j = 0; j < B; j++) {
            m_data[j * R + i] = diagonal_constants[j];
        }
    }
}

}  // namespace solvant
}  // namespace solvant
#endif
