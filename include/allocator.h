#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "alloc.h"

namespace istl
{
    template<typename T>
    class allocator{
        public:
            typedef T            value_type;
            typedef T*           pointer;
            typedef const T*     const_pointer;
            typedef T&           reference;
            typedef const T&     const_reference;
        
        public:
            static T *allocate();
            static T *allocate(size_t n);
            static void deallocate(T *ptr);
            static void deallocate(T *ptr, size_t n);

            static void construct(T *ptr);
            static void construct(T *ptr, const T& value);
            static void destroy(T *ptr);
            static void destroy(T *first, T *last);
    };

    template<typename T>
    T *allocator<T>::allocate(){
        return static_cast<T *>(alloc::allocate(sizeof(T)));
    }

    template<typename T>
    T *allocator<T>::allocate(size_t n){
        if(n==0) return 0;
        return static_cast<T *>(alloc::allocate(sizeof(T)*n));
    }

    template<typename T>
    void allocator<T>::deallocate(T *ptr){
        alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
    }

    template<typename T>
    void allocator<T>::deallocate(T *ptr, size_t n){
        alloc::deallocate(static_cast<void *>(ptr), sizeof(T)*n);
    }

    template<typename T>
    void allocator<T>::construct(T *ptr){
        new(ptr)T();
    }

    template<typename T>
    void allocator<T>::construct(T *ptr, const T& value){
        new(ptr)T(value);
    }

    template<typename T>
    void allocator<T>::destroy(T *ptr){
        ptr->~T();
    }

    template<typename T>
    void allocator<T>::destroy(T *begin, T *end){
        for(T *ptr = begin; ptr!=end; ++ptr){
            ptr->~T();
        }
    }
} // namespace istl


#endif