#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "allocator.h"
#include "iterator.h"

namespace istl
{
    template<typename T, typename Alloc = istl::allocator<T> >
    class deque;
    namespace it
    {
        template<typename T>
        class deque_iterator : public iterator<bidirectional_iterator_tag, T>
        {
        public:
            typedef T*   pointer;
        private:
            template<typename T, typename Alloc>
            friend class ::istl::deque;
            typedef const ::istl::deque<T>* containerPtr;
        
        private:
            containerPtr _container;
            size_t _mapIndex;
            T* _cur;

        public:
            /* 构造函数 */
            deque_iterator():_container(nullptr), _mapIndex(-1), _cur(nullptr){}
            deque_iterator(size_t index, T *ptr, containerPtr container):_container(container), _mapIndex(index), _cur(ptr){}
            deque_iterator(const deque_iterator& it):_container(it._container), _mapIndex(it._mapIndex), _cur(it._cur){}
            deque_iterator& operator = (const deque_iterator& it);

            /* 运算符重载 */
            deque_iterator& operator ++ ();
            deque_iterator operator ++ (int);
            deque_iterator& operator -- ();
            deque_iterator operator -- (int);
            reference operator *() { return *cur_; }
            const reference operator *()const { return *cur_; }
            pointer operator ->() { return &(operator*()); }
            const pointer operator ->()const { return &(operator*()); }

            /* 比较运算 */
            bool operator ==(const deque_iterator& rhs)const;
            bool operator !=(const deque_iterator& rhs)const;
        
        public:
            template<typename T>
            friend typename deque_iterator<T>::difference_type operator - (const deque_iterator<T>& lhs, const deque_iterator<T>& rhs);
            template<typename T>
            friend deque_iterator<T> operator + (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n);
            template<typename T>
            friend deque_iterator<T> operator + (typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it);
            template<typename T>
            friend deque_iterator<T> operator - (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n);
            template<typename T>
            friend deque_iterator<T> operator - (typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it);
            template<typename T>
            friend void swap(deque_iterator<T> &lhs, deque_iterator<T> &rhs);

        private:
            T* getBlockTail(size_t mapIndex)const;
            T* getBlockHead(size_t mapIndex)const;
            size_t getBlockSize()const;
        };
    } // namespace it
    



    /* https://en.cppreference.com/w/cpp/container/deque */
    template<typename T, typename Alloc>
    class deque
    {
    public:
        typedef T                                   value_type;
        typedef T*                                  pointer;
        typedef it::deque_iterator<T>               iterator;
        typedef it::deque_iterator<const T>         const_iterator;
        typedef T&                                  reference;
        typedef const T&                            const_reference;
        typedef size_t                              size_type;
        typedef ptrdiff_t                           difference_type;
        typedef Alloc                               allocator_type;
    
    private:
        template<typename T>
        friend class ::istl::it::deque_iterator;
        
        typedef Alloc                               dataAllocator;
        typedef allocator<T*>                       mapAllocator;
        typedef pointer*                            map_pointer;

    private:
        iterator _start;
        iterator _finish;
        map_pointer _map;
        size_type _mapSize;
        enmu class eBlockSize{ BlockSize=64; }


    public:

        /* 构造、析构、赋值 */
        deque():_map(nullptr), _mapSize(0){}
        explicit deque(size_type n, const value_type& val = value_type());
        template<typename InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(const deque &other);
        ~deque();
        
        /* 迭代器相关 */
        iterator begin(){ return _start; }
        iterator end() { return _finish; }
        iterator begin()const { return _start; }
        iterator end() const { return _finish; }

        /* 元素访问 */
        reference front();
        reference back();
        reference front()const;
        reference back()const;
        reference operator[] (size_type n);
        reference operator[](size_type n)const;

        /* 增删改 */
        void push_back(const value_type& val);
        void push_front(const value_type& val);
        void pop_back();
        void pop_front();
        void swap(deque& x);
        void clear();

        /* 容器大小 */
        size_type size() const{ return end() - begin(); }
        bool empty() const{ return begin() == end(); }

        /* 容器比较 */
        template <typename T, typename Alloc>
        friend bool operator == (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);
        template <typename T, typename Alloc>
        friend bool operator != (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);

    private:


    };
} // namespace istl



#endif