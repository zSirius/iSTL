#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstddef>
#include <memory>

namespace istl
{
    template<typename T>
    class Vector
    {
    public:
        typedef T value_type;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        Vector() : data_(nullptr), size_(0), capacity_(0) {}
        explicit Vector(size_type n) : data_(new T[n]), size_(n), capacity_(n) {}
        ~Vector() { delete[] data_; }

        size_type size() const { return size_; }
        iterator begin() { return data_; }
        const_iterator begin() const { return data_; }
        iterator end() { return data_ + size_; }
        const_iterator end() const { return data_ + size_; }

        T& operator[](size_type i) { return data_[i]; }
        const T& operator[](size_type i) const { return data_[i]; }

    private:
        T* data_;
        size_type size_;
        size_type capacity_;
    };

} // namespace istl

#endif // _VECTOR_H_