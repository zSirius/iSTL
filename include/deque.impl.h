#ifndef _ISTL_DEQUE_IMPL_H_
#define _ISTL_DEQUE_IMPL_H_
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include "deque.h"
#include "allocator.h"

namespace istl
{
    namespace it
    {
        /* 赋值 */
        template<typename T>
        deque_iterator<T>& deque_iterator<T>::operator = (const deque_iterator& it){
            if(*this != it){
                _mapIndex = it._mapIndex;
                _cur = it._cur;
                _container = it._container;
            }
            return *this;
        }

        /* 运算符重载 */
        template<typename T>
        deque_iterator<T>& deque_iterator<T>::operator ++ (){
            if(_cur != getBlockTail(_mapIndex)){
                ++_cur;
            }else if(_mapIndex + 1 < _container->_mapSize){
                ++_mapIndex;
                _cur = getBlockHead(_mapIndex);
            }else{
                // reach end()
                _mapIndex = _container->_mapSize;
                _cur = (_container->_map[_mapIndex-1])+1;
            }
            return *this;
        }

        template<typename T>
        deque_iterator<T> deque_iterator<T>::operator ++ (int){
            auto res = *this;
            ++(*this);
            return res;
        }

        template<typename T>
        deque_iterator<T>& deque_iterator<T>::operator -- (){
            if(_cur != getBlockHead(_mapIndex)){
                --_cur;
            }else if(_mapIndex - 1 >= 0){
                --_mapIndex;
                _cur = getBlockTail(_mapIndex);
            }else{
                _mapIndex = 0;
                _cur = _container->_map[_mapIndex];
            }
            return *this;
        }

        template<typename T>
        deque_iterator<T> deque_iterator<T>::operator -- (int){
            auto res = *this;
            --(*this);
            return res;
        }

        template<typename T>
        typename deque_iterator<T>::difference_type
        operator - (const deque_iterator<T>& lhs, const deque_iterator<T>& rhs){
            if(lhs._container != rhs._container || lhs._container == nullptr || rhs._container == nullptr){
                return 0;
            }
            auto lhsToHead = 0;
            if(lhs._mapIndex != lhs._container->_mapSize){
                lhsToHead = lhs._cur - lhs.getBlockHead(lhs._mapIndex);
            }
            auto rhsToTail = rhs.getBlockTail(rhs._mapIndex) - rhs._cur + 1;
            auto innerBlockNum = (lhs._mapIndex - rhs._mapIndex - 1) * (lhs.getBlockSize());
            //std::cout << "lhs=" << lhsToHead << " rhs=" << rhsToTail << " inner=" << innerBlockNum<< std::endl;
            return lhsToHead + rhsToTail + innerBlockNum;
        }

        template<typename T>
        deque_iterator<T> operator + (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n){
            deque_iterator<T> res(it);
            // m: 该桶中后续还有多少个元素
            auto m = res.getBlockTail(res._mapIndex) - res._cur;
            if(n <= m){
                res._cur += n;
            }else{
                n -= (m+1); // it初始所在桶全部元素, +1表示加上it所指元素本身
                res._mapIndex += (n / res.getBlockSize() + 1);
                auto ptr = res.getBlockHead(res._mapIndex);
                auto dif = (n % res.getBlockSize());
                res._cur = ptr + dif;
            }
            return res;
        }

        template<typename T>
        deque_iterator<T> operator + (typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it){
            return (it + n);
        }

        template<typename T>
        deque_iterator<T> operator - (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n){
            deque_iterator<T> res(it);
            auto m = res._cur - res.getBlockHead(res._mapIndex);
            if(n <= m){
                res._cur -= n;
            }else{
                n -= m;
                res._mapIndex -= (n / res.getBlockSize() + 1);
                auto ptr = res.getBlockTail(res._mapIndex);
                auto dif = (n % res.getBlockSize()) - 1;
                res._cur = ptr - dif;
            }
            return res;
        }

        template<typename T>
        deque_iterator<T> operator - (typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it){
            return (it - n);
        }

        /* 比较运算 */
        template<typename T>
        bool deque_iterator<T>::operator == (const deque_iterator<T>& rhs)const{
            return (_container == rhs._container) && (_mapIndex == rhs._mapIndex) && (_cur == rhs._cur);
        }

