#ifndef _ISTL_QUEUE_H_
#define _ISTL_QUEUE_H_

#include "deque.h"

namespace istl
{
    template<typename T, typename Container = istl::deque<T>> class queue;
    template <typename T, typename Container>
    bool operator== (const queue<T, Container>& lhs, const queue<T, Container>& rhs);
    template <typename T, typename Container>
    bool operator!= (const queue<T, Container>& lhs, const queue<T, Container>& rhs);
    
    // template <typename T, typename Container>
    // void swap(queue<T, Container>& x, queue<T, Container>& y);

    template<typename T, typename Container>
    class queue
    {
    public:
        typedef T value_type;
        typedef Container container_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        typedef typename Container::size_type size_type;
    private:
        Container _container;
    public:
        queue():_container(){}
        explicit queue(const container_type& container) :_container(container){}
        bool empty() const{ return _container.empty(); }
        size_type size() const{ return _container.size(); }
        reference& front(){ return _container.front(); }
        const_reference& front() const{ return _container.front(); }
        reference& back(){ return _container.back(); }
        const_reference& back() const{ return _container.back(); }
        void push(const value_type& val){ _container.push_back(val); }
        void pop(){ _container.pop_front(); }
        void swap(queue& other){ _container.swap(other._container); }

    public:

        friend bool operator== <>(const queue<T, Container>& lhs, const queue<T, Container>& rhs);

        friend bool operator!= <>(const queue<T, Container>& lhs, const queue<T, Container>& rhs);

        //friend void swap<>(queue<T, Container>& x, queue<T, Container>& y);

    };

    template <typename T, typename Container>
    bool operator== (const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return lhs._container == rhs._container;
    }

    template <typename T, typename Container>
    bool operator!= (const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return lhs._container != rhs._container;
    }

    // template <typename T, typename Container>
    // void swap(queue<T, Container>& x, queue<T, Container>& y){
    //     x._container.swap(y._container);
    // }

    
} // namespace istl


#endif