#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_
#include "vector.h"
#include "uninitialized.h"
#include <algorithm>

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
    template<typename InputIterator>
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
			start_ = v.start_;
			finish_ = v.finish_;
			endOfStorage_ = v.endOfStorage_;
			v.start_ = v.finish_ = v.endOfStorage_ = nullptr;
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
        if(capacity()<=n) return;
        T *newstart = dataAllocator::allocate(n);
        T *newfinish = istl::uninitialized_copy(begin(), end(), newstart);
        destroyAndDeallocateAll();
        _start = newstart;
        _finish = newfinish;
        _endOfStorage = _start + n;
    }
    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type n, const value_type& val = value_type()){
        if(n<size()){
            dataAllocator::destory(_start + n, _finish);
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
        T *newfinish = istl::uninitialized_copy(_start, _finish, newstart);
        destroyAndDeallocateAll();
        _start = newstart;
        _endOfStorage = _finish = newfinish;
    }

    /* 增删改 */
    template<typename T, typename Alloc>
    void vector<T, Alloc>::clear(){
        dataAllocator::destory(_start, _finish);
        _finish = _start;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::swap(vector &v){
        //Todo
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator pos){
        erase(pos, pos+1);
    }

    template<typename T, typename Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator first, iterator last){
        difference_type lenOfTail = end() - last;
        difference_type lenOfRemoved = last - first;
        iterator dst = first;
        iterator src = last;
        dataAllocator::destory(first, last);
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
        dataAllocator::destory(_finish);
    }


    template<typename T, typename Alloc>
    vector<T, Alloc>::iterator 
    vector<T, Alloc>::insert(const_iterator pos, size_type count, const value_type& value){
        difference_type SpaceLeft = _endOfStorage - _finish;
        difference_type SpaceNeed = n;
        if(SpaceLeft >= SpaceNeed){
            
        }
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
			dataAllocator::destroy(start_, finish_);
			dataAllocator::deallocate(start_, capacity());
            start_ = finish_ = endOfStorage_ = nullptr;
		}
	}

    template<typename T, typename Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const{
		size_type oldCapacity = endOfStorage_ - start_;
		auto res = std::max(oldCapacity, len);
		size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);
		return newCapacity;
	}
} // namespace istl

#endif
