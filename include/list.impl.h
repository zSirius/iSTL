#ifndef _ISTL_LIST_IMPL_
#define _ISTL_LIST_IMPL_
#include "list.h"

namespace istl
{
    namespace it
    {
        template<typename T>
        list_iterator<T>& list_iterator<T>::operator ++ (){
            if(_ptr != nullptr) _ptr = _ptr->_next;
            return *this;
        }

        template<typename T>
        list_iterator<T> list_iterator<T>::operator ++ (int){
            list_iterator<T> res = *this;
            ++(*this);
            return res;
        }

        template<typename T>
        list_iterator<T>& list_iterator<T>::operator -- (){
            if(_ptr != nullptr) _ptr = _ptr->_prev;
            return *this;
        }

        template<typename T>
        list_iterator<T> list_iterator<T>::operator -- (int){
            list_iterator<T> res = *this;
            --(*this);
            return res;
        }

        template<typename T>
        bool list_iterator<T>::operator ==(const list_iterator<T>& rhs)const{
            return _ptr == rhs._ptr;
        }

        template<typename T>
        bool list_iterator<T>::operator !=(const list_iterator<T>& rhs)const{
            return !(_ptr == rhs._ptr);
        }
    } // namespace it

    template<typename T, typename Alloc>
    void list<T, Alloc>::__list(size_type n, const value_type& val, std::true_type){
        nodePtr node = newNode();
        node->_next = node;
        node->_prev = node;

        _head._ptr = node; //dummy node
        _tail._ptr = node;
        while(n--){
            push_back(val);
        }
    }

    template<typename T, typename Alloc>
    template<typename InputIterator>
    void list<T, Alloc>::__list(InputIterator first, InputIterator last, std::false_type){
        nodePtr node = newNode();
        node->_next = node;
        node->_prev = node;

        _head._ptr = node;
        _tail._ptr = node;
        for(InputIterator it = first; it != last; ++it){
            push_back(*it);
        }
    }

    template<typename T, typename Alloc>
    list<T, Alloc>::list(){
        nodePtr node = newNode();
        node->_next = node;
        node->_prev = node;

        _head._ptr = node;
        _tail._ptr = node;
    }
    
    template<typename T, typename Alloc>
    list<T, Alloc>::list(size_type n, const value_type& val){
        __list(n, val, typename std::is_integral<size_type>::type());
    }
    
    template<typename T, typename Alloc>
    template<typename InputIterator>
    list<T, Alloc>::list(InputIterator first, InputIterator last){
        __list(first, last, typename std::is_integral<InputIterator>::type());
    }

    template<typename T, typename Alloc>
    list<T, Alloc>::list(const list& other){
        nodePtr node = newNode();
        node->_next = node;
        node->_prev = node;

        _head._ptr = node;
        _tail._ptr = node;
        for(auto node = other._head._ptr; node != other._tail._ptr; node = node->_next){
            push_back(node->_data);
        }
    }

    template<typename T, typename Alloc>
    list<T, Alloc>::~list(){
        iterator it = _head;
        while(it != _tail){
            iterator tmp = it++;
            nodeAllocator::destroy(tmp._ptr);
            nodeAllocator::deallocate(tmp._ptr);
        }
        nodeAllocator::destroy(_tail._ptr);
        nodeAllocator::deallocate(_tail._ptr);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::push_back(const value_type& val){
        nodePtr node = newNode(val, (_tail._ptr)->_prev, (_tail._ptr));
        node->_prev->_next = node;
        node->_next->_prev = node;
        _head._ptr = (_tail._ptr)->_next;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::pop_back(){
        if(_head._ptr == _tail._ptr) return; //if no more node
        nodePtr nodeTopop = (_tail._ptr)->_prev;
        nodeTopop->_next->_prev = nodeTopop->_prev;
        nodeTopop->_prev->_next = nodeTopop->_next;
        deleteNode(nodeTopop);
        _head._ptr = (_tail._ptr)->_next;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::push_front(const value_type& val){
        nodePtr node = newNode(val, _tail._ptr, (_tail._ptr)->_next);
        node->_next->_prev = node;
        node->_prev->_next = node;
        _head._ptr = (_tail._ptr)->_next;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::pop_front(){
        if(_head._ptr == _tail._ptr) return; //if no more node
        nodePtr nodeTopop = (_tail._ptr)->_next;
        nodeTopop->_next->_prev = nodeTopop->_prev;
        nodeTopop->_prev->_next = nodeTopop->_next;
        deleteNode(nodeTopop);
        _head._ptr = (_tail._ptr)->_next;
    }

    template<typename T, typename Alloc>
    typename list<T, Alloc>::size_type list<T, Alloc>::size()const{
        return istl::distance(_head, _tail);
    }

    template<typename T, typename Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::insert(iterator position, const value_type& val){
        iterator pre = position;
        --pre;
        nodePtr node = newNode(val, (position._ptr)->_prev, (position._ptr));
        node->_next->_prev = node;
        node->_prev->_next = node;
        _head._ptr = (_tail._ptr)->_next;
        return ++pre;
    }

    template<typename T, typename Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::__insert(iterator position, size_type n, const value_type& val, std::true_type){
        while(n--){
            position = insert(position, val);
        }
        return position;
    }

    template<typename T, typename Alloc>
    template <typename InputIterator>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::__insert(iterator position, InputIterator first, InputIterator last, std::false_type){
        iterator ret = position;
        --ret;
        for(iterator it = first; it != last; ++it){
            insert(position, *it);
        }
        return ++ret;
    }

    template<typename T, typename Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::insert(iterator position, size_type n, const value_type& val){
        return __insert(position, n, val, typename std::is_integral<size_type>::type());
    }

    template<typename T, typename Alloc>
    template <typename InputIterator>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last){
        return __insert(position, first, last, typename std::is_integral<InputIterator>::type());
    }

    template<typename T, typename Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::erase(iterator position){
        iterator pre = position, post = position;
        --pre, ++post;
        pre._ptr->_next = post._ptr;
        post._ptr->_prev = pre._ptr;
        deleteNode(position._ptr);
        _head._ptr = (_tail._ptr)->_next;
        return post;
    }

    template<typename T, typename Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::erase(iterator first, iterator last){
        iterator it = first;
        while(it != last){
            it = erase(it);
        }
        return it;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list& other){
        splice(pos, other, other.begin(), other.end());
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator it){
        // remove node from other list
        nodePtr node = it._ptr;
        nodePtr prev = (it._ptr)->_prev;
        nodePtr next = (it._ptr)->_next;

        prev->_next = next;
        next->_prev = prev;
        
        //insert node to new list
        nodePtr pos_node = pos._ptr;
        nodePtr pos_prev = (pos._ptr)->_prev;
        pos_node->_prev = node;
        node->_next = pos_node;
        pos_prev->_next = node;
        node->_prev = pos_prev;

        //update head
        _head._ptr = (_tail._ptr)->_next;
        other._head._ptr = (other._tail._ptr)->_next;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last){
        iterator it = first;
        while(it != last){
            splice(pos, other, it++);
        }
    }
    


    



    template<typename T, typename Alloc>
    typename list<T, Alloc>::nodePtr list<T, Alloc>::newNode(const value_type& val, nodePtr prev, nodePtr next){
        nodePtr ptr = nodeAllocator::allocate();
        nodeAllocator::construct(ptr, it::node<T>(val, prev, next, this));
        return ptr;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::deleteNode(nodePtr ptr){
        if(ptr == nullptr) return;
        ptr->_prev = ptr->_next = nullptr;
        nodeAllocator::destroy(ptr);
        nodeAllocator::deallocate(ptr);
    }

} // namespace istl


#endif