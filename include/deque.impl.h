#ifndef _DEQUE_IMPL_H_
#define _DEQUE_IMPL_H_
#include <stddef.h>
#include "deque.h"

namespace istl
{
    namespace it
    {
        /* 运算符重载 */
        template<typename T>
        deque_iterator<T>& deque_iterator<T>::operator ++ (){
            if(_cur != getBlockTail(_mapIndex)){
                ++_cur;
            }else if(_mapIndex + 1 < _container->_mapSize){
                ++_mapIndex;
                _cur = getBlockHead[_mapIndex];
            }else{
                _mapIndex = _container->_mapSize;
                _cur = _container->_map[_mapIndex];
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
            auto lhsToHead = lhs._cur - lhs.getBlockHead(lhs._mapIndex);
            auto rhsToTail = rhs.getBlockTail(rhs._mapIndex) - rhs._cur + 1;
            auto mapIndexDiff = lhs._mapIndex - rhs._mapIndex - 1;
            return lhsToHead + rhsToTail + mapIndexDiff;
        }

        template<typename T>
        deque_iterator<T> operator + (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n){
            if(n < 0) return (it - (-n));
            deque_iterator<T> res(it);
            auto m = res.getBlockTail(res._mapIndex) - res._cur;
            if(n <= m){
                res._cur += n;
            }else{
                n -= m;
                res._mapIndex += (n / res.getBlockSize() + 1);
                auto ptr = res.getBlockHead(res._mapIndex);
                auto dif = n % res.getBlockSize() - 1;
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
            if(n < 0) return (it + (-n));
            deque_iterator<T> res(it);
            auto m = res._res - res.getBlockHead(res._mapIndex);
            if(n <= m){
                res._cur -= n;
            }else{
                n -= m;
                res._mapIndex -= (n / res.getBlockSize() + 1);
                auto ptr = res.getBlockTail(res._mapIndex);
                auto dif = n % res.getBlockSize - 1;
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
    



} // namespace istl


#endif