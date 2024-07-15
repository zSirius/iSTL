#ifndef _ISTL_LIST_H_
#define _ISTL_LIST_H_

#include "allocator.h"
#include "iterator.h"
#include "ReverseIterator.h"
#include "uninitialized.h"

/**********************************************************
 *     begin()    last valid node                         *
 *     ------       ------       ------                   *
 *  ->| node | <-> | node | <-> | tail |<--------------   *
 * |   ------       ------       ------                |  *
 * |                          end() is a dummy node    |  *
 * |                                                   |  *
 *  ---------------------------------------------------   *
***********************************************************/

/* 为了能够支持双向迭代器 list实现为双向循环链表 */

namespace istl
{
    namespace it
    {
        template<typename T> struct node;
    } // namespace it
    
    template<typename T, typename Alloc = istl::allocator<::istl::it::node<T>>>
    class list;
    namespace it
    {
        template<typename T>
        struct node
        {
            T _data;
            struct node *_prev;
            struct node *_next;
            list<T> *_container;
            node(const T& val, struct node *prev, struct node *next, list<T> *container):
                _data(val), _prev(prev), _next(next), _container(container){}
            
            bool operator == (const struct node& other){
                return _data == other._data && _prev == other._prev && _next == other._next && _container == other._container;
            }
        };
        
        template<typename T, typename Ref, typename Ptr>
        class list_iterator : public iterator<bidirectional_iterator_tag, T>
        {
        public:
            typedef list_iterator<T, T&, T*>                iterator;
            typedef list_iterator<T, const T&, const T*>    const_iterator;
            typedef Ref                                     reference;
            typedef Ptr                                     pointer;

        private:
            friend class istl::list<T>;
        public:
            typedef node<T>* nodePtr;
            nodePtr _ptr;

            list_iterator(nodePtr ptr = nullptr) : _ptr(ptr){}
            list_iterator(const list_iterator& other) = default;
            list_iterator& operator = (const list_iterator& other) = default;


            /* 运算符重载 */
            /* 注意不能是const类型 */
            list_iterator& operator ++ ();
            list_iterator operator ++ (int);
            list_iterator& operator -- ();
            list_iterator operator -- (int);
            reference operator * (){ return _ptr->_data; }
            pointer operator -> (){ return &(operator*()); }
            
            /* 比较运算 */
            bool operator ==(const list_iterator& rhs)const;
            bool operator !=(const list_iterator& rhs)const;
        };
    } // namespace it
    
    template<typename T, typename Alloc>
    class list
    {
    private:
        friend ::istl::it::list_iterator<T, T&, T*>;
        friend ::istl::it::list_iterator<T, const T&, const T*>;

        typedef Alloc nodeAllocator;
        typedef it::node<T>*  nodePtr;

    public:
        typedef T value_type;
        typedef it::list_iterator<T, T&, T*> iterator;
        typedef it::list_iterator<T, const T&, const T*> const_iterator;
        typedef reverse_iterator_t<iterator> reverse_iterator;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
    
    private:
        iterator _head;
        iterator _tail;
    
    public:
        list();
        list(size_type n, const value_type& val = value_type());
        template<typename InputIterator>
        list(InputIterator first, InputIterator last);
        list(const list& other);
        list& operator = (const list& other);
        ~list();
    
    private:
        void __list(size_type n, const value_type& val, std::true_type);
        template<typename InputIterator>
        void __list(InputIterator first, InputIterator last, std::false_type);
    
    public:
        bool empty()const{ return _head == _tail; }
        size_type size()const;
        reference front(){ return (_head._ptr->_data); }
        reference back(){ return (_tail._ptr->_prev->_data); }

        void push_front(const value_type& val);
        void pop_front();
        void push_back(const value_type& val);
        void pop_back();

        iterator begin(){ return _head; }
        iterator end() { return _tail; }
        const_iterator cbegin()const{ return const_iterator(_head._ptr); }
        const_iterator cend()const{return const_iterator(_tail._ptr);}
        reverse_iterator rbegin(){ return reverse_iterator(_head); }
        reverse_iterator rend(){ return reverse_iterator(_tail); }

        /* https://en.cppreference.com/w/cpp/container/list/insert */
        iterator insert(iterator position, const value_type& val);
        iterator insert(iterator position, size_type n, const value_type& val);
        template <typename InputIterator>
        iterator insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
    
    private:
        iterator __insert(iterator position, size_type n, const value_type& val, std::true_type);
        template <typename InputIterator>
        iterator __insert(iterator position, InputIterator first, InputIterator last, std::false_type);


    public:
        void swap(list& other);
        void clear();

        void splice(const_iterator pos, list& other);
        void splice(const_iterator pos, list& other, const_iterator it);
        void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

        void remove(const value_type& val);
        template <typename Predicate>
        void remove_if(Predicate pred);
        
        void unique();
        template <typename BinaryPredicate>
        void unique(BinaryPredicate binary_pred);
        
        void merge(list& other);
        template <typename Compare>
        void merge(list& other, Compare comp);
        
        void sort();
        template <typename Compare>
        void sort(Compare comp);
        
        void reverse();
    
    private:
        nodePtr newNode(const value_type& val = value_type(), nodePtr prev = nullptr, nodePtr next = nullptr);
        void deleteNode(nodePtr ptr);
        //used to merge sort and merge
        template <typename Compare>
        nodePtr merge(nodePtr head1, nodePtr head2, Compare comp);

    };
    

    
} // namespace istl

#include "list.impl.h"

#endif