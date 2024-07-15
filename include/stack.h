#ifndef _ISTL_STACK_H_
#define _ISTL_STACK_H_

#include "deque.h"

namespace istl
{
    template<typename T, typename Container = istl::deque<T> > class stack;
    /* 友元函数前向声明 */
    template <typename T, typename Container>
    bool operator== (const stack<T, Container>& lhs, const stack<T, Container>& rhs);
    template <typename T, typename Container>
    bool operator!= (const stack<T, Container>& lhs, const stack<T, Container>& rhs);
    // template <typename T, typename Container>
	// void swap(stack<T, Container>& x, stack<T, Container>& y);

    /* https://en.cppreference.com/w/cpp/container/stack */
    template<typename T, typename Container>
    class stack
    {
    public:
        typedef typename Container::value_type value_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        typedef typename Container::size_type size_type;
        typedef Container container_type;

    private:
        container_type _container;
    
    public:
        stack():_container(){}
        explicit stack(const container_type& container):_container(container){}

        bool empty()const { return _container.empty; }
        size_type size()const { return _container.size(); }
        reference top(){ return _container.back(); }
        const_reference top()const{ return _container.back(); }

        void push(const value_type& val){ _container.push_back(val); }
        void pop(){ _container.pop_back(); }
        void swap(stack& other){ std::swap(_container, other.container_); }
    
    public:
        friend bool operator== <>(const stack<T, Container>& lhs, const stack<T, Container>& rhs);
        friend bool operator!= <>(const stack<T, Container>& lhs, const stack<T, Container>& rhs);
        // friend void swap<>(stack<T, Container>& x, stack<T, Container>& y);
    };

    template <typename T, typename Container>
    bool operator== (const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return lhs._container == rhs._container;
    }
    template <typename T, typename Container>
    bool operator!= (const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return lhs._container != rhs._container;
    }
    // template <typename T, typename Container>
	// void swap(stack<T, Container>& x, stack<T, Container>& y){
    //     x.swap(y);
    // }

    
} // namespace istl



#endif