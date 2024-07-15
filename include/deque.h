#ifndef _ISTL_DEQUE_H_
#define _ISTL_DEQUE_H_

#include "allocator.h"
#include "iterator.h"
#include <iostream>

namespace istl
{
    template<typename T, typename Alloc = istl::allocator<T> >
    class deque;
    template<typename T, typename Alloc> bool operator == (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);
    template<typename T, typename Alloc> bool operator != (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);
    namespace it
    {
        template<typename T> class deque_iterator;
        /* 友元函数前向声明 */
        template<typename T>typename deque_iterator<T>::difference_type operator - (const deque_iterator<T>& lhs, const deque_iterator<T>& rhs);
        template<typename T> deque_iterator<T> operator + (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n);
        template<typename T> deque_iterator<T> operator + (typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it);
        template<typename T> deque_iterator<T> operator - (const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n);
        template<typename T> deque_iterator<T> operator - (typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it);

        template<typename T>
        class deque_iterator : public iterator<bidirectional_iterator_tag, T>
        {
        public:
            typedef T*          pointer;
            typedef const T*    const_pointer;
            typedef T&          reference;
            typedef const T&    const_reference;

        private:
            friend class ::istl::deque<T>;
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
            reference operator *() { return *_cur; }
            const_reference operator *()const { return *_cur; }
            pointer operator ->() { return &(operator*()); }
            const_pointer operator ->()const { return &(operator*()); }

            /* 比较运算 */
            bool operator ==(const deque_iterator& rhs)const;
            bool operator !=(const deque_iterator& rhs)const;
        
        public:

            friend typename deque_iterator<T>::difference_type operator - <>(const deque_iterator<T>& lhs, const deque_iterator<T>& rhs);
            friend deque_iterator<T> operator + <>(const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n);
            friend deque_iterator<T> operator + <>(typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it);
            friend deque_iterator<T> operator - <>(const deque_iterator<T> &it, typename deque_iterator<T>::difference_type n);
            friend deque_iterator<T> operator - <>(typename deque_iterator<T>::difference_type n, const deque_iterator<T> &it);
            
            void swap(deque_iterator<T> &it);

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
        typedef T                                           value_type;
        typedef T*                                          pointer;
        typedef ::istl::it::deque_iterator<T>               iterator;
        typedef ::istl::it::deque_iterator<const T>         const_iterator;
        typedef T&                                          reference;
        typedef const T&                                    const_reference;
        typedef size_t                                      size_type;
        typedef ptrdiff_t                                   difference_type;
        typedef Alloc                                       allocator_type;
    
    private:
        friend class ::istl::it::deque_iterator<T>;
        friend class ::istl::it::deque_iterator<const T>;
        friend typename iterator::difference_type it::operator - <>(const iterator& lhs, const iterator& rhs);
        
        typedef Alloc                               dataAllocator;
        typedef allocator<T*>                       mapAllocator;
        typedef pointer*                            map_pointer;

    private:
        iterator _start;
        iterator _finish;
        map_pointer _map;
        size_type _mapSize;
        enum class eBlockSize{ BlockSize=64 };


    public:

        /* 构造、析构、赋值 */
        deque():_map(nullptr), _mapSize(0){}
        deque(size_type n, const value_type &val = value_type());
        template<typename InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(const deque &other);
        deque(const deque &&other);
        ~deque();
    
    private:
        void __deque(size_type n, const value_type &val, std::true_type);
        template<typename InputIterator>
        void __deque(InputIterator first, InputIterator last, std::false_type);

    public:
        /* 迭代器相关 */
        iterator begin(){ return _start; }
        iterator end() { return _finish; }
        iterator begin()const { return _start; }
        iterator end() const { return _finish; }

        /* 元素访问 */
        reference front(){ return *begin(); }
        reference back(){ return *(end()-1); }
        const_reference front()const{ *begin(); }
        const_reference back()const{ return *(end()-1); }
        // reference operator[] (size_type n){ 
        //     std::cout << "in []: begin = " << _start._cur << " n = " << n << std::endl;
        //     std::cout << " begin()+n=" << (_start+n)._cur << std::endl;
        //     std::cout << " end()=" << _finish._cur << " val=" << *(_start+n) << std::endl; 
        //     return *(begin()+n); }
        reference operator[] (size_type n){ return *(begin()+n); }
        const_reference operator[](size_type n)const{ return *(begin()+n); };

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
        friend bool operator == <>(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);
        friend bool operator != <>(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);

    private:
        void init();
        bool back_full()const;
        bool front_full()const;
        pointer getNewBlock();
        map_pointer getNewMap(const size_t size);
        size_t getBlockSize()const;
        size_t getNewMapSize(const size_t size);
        void reallocateAndMove();

    };
} // namespace istl


#include "deque.impl.h"
#endif