        template<typename T>
        bool deque_iterator<T>::operator != (const deque_iterator<T>& rhs)const{
            return !(*this == rhs);
        }

        template<typename T>
        void deque_iterator<T>::swap(deque_iterator<T> &it){
            std::swap(_mapIndex, it._mapIndex);
            std::swap(_cur, it._cur);
        }

        template<typename T>
        T* deque_iterator<T>::getBlockTail(size_t mapIndex)const{
            return _container->_map[mapIndex] + (_container->getBlockSize() - 1);
        }
        template<typename T>
        T* deque_iterator<T>::getBlockHead(size_t mapIndex)const{
            return _container->_map[mapIndex];
        }
        template<typename T>
        size_t deque_iterator<T>::getBlockSize()const{
            return _container->getBlockSize();
        }
    } // namespace it

    /* 构造、析构、赋值 */
    template<typename T, typename Alloc>
    deque<T, Alloc>::deque(size_type n, const value_type& val):_map(nullptr), _mapSize(0){
        __deque(n, val, typename std::is_integral<size_type>::type());
    }

    template<typename T, typename Alloc>
    template<typename InputIterator>
    deque<T, Alloc>::deque(InputIterator first, InputIterator last):_map(nullptr), _mapSize(0){
        __deque(first, last, typename std::is_integral<InputIterator>::type());
    }

    template<typename T, typename Alloc>
    void deque<T, Alloc>::__deque(size_type n, const value_type &val, std::true_type){
        for(size_t i=0; i<n; ++i){
            if(i < n/2) this->push_back(val);
            else this->push_front(val);
        }
    }

    template<typename T, typename Alloc>
    template<typename InputIterator>
    void deque<T, Alloc>::__deque(InputIterator first, InputIterator last, std::false_type){
        auto mid = (last - first)/2;
        for(auto it = first + mid; it != first - 1; --it){
            this->push_front(*it);
        }
        for(auto it = first + mid + 1; it != last; ++it){
            this->push_back(*it);
        }
    }

    template<typename T, typename Alloc>
    deque<T, Alloc>::deque(const deque &other){
        _mapSize = other._mapSize;
        _map = getNewMap(_mapSize);
        size_t startIndex = other._start._mapIndex;
        size_t finishIndex = other._finish._mapIndex;
        for(size_t i=0; i + startIndex <= finishIndex; ++i){
            size_t j = 0;
            size_t ceiling = getBlockSize();
            if(i == 0){
                // start offset
                j = other._start._cur - other._map[other._start._mapIndex];
            }
            if(i == finishIndex - startIndex){
                if(other.back_full()) ceiling = 0;
                else ceiling = other._finish._cur - other._map[other._finish._mapIndex];
            }
            for(; j<ceiling; ++j){
                _map[startIndex + i][j] = other._map[startIndex + i][j];
            }
        }
        
        size_t start_offset = other._start._cur - other._map[other._start._mapIndex];
        size_type size = other.size();
        _start = iterator{startIndex, _map[startIndex]+start_offset, this};
        
        size_t finish_offset = 0;
        if(!other.back_full()){
            finish_offset = other._finish._cur - other._map[_finish._mapIndex];
        }
        _finish = iterator{finishIndex, _map[finishIndex]+finish_offset, this};
    }

    template<typename T, typename Alloc>
    deque<T, Alloc>::deque(const deque &&other){
        _start = other._start;
        _finish = other._start;
        _map = other._map;
        _mapSize = other._mapSize;
        _start._container = _finish._container = this;
        
        other._start = other._finish = iterator();
        other._map = nullptr;
        other._mapSize = 0;
    }

    template<typename T, typename Alloc>
    deque<T, Alloc>::~deque(){
        for(size_t i=0; i<_mapSize; ++i){
            for(auto ptr = _map[i]; ptr != nullptr && (ptr != _map[i] + getBlockSize()); ++ptr){
                dataAllocator::destroy(ptr);
            }
            if(_map[i] != nullptr){
                dataAllocator::deallocate(_map[i], getBlockSize());
            }
        }
        delete[] _map;
    }

    /* 增删改 */
    template<typename T, typename Alloc>
    void deque<T, Alloc>::push_back(const value_type& val){
        if(empty()){
            init();
        }else if(back_full()){
            reallocateAndMove();
        }
        //std::cout << "val=" << val << "_finish_mapIndex = " << _finish._mapIndex<< " _finish._cur=" <<_finish._cur << std::endl;
        dataAllocator::construct(_finish._cur, val);
        ++_finish;
    }

