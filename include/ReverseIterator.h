#ifndef _REVERSE_ITERATOR_H_
#define _REVERSE_ITERATOR_H_
#include "iterator.h"

namespace istl
{
    template<typename Iterator>
    class reverse_iterator_t{
        public:
            typedef Iterator iterator_type;
            typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
            typedef typename iterator_traits<Iterator>::value_type value_type;
            typedef typename iterator_traits<Iterator>::difference_type difference_type;
            typedef typename iterator_traits<Iterator>::pointer pointer;
            typedef const pointer const_pointer;
            typedef typename iterator_traits<Iterator>::reference reference;
            typedef const reference const_reference;
        private:
            Iterator _base;
            Iterator _cur;
        public:
            //构造、拷贝
            reverse_iterator_t() :_base(nullptr), _cur(nullptr){}
            explicit reverse_iterator_t(const iterator_type &it) :_base(it){
                _cur = it-1;
            }
            template<typename Iter>
            reverse_iterator_t(const reverse_iterator_t<Iter> &it){
                _base = (iterator_type)it.base();
                _cur = _base-1;
            }

            iterator_type base(){ return _base; }
            reference operator*(){ return (*_cur); }
            const_reference operator*()const{ return(*_cur); }
            pointer operator->(){ return &(operator *()); }
            const_pointer operator->()const{ return &(operator*()); }

            reverse_iterator_t& operator ++(){
                --_base;
                --_cur;
                return *this;
            }
            reverse_iterator_t& operator ++(int){
                reverse_iterator_t temp = *this;
                ++(*this);
                return temp;
            }
            reverse_iterator_t& operator--(){
                ++_base;
                ++_cur;
                return *this;
            }
            reverse_iterator_t  operator--(int){
                reverse_iterator_t temp = *this;
                --(*this);
                return temp;
            }
            reference operator[] (difference_type n){
                return base()[-n - 1];
            }
            reverse_iterator_t operator + (difference_type n)const;
            reverse_iterator_t& operator += (difference_type n);
            reverse_iterator_t operator - (difference_type n)const;
            reverse_iterator_t& operator -= (difference_type n);

        private:
            Iterator advanceNStep(Iterator it,
                difference_type n,
                bool right,//true -> \ false <-
                random_access_iterator_tag){
                if (right){
                    it += n;
                }else{
                    it -= n;
                }
                return it;
            }
            Iterator advanceNStep(Iterator it,
                difference_type n,
                bool right,//true -> \ false <-
                bidirectional_iterator_tag){
                difference_type i;
                difference_type absN = n >= 0 ? n : -n;
                if ((right && n > 0) || (!right && n < 0)){// ->
                    for (i = 0; i != absN; ++i){
                        it = it + 1;
                    }
                }
                else if ((!right && n > 0) || (right && n < 0)){// <-
                    for (i = 0; i != absN; ++i){
                        it = it - 1;
                    }
                }
                return it;
            }
        
        public:
            template <typename It>
            friend bool operator == (const reverse_iterator_t<It>& lhs, 
                                    const reverse_iterator_t<It>& rhs);
            template <typename It>
            friend bool operator != (const reverse_iterator_t<It>& lhs, 
                                    const reverse_iterator_t<It>& rhs);
            template <typename It>
            friend bool operator < (const reverse_iterator_t<It>& lhs,
                                    const reverse_iterator_t<It>& rhs);
            template <typename It>
            friend bool operator <= (const reverse_iterator_t<It>& lhs,
                                    const reverse_iterator_t<It>& rhs);
            template <typename It>
            friend bool operator > (const reverse_iterator_t<It>& lhs,
                                    const reverse_iterator_t<It>& rhs);
            template <typename It>
            friend bool operator >= (const reverse_iterator_t<It>& lhs,
                                    const reverse_iterator_t<It>& rhs);

            template <typename It>
            friend reverse_iterator_t<It> operator + (
                typename reverse_iterator_t<It>::difference_type n,
                const reverse_iterator_t<It>& rev_it);
            template <typename It>
            friend typename reverse_iterator_t<It>::difference_type operator- (
                const reverse_iterator_t<It>& lhs,
                const reverse_iterator_t<It>& rhs);
    };

    template<typename Iterator>
    reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator += (difference_type n){
        _base = advanceNStep(_base, n, false, iterator_category());
        _cur = advanceNStep(_cur, n, false, iterator_category());
        return *this;
    }
    template<typename Iterator>
    reverse_iterator_t<Iterator>& reverse_iterator_t<Iterator>::operator -= (difference_type n){
        _base = advanceNStep(_base, n, true, iterator_category());
        _cur = advanceNStep(_cur, n, true, iterator_category());
        return *this;
    }
    template<typename Iterator>
    reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator + (difference_type n)const{
        reverse_iterator_t<Iterator> res = *this;
        res += n;
        return res;
    }
    template<typename Iterator>
    reverse_iterator_t<Iterator> reverse_iterator_t<Iterator>::operator - (difference_type n)const{
        reverse_iterator_t<Iterator> res = *this;
        res -= n;
        return res;
    }

    template <typename Iterator>
    bool operator == (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs){
        return lhs._cur == rhs._cur;
    }
    template <typename Iterator>
    bool operator != (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs){
        return !(lhs == rhs);
    }
    template <typename Iterator>
    bool operator < (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs){
        return lhs._cur < rhs._cur;
    }
    template <typename Iterator>
    bool operator > (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs){
        return lhs._cur > rhs._cur;
    }
    template <typename Iterator>
    bool operator >= (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs){
        return !(lhs < rhs);
    }
    template <typename Iterator>
    bool operator <= (const reverse_iterator_t<Iterator>& lhs, const reverse_iterator_t<Iterator>& rhs){
        return !(lhs > rhs);
    }

    template <typename Iterator>
    reverse_iterator_t<Iterator> operator + (
        typename reverse_iterator_t<Iterator>::difference_type n,
        const reverse_iterator_t<Iterator>& rev_it){
        return rev_it + n;
    }
    template <typename Iterator>
    typename reverse_iterator_t<Iterator>::difference_type operator - (
        const reverse_iterator_t<Iterator>& lhs,
        const reverse_iterator_t<Iterator>& rhs){
        return lhs._cur - rhs._cur;
    }
} // namespace istl



#endif