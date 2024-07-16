#ifndef _ISTL_VECTOR_IMPL_H_
#define _ISTL_VECTOR_IMPL_H_
#include "vector.h"
#include "uninitialized.h"
#include <algorithm>
#include <utility>

namespace istl
{
    /* 构造、析构、赋值函数 */
    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const size_type n){
        allocateAndFillN(n, value_type());
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const size_type n, const value_type &value){
        allocateAndFillN(n, value);
    }

    template<typename T, typename Alloc>
    template<typename InputIterator, typename>
    vector<T, Alloc>::vector(InputIterator first, InputIterator last){
        allocateAndCopy(first, last);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector& v){
        allocateAndCopy(v._start, v._finish);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(vector&& v){
        _start = v._start;
        _finish = v._finish;
        _endOfStorage = v._endOfStorage;
        v._start = v._finish = v._endOfStorage = nullptr;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (const vector &v){
        if(this != &v){
            destroyAndDeallocateAll();
            allocateAndCopy(v._start, v._finish);
        }
        return *this;
    }
    template<typename T, typename Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator = (vector &&v){
        if (this != &v){
            destroyAndDeallocateAll();
            _start = v._start;
            _finish = v._finish;
            _endOfStorage = v._endOfStorage;
            v._start = v._finish = v._endOfStorage = nullptr;
        }
        return *this;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::~vector(){
        destroyAndDeallocateAll();
    }

    /* 容量相关函数 */
    template<typename T, typename Alloc>
    void vector<T, Alloc>::reserve(size_type n){
        if(capacity() >= n) return;
        T *newstart = dataAllocator::allocate(n);
        T *newfinish = istl::uninitialized_move(begin(), end(), newstart);
        destroyAndDeallocateAll();
        _start = newstart;
        _finish = newfinish;
        _endOfStorage = _start + n;
    }
    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type n, const value_type& val){
        if(n<size()){
            dataAllocator::destroy(_start + n, _finish);
            _finish = _start + n;
        }else if(n > size() && n <= capacity()){
            auto len = n - size();
            _finish = istl::uninitialized_fill_n(_finish, len, val);
        }else if(n > capacity()){
            auto len = n - size();
            T *newstart = dataAllocator::allocate(getNewCapacity(len));
            T *newfinish = istl::uninitialized_copy(begin(), end(), newstart);
            newfinish = istl::uninitialized_fill_n(newfinish, len, val);

            destroyAndDeallocateAll();
            _start = newstart;
            _finish = newfinish;
            _endOfStorage = _start + n;
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::shrink_to_fit(){
        T *newstart = dataAllocator::allocate(size());
        T *newfinish = istl::uninitialized_move(_start, _finish, newstart);
        destroyAndDeallocateAll();
        _start = newstart;
        _endOfStorage = _finish = newfinish;
    }

    /* 增删改 */
    template<typename T, typename Alloc>
    void vector<T, Alloc>::clear(){
        dataAllocator::destroy(_start, _finish);
        _finish = _start;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::swap(vector &v){
        std::swap(_start, v._start);
        std::swap(_finish, v._finish);
        std::swap(_endOfStorage, v._endOfStorage);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator pos){
        return erase(pos, pos+1);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator first, iterator last){
        difference_type lenOfTail = end() - last;
        difference_type lenOfRemoved = last - first;
        iterator dst = first;
        iterator src = last;
        dataAllocator::destroy(first, last);
        for(difference_type i=0; i<lenOfTail; ++i){
            if(i<lenOfRemoved){
                dataAllocator::construct(dst, std::move(*src));
            }else{
                *dst  = std::move(*src);
            }
            ++dst, ++src;
        }
        _finish -= lenOfRemoved;
        return first;
    }


    template<typename T, typename Alloc>
    void vector<T, Alloc>::pop_back(){
        --_finish;
        dataAllocator::destroy(_finish);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const value_type& val){
        insert(end(), val);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert(const_iterator pos, const value_type& value){
        return insert(pos, (size_type)1, value);
    }


    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert(const_iterator pos, size_type count, const value_type& value){
        difference_type idx = pos - begin();
        iterator vpos = begin() + idx;
        if(count == 0) return vpos;
        difference_type SpaceLeft = _endOfStorage - _finish;
        difference_type SpaceNeed = count;
        if(SpaceLeft >= SpaceNeed){
            auto ptr = end() - 1;
            for(; ptr >= pos; --ptr){
                dataAllocator::construct(vpos+SpaceNeed, std::move(*ptr));
            }
            istl::uninitialized_fill_n(vpos, count, value);
            _finish += count;
        }else{
            difference_type newCapacity = getNewCapacity(count);

            T *newstart = dataAllocator::allocate(newCapacity);
            T *newfinish = istl::uninitialized_move(begin(), vpos, newstart);
            newfinish = istl::uninitialized_fill_n(newfinish, count, value);
            newfinish = istl::uninitialized_move(vpos, end(), newfinish);

            destroyAndDeallocateAll();
            _start = newstart, _finish = newfinish;
            _endOfStorage = _start + newCapacity;
        }
        return begin() + idx;
    }

    template<typename T, typename Alloc>
    template<typename InputIt, typename>
    typename vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert(const_iterator pos, InputIt first, InputIt last){
        difference_type idx = pos - begin();
        iterator vpos = begin() + idx;
        if(first == last) return vpos;
        difference_type SpaceLeft = _endOfStorage - _finish;
        difference_type SpaceNeed = last - first;
        

        if(SpaceLeft >= SpaceNeed){
            if(_finish - pos > SpaceNeed){
                istl::uninitialized_copy(_finish - SpaceNeed, _finish, _finish);
                std::copy_backward(vpos, _finish-SpaceNeed, _finish);
                std::copy(first, last, vpos);
            }else{
                difference_type dif = _finish - vpos;
                iterator InsertItTail = first + dif;
                iterator tmp = istl::uninitialized_copy(InsertItTail, last, _finish);
                istl::uninitialized_copy(vpos, _finish, tmp);
                std::copy(first, InsertItTail, vpos);
            }
            _finish += SpaceNeed;
        }else{
            difference_type newCapacity = getNewCapacity(last - first);
            T *newstart = dataAllocator::allocate(newCapacity);
            T *newfinish = istl::uninitialized_move(begin(), vpos, newstart);
            newfinish = istl::uninitialized_copy(first, last, newfinish);
            newfinish = istl::uninitialized_move(vpos, end(), newfinish);

            destroyAndDeallocateAll();
            _start = newstart;
            _finish = newfinish;
            _endOfStorage = _start + newCapacity;
        }
        return begin()+idx;
    }


    template<typename T, typename Alloc>
    bool vector<T, Alloc>::operator == (const vector& v)const{
        if (size() != v.size()){
            return false;
        }
        else{
            auto ptr1 = _start;
            auto ptr2 = v._start;
            for (; ptr1 != _finish && ptr2 != v._finish; ++ptr1, ++ptr2){
                if (*ptr1 != *ptr2)
                    return false;
            }
            return true;
        }
    }
    template<typename T, typename Alloc>
    bool vector<T, Alloc>::operator != (const vector& v)const{
        return !(*this == v);
    }



    template<typename T, typename Alloc>
    void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value){
        _start = dataAllocator::allocate(n);
        istl::uninitialized_fill_n(_start, n, value);
        _finish = _endOfStorage = _start + n;
    }

    template<typename T, typename Alloc>
    template<typename InputIterator>
    void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last){
        _start = dataAllocator::allocate(last-first);
        _finish = istl::uninitialized_copy(first, last, _start);
        _endOfStorage = _finish;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::destroyAndDeallocateAll(){
        if (capacity() != 0){
            dataAllocator::destroy(_start, _finish);
            dataAllocator::deallocate(_start, capacity());
            _start = _finish = _endOfStorage = nullptr;
        }
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const{
        //可以在这里修改内存扩张策略
        size_type oldCapacity = _endOfStorage - _start;
        auto res = std::max(oldCapacity, len);
        size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);
        return newCapacity;
    }
} // namespace istl

#endif
