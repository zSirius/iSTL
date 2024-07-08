#ifndef _VECTOR_H_
#define _VECTOR_H_

#include<stddef.h>
#include<assert.h>
#include "allocator.h"
#include "ReverseIterator.h"

namespace istl
{
    /* https://en.cppreference.com/w/cpp/container/vector */
    template <typename T, class Alloc = allocator<T> >
    class vector
    {
    private:
        T* _start;
        T* _finish;
        T* _endOfStorage;

        typedef Alloc dataAllocator;
    public:
        typedef T                               value_type;
        typedef T*                              iterator;
        typedef const T* const_iterator;
        typedef reverse_iterator_t<T*>            reverse_iterator;
        typedef reverse_iterator_t<const T*>    const_reverse_iterator;
        typedef T*                              pointer;
        typedef const T*                        const_pointer;
        typedef T&                              reference;
        typedef const T&                        const_reference;
        typedef size_t                          size_type;
        typedef ptrdiff_t                       difference_type;

        //构造、析构、复制函数
        vector():_start(nullptr), _finish(nullptr), _endOfStorage(nullptr){}
        
        explicit vector(const size_type n);
        vector(const size_type n, const value_type& value);
        //vector(size_t n, const value_type& val = value_type());
        
        template<typename InputIterator, typename = typename std::enable_if<!std::is_integral<InputIterator>::value>::type>
        vector(InputIterator first, InputIterator last);
        
        vector(const vector& v);
        vector(vector&& v);
        
        vector& operator = (const vector& v);
        vector& operator = (vector&& v);

        ~vector();


        /*迭代器相关*/
        iterator begin(){ return _start; }
        const_iterator begin()const{ return _start; }
        const_iterator cbegin()const{ return _start; } 
        
        iterator end(){ return _finish; }
        const_iterator end()const{ return _finish; }
        const_iterator cend()const{ return _finish; }

        reverse_iterator rbegin(){ return reverse_iterator(_finish); }
        const_reverse_iterator crbegin()const{ return const_reverse_iterator(_finish); }
        reverse_iterator rend(){ return reverse_iterator(_start); }
        const_reverse_iterator crend()const{ return const_reverse_iterator(_start); }
        

        /* 容量相关函数 */
        difference_type size(){ return _finish-_start; }
        difference_type capacity(){ return _endOfStorage-_start; }
        bool empty(){ return _start == _finish; }
        void reserve(size_type n);
        void resize(size_type n, const value_type& val = value_type());
        void shrink_to_fit();

        /* 元素访问 */
        reference operator[](const difference_type i){ return *(begin()+i); }
        const_reference operator[](const difference_type i)const{ return *(cbegin()+i); }
        reference front(){ return *(begin()); }
        const_reference front()const{ return *(cbegin()); }
        reference back(){ return *(end()-1); }
        const_reference back()const { return *(end()-1); }
        pointer data(){ return _start; }
        const_pointer data()const{ return _start; }



        /* 增删改 */
        void clear();
        void swap(vector &v);

        void push_back(const value_type& val);
        void pop_back();

        /* https://en.cppreference.com/w/cpp/container/vector/insert */
        iterator insert(const_iterator pos, const value_type& value);
        //iterator insert(const_iterator pos, value_type&& value);
        iterator insert(const_iterator pos, size_type count, const value_type& value);
        template<typename InputIt, typename = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
        iterator insert(const_iterator pos, InputIt first, InputIt last);
        //iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);

        /* https://en.cppreference.com/w/cpp/container/vector/erase */
        iterator erase(iterator pos);
        //iterator erase(const_iterator pos);
        iterator erase(iterator first, iterator last);
        //iterator erase(const_iterator first, const_iterator last);

        
        //比较操作相关
        bool operator == (const vector& v)const;
        bool operator != (const vector& v)const;

        Alloc get_alloctor(){ return dataAllocator(); }

    private:
        void allocateAndFillN(const size_type n, const value_type& value);
        template<typename InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last);
        void destroyAndDeallocateAll();

        size_type getNewCapacity(size_type len)const;
    };
} // namespace istl

#include "vector.impl.h"

#endif