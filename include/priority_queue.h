#ifndef _ISTL_PRIORITY_QUEUE_H_
#define _ISTL_PRIORITY_QUEUE_H_

#include "vector.h"
#include <algorithm>

namespace istl
{
    /* https://en.cppreference.com/w/cpp/container/priority_queue */
    template<typename T, typename Container = istl::vector<T>, typename Compare = std::less<typename Container::value_type>>
    class priority_queue
    {
    public:
        typedef T                                           value_type;
        typedef Container                                   container_type;
		typedef typename Container::reference               reference;
		typedef typename Container::const_reference         const_reference;
		typedef typename Container::size_type               size_type;
    
    private:
        container_type  _container;
        Compare         _compare;
        
    public:
        explicit priority_queue(const Container& container = Container(), 
                                const Compare& compare = Compare())
                                : _container(container), _compare(compare){}

        template<typename InputIterator>
        priority_queue(InputIterator first, InputIterator last, 
                       const Container& container = Container(), 
                       const Compare& compare = Compare())
                       : _container(container), _compare(compare)
                       {
                        _container.insert(_container.end(), first, last);
                        build_heap();
                       }
        
        bool empty() { return _container.empty(); }
        size_type size() { return _container.size(); }
        const_reference top()const { return _container.front(); }

        //modifiers
        void push(const value_type& val){
            _container.push_back(val);
            heap_up(_container.size()-1);
        }

        void pop(){
            if(size() == 0) return;
            std::swap(_container[0], _container[size()-1]);
            _container.pop_back();
            heap_down(0);
        }

    
    private:
        void build_heap(){
            size_type n = _container.size();
            for(int i=(n)/2-1; i >=0; --i){
                heap_down(i);
            }
        }

        void heap_down(int k){
            int n = _container.size();
            value_type val = _container[k];
            for(int i=k*2+1; i<n; i = i*2+1){
                // select one child
                if(i<n-1 && _compare(_container[i], _container[i+1])){
                    ++i;
                }
                // compare child and parent
                if(_compare(val, _container[i])){
                    _container[k] = _container[i];
                    k = i;
                }else{
                    break;
                }
            }
            _container[k] = val;
        }

        void heap_up(int k){
            int n = _container.size();
            value_type val = _container[k];
            for(int i=(k-1)/2; i>=0; i=(i-1)/2){
                size_type parent = _container[i];
                if(k!=0 && _compare(parent, val)){
                    _container[k] = parent;
                    k = i;
                }else{
                    break;
                }
            }
            _container[k] = val;
        }

    };
    

    
} // namespace istl


#endif