    template<typename T, typename Alloc>
    void deque<T, Alloc>::push_front(const value_type& val){
        if(empty()){
            init();
        }else if(front_full()){
            reallocateAndMove();
        }
        --_start;
        dataAllocator::construct(_start._cur, val);
    }

    template<typename T, typename Alloc>
    void deque<T, Alloc>::pop_back(){
        --_finish;
        dataAllocator::destroy(_finish._cur);
    }

    template<typename T, typename Alloc>
    void deque<T, Alloc>::pop_front(){
        dataAllocator::destroy(_start._cur);
        ++_start;
    }

    template<typename T, typename Alloc>
    void deque<T, Alloc>::swap(deque &x){
        std::swap(_map, x._map);
        std::swap(_mapSize, _mapSize);
        _start.swap(x._start);
        _finish.swap(x._finish);
    }


    template<typename T, typename Alloc>
    void deque<T, Alloc>::clear(){
        if(_map == nullptr) return;
        for(size_t i=0; i<_mapSize; ++i){
            for(auto ptr = _map[i]; ptr != nullptr && (ptr != _map[i] + getBlockSize()); ++ptr){
                dataAllocator::destroy(ptr);
            }
        }
        _start._mapIndex = _finish._mapIndex = _mapSize/2;
        _start._cur = _finish._cur = _map[_mapSize/2];
    }


    /* aux */
    template<typename T, typename Alloc>
    void deque<T, Alloc>::init(){
        _mapSize = 2;
        _map = getNewMap(_mapSize);
        _start._container = _finish._container = this;
        _start._mapIndex = _finish._mapIndex = _mapSize / 2;
        _start._cur = _finish._cur = _map[_mapSize / 2];
    }

    template<typename T, typename Alloc>
    bool deque<T, Alloc>::back_full()const{
        return _finish._mapIndex == _finish._container->_mapSize;
    }

    template<typename T, typename Alloc>
    bool deque<T, Alloc>::front_full()const{
        return _start._cur == _map[0];
    }


    template<typename T, typename Alloc>
    typename deque<T, Alloc>::pointer
    deque<T, Alloc>::getNewBlock(){
        return dataAllocator::allocate(getBlockSize());
    }

    template<typename T, typename Alloc>
    typename deque<T, Alloc>::map_pointer
    deque<T, Alloc>::getNewMap(const size_t size){
        typename deque<T, Alloc>::map_pointer map = new T*[size];
        for(size_t i=0; i<size; i++){
            map[i] = getNewBlock();
        }
        return map;
    }

    template<typename T, typename Alloc>
    size_t deque<T, Alloc>::getBlockSize()const{
        return (size_t)eBlockSize::BlockSize;
    }

    template<typename T, typename Alloc>
    size_t deque<T, Alloc>::getNewMapSize(const size_t size){
        return (size == 0) ? 2 : size * 2;
    }

    template<typename T, typename Alloc>
    void deque<T, Alloc>::reallocateAndMove(){
        size_t newSize = getNewMapSize(_mapSize);
        deque<T, Alloc>::map_pointer newMap = getNewMap(newSize);
        size_t startIndex = newSize / 4;
        for(size_t i=0; i + _start._mapIndex <= _finish._mapIndex; ++i){
            size_t j = 0;
            size_t ceiling = getBlockSize();
            if(i == 0){
                j = _start._cur - _map[_start._mapIndex];
            }
            if(i == _finish._mapIndex - _start._mapIndex){
                //if _finish in _map[_mapSize], ceiling = 0
                if(back_full()) ceiling = 0;
                else ceiling = _finish._cur - _map[_finish._mapIndex];
            }
            for(; j < ceiling; ++j){
                newMap[startIndex + i][j] = {std::move(_map[_start._mapIndex + i][j])};
            }
        }
        size_t dif = _start._cur - _map[_start._mapIndex];
        size_type size = this->size();
        //std::cout << "size: " <<size <<std::endl;
        _mapSize = newSize;
        _map = newMap;
        _start = iterator{startIndex, _map[startIndex]+dif, this};
        _finish = _start + size;
    }

    

} // namespace istl


#endif