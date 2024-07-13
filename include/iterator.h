#ifndef _ISTL_ITERATOR_H_
#define _ISTL_ITERATOR_H_

#include <cstddef>

namespace istl
{
    /* iterator category tag */
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag :public input_iterator_tag {};
    struct bidirectional_iterator_tag :public forward_iterator_tag {};
    struct random_access_iterator_tag :public bidirectional_iterator_tag {};


    /* base iterator */
    template<typename T, typename Distance>
    struct input_iterator
    {
        typedef input_iterator_tag      iterator_category;
        typedef T                       value_type;
        typedef Distance                difference_type;
        typedef T*                      pointer;
        typedef T&                      reference;
    };

    struct output_iterator
    {
        typedef output_iterator_tag     iterator_category;
        typedef void                    value_type;
        typedef void                    difference_type;
        typedef void                    pointer;
        typedef void                    reference;
    };

    template<typename T, typename Distance>
    struct forward_iterator
    {
        typedef forward_iterator_tag    iterator_category;
        typedef T                       value_type;
        typedef Distance                difference_type;
        typedef T*                      pointer;
        typedef T&                      reference;
    };

    template<typename T, typename Distance>
    struct bidirectional_iterator 
    {
        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };
    
    template<typename T, typename Distance>
    struct random_access_iterator
    {
        typedef random_access_iterator_tag    iterator_category;
        typedef T                            value_type;
        typedef Distance                    difference_type;
        typedef T*                            pointer;
        typedef T&                            reference;
    };


    /* general iterator template */
    template<typename Category, typename T, typename Distance = ptrdiff_t,
    typename Pointer = T*, typename Reference = T&>
    struct iterator
    {
        typedef Category    iterator_category;
        typedef T            value_type;
        typedef Distance    difference_type;
        typedef Pointer        pointer;
        typedef Reference    reference;
    };

    /* iterator traits*/
    template<typename Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type            value_type;
        typedef typename Iterator::difference_type        difference_type;
        typedef typename Iterator::pointer                pointer;
        typedef typename Iterator::reference             reference;
    };

    template<typename T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag     iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef T*                            pointer;
        typedef T&                             reference;
    };

    template<typename T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag     iterator_category;
        typedef T                             value_type;
        typedef ptrdiff_t                     difference_type;
        typedef const T*                    pointer;
        typedef const T&                     reference;
    };

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator &It){
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::value_type*
        value_type(const Iterator& It){
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
        difference_type(const Iterator& It){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }


    template<typename InputIterator>
    typename istl::iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last){
        typename istl::iterator_traits<InputIterator>::difference_type n=0;
        while(first!=last){
            ++first;
            ++n;
        }
        return n;
    }



} // namespace istl


#endif