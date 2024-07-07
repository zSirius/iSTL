#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_

#include <type_traits>
#include <cstring>
#include "iterator.h"


namespace istl
{

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type){
        typedef typename istl::iterator_traits<InputIterator>::value_type ValueType;
        typename istl::iterator_traits<InputIterator>::difference_type n = istl::distance(first, last);
        memcpy(static_cast<void*>(result), static_cast<const void*>(&(*first)), n*sizeof(ValueType));
        return result+n;
    }

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type){
        ForwardIterator current = result;
        for(; first != last; ++first, ++current){
            new (static_cast<void *>(&(*current))) typename
            istl::iterator_traits<ForwardIterator>::value_type(*first);
        }
        return current;
    }

    template<typename InputIterator, typename ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        typedef typename std::is_trivial<typename istl::iterator_traits<InputIterator>::value_type>::type IsTriviallyCopyable;
        return _uninitialized_copy_aux(first, last, result, IsTriviallyCopyable());
    } 

    /*******************************************************************************/

    template<typename ForwardIterator, typename T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, std::true_type){
        for(; first != last; ++first){
            *first = value;
        }
    }

    template<typename ForwardIterator, typename T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, std::false_type){
        for(; first != last; ++first){
            new (static_cast<void *>(&(*first))) T(value);
        }
    }

    template<typename ForwardIterator, typename T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
        typedef typename std::is_trivial<T>::type IsTriviallyCopyable;
        _uninitialized_fill_aux(first, last, value, IsTriviallyCopyable());
    }

    /*******************************************************************************/

    template<typename ForwardIterator, typename Size, typename T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, std::true_type){
        for(Size i=0; i<n; ++i){
            *first = value;
            ++first;
        }
        return first;
    }

    template<typename ForwardIterator, typename Size, typename T>
    ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& value, std::false_type){
        ForwardIterator current = first;
        for(Size i=0; i<n; ++i, ++current){
            new (static_cast<void*>(&(*current))) T(value);
        }
        return current;
    }

    template<typename ForwardIterator, typename Size, typename T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& value){
         typedef typename std::is_trivial<T>::type IsTriviallyCopyable;
         return _uninitialized_fill_n_aux(first, n, value, IsTriviallyCopyable());
    }

} // namespace istl


#endif