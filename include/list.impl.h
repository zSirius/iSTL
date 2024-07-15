#ifndef _ISTL_LIST_IMPL_
#define _ISTL_LIST_IMPL_
#include "list.h"
#include "functional.h"

namespace istl
{
    namespace it
    {
        template<typename T, typename Ref, typename Ptr>
        list_iterator<T, Ref, Ptr>& list_iterator<T, Ref, Ptr>::operator ++ (){
            if(_ptr != nullptr) _ptr = _ptr->_next;
            return *this;
        }

        template<typename T, typename Ref, typename Ptr>
        list_iterator<T, Ref, Ptr> list_iterator<T, Ref, Ptr>::operator ++ (int){
            list_iterator<T, Ref, Ptr> res = *this;
            ++(*this);
            return res;
        }

        template<typename T, typename Ref, typename Ptr>
        list_iterator<T, Ref, Ptr>& list_iterator<T, Ref, Ptr>::operator -- (){
            if(_ptr != nullptr) _ptr = _ptr->_prev;
            return *this;
        }

        template<typename T, typename Ref, typename Ptr>
        list_iterator<T, Ref, Ptr> list_iterator<T, Ref, Ptr>::operator -- (int){
            list_iterator<T, Ref, Ptr> res = *this;
            --(*this);
            return res;
        }

        template<typename T, typename Ref, typename Ptr>
        bool list_iterator<T, Ref, Ptr>::operator ==(const list_iterator<T, Ref, Ptr>& rhs)const{
            return _ptr == rhs._ptr;
        }

        template<typename T, typename Ref, typename Ptr>
        bool list_iterator<T, Ref, Ptr>::operator !=(const list_iterator<T, Ref, Ptr>& rhs)const{
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
    list<T, Alloc>::list(std::initializer_list<T> init){
        nodePtr node = newNode();
        node->_next = node;
        node->_prev = node;

        _head._ptr = node;
        _tail._ptr = node;

        for(const T& value : init){
            push_back(value);
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
        splice(pos, other, other.cbegin(), other.cend());
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
        const_iterator it = first;
        while(it != last){
            splice(pos, other, it++);
        }
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::swap(list& other){
        std::swap(_head._ptr, other._head._ptr);
        std::swap(_tail._ptr, other._tail._ptr);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::clear(){
        erase(_head, _tail);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::unique(){
        if(size() == 0) return;
        iterator first = _head, last = _head;
        while(last != _tail){
            while(last != _tail && (first._ptr)->_data == (last._ptr)->_data){
                ++last;
            }
            erase(++first, last);
            first = last;
        }
    }

    template<typename T, typename Alloc>
    template<typename BinaryPredicate>
    void list<T, Alloc>::unique(BinaryPredicate binary_pred) {
        if (size() == 0) return; 
        iterator first = _head, last = _head;
        while (last != _tail) {
            while(last != _tail && binary_pred((first._ptr)->_data, (last._ptr)->_data)) {
                ++last;
            }
            erase(++first, last);
            first = last;
        }
    }



    template<typename T, typename Alloc>
    void list<T, Alloc>::remove(const value_type& val){
        iterator it = _head;
        while(it != _tail){
            if((*it) == val){
                erase(it++);
            }else{
                ++it;
            }
        }
    }

    template<typename T, typename Alloc>
    template<typename Predicate>
    void list<T, Alloc>::remove_if(Predicate pred) {
        iterator it = begin();
        while (it != end()) {
            if (pred(*it)) {
                erase(it++);
            } else {
                ++it;
            }
        }
    }


    /*               merge head1 and head2 
     ------        -------      -------      ------
    | prev |  ->  | head1 | -> | head2 | -> | next | -> nullptr
     ------        -------      -------      ------
    */
    template<typename T, typename Alloc>
    void list<T, Alloc>::sort(){
        sort(istl::less<T>());
    }
    template<typename T, typename Alloc>
    template <typename Compare>
    void list<T, Alloc>::sort(Compare comp){
        size_t length = size();
        if(length == 0) return;
        (_tail._ptr)->_prev->_next = nullptr;
        for(int sublength = 1; sublength < length; sublength <<= 1){
            nodePtr prev = _tail._ptr, curr = prev->_next;
            while(curr != nullptr){
                nodePtr head1 = curr;
                //head1->_prev = nullptr;
                for(int i=1; i<sublength && curr->_next != nullptr; ++i){
                    curr = curr->_next;
                }
                nodePtr head2 = curr->_next;
                curr->_next = nullptr; //断开head1 head2
                curr = head2; //head2可能为nullptr
                for(int i=1; i<sublength && curr != nullptr && curr->_next != nullptr; ++i){
                    curr = curr->_next;
                }
                nodePtr next = nullptr;
                if(curr != nullptr){ // head2 is not null
                    next = curr->_next;
                    curr->_next = nullptr; //断开head2和下一个子串
                    //head2->_prev = nullptr;
                }
                nodePtr merged_res = merge(head1, head2, comp);
                prev->_next = merged_res;
                merged_res->_prev = prev;
                while(prev->_next != nullptr) prev = prev->_next;
                curr = next;
            }
        }
        _head._ptr = (_tail._ptr)->_next;
        nodePtr lastNode = _head._ptr;
        while(lastNode->_next != nullptr){
            lastNode = lastNode->_next;
        }
        lastNode->_next = _tail._ptr;
        (_tail._ptr)->_prev = lastNode;
    }

    template<typename T, typename Alloc>
    template <typename Compare>
    typename list<T, Alloc>::nodePtr
    list<T, Alloc>::merge(nodePtr head1, nodePtr head2, Compare comp){
        nodePtr dummy = newNode(), ptr = dummy;
        while(head1 != nullptr && head2 != nullptr){
            if(comp(head1->_data, head2->_data)){
                ptr->_next = head1;
                head1->_prev = ptr;
                ptr = ptr->_next;
                head1 = head1->_next;
            }else{
                ptr->_next = head2;
                head2->_prev = ptr;
                ptr = ptr->_next;
                head2 = head2->_next;
            }
        }
        if(head1 != nullptr){
            ptr->_next = head1;
            head1->_prev = ptr;
        }
        if(head2 != nullptr){
            ptr->_next = head2;
            head2->_prev = ptr;
        }
        ptr = dummy->_next;
        deleteNode(dummy);
        return ptr;
    }
    template<typename T, typename Alloc>
    void list<T, Alloc>::merge(list& other){
        merge(other, less<T>());
    }

    template<typename T, typename Alloc>
    template <typename Compare>
    void list<T, Alloc>::merge(list& other, Compare comp){
        if(other.size() == 0) return;
        else if(size() == 0){
            splice(cbegin(), other);
            return;
        }

        (_tail._ptr)->_prev->_next = nullptr;
        (other._tail._ptr)->_prev->_next = nullptr;
        nodePtr head1 = _head._ptr, head2 = other._head._ptr;
        nodePtr merged_res = merge(head1, head2, comp);
        
        (_tail._ptr)->_next = merged_res;
        merged_res->_prev = (_tail._ptr);
        _head._ptr = merged_res;

        //process _tail._prev
        nodePtr lastNode = _head._ptr;
        while(lastNode->_next != nullptr){
            lastNode = lastNode->_next;
        }
        lastNode->_next = _tail._ptr;
        (_tail._ptr)->_prev = lastNode;

        //process other list's _head
        other._head._ptr = other._tail._ptr;
        (other._tail._ptr)->_next = other._tail._ptr;
        (other._tail._ptr)->_prev = other._tail._ptr;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::reverse(){
        iterator it = _head;
        ++it;
        while(it != _tail){
            splice(_head, *this, it++);
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