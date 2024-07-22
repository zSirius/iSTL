#ifndef _ISTL_RB_TREE_H_
#define _ISTL_RB_TREE_H_
#include <utility>
#include <memory>
#include <iostream>
#include <type_traits>
#include "iterator.h"
#include "allocator.h"
#include "ReverseIterator.h"

//debug
#include <queue>

namespace istl
{
    enum _Rb_tree_color { _red = false, _black = true };

    struct _Rb_tree_node_base 
    {
        typedef _Rb_tree_node_base* _Base_ptr;
        typedef const _Rb_tree_node_base* _Const_Base_ptr;

        _Rb_tree_color _color;
        _Base_ptr _parent;
        _Base_ptr _left;
        _Base_ptr _right;

        static _Base_ptr _S_minimum(_Base_ptr __x){
            while (__x->_left != nullptr) __x = __x->_left;
            return __x;
        }

        static _Const_Base_ptr _S_minimum(_Const_Base_ptr __x){
            while (__x->_left != 0) __x = __x->_left;
            return __x;
        }

        static _Base_ptr _S_maximum(_Base_ptr __x){
            while (__x->_right != nullptr) __x = __x->_right;
            return __x;
        }

        static _Const_Base_ptr  _S_maximum(_Const_Base_ptr __x){
            while (__x->_right != 0) __x = __x->_right;
            return __x;
        }
    };

    // 红黑树节点
    template<typename _Val>
    struct _Rb_tree_node : public _Rb_tree_node_base {
        typedef _Rb_tree_node<_Val>* _Link_type;
        _Val _value_field;  // 存储键值对

        _Val* _valptr(){ return std::addressof(_value_field); }
        const _Val* _valptr()const { return std::addressof(_value_field); }

        _Rb_tree_node(const _Val& __value) : _value_field(__value) {
            _color = _red;
            _parent = _left = _right = nullptr;
        }
    };
    
    _Rb_tree_node_base* _Rb_tree_increment(_Rb_tree_node_base* __x);
    const _Rb_tree_node_base* _Rb_tree_increment(const _Rb_tree_node_base* __x);
    _Rb_tree_node_base* _Rb_tree_decrement(_Rb_tree_node_base* __x);
    const _Rb_tree_node_base*_Rb_tree_decrement(const _Rb_tree_node_base* __x);

    template<typename _Tp>
    struct _Rb_tree_iterator
    {
        typedef _Tp                             value_type;
        typedef _Tp&                            reference;
        typedef _Tp*                            pointer;

        typedef bidirectional_iterator_tag      iterator_category;
        typedef ptrdiff_t                       difference_type;

        typedef _Rb_tree_iterator<_Tp>          _Self;
        typedef _Rb_tree_node_base::_Base_ptr   _Base_ptr;
        typedef _Rb_tree_node<_Tp>*             _Link_type;

        _Base_ptr _node;

        _Rb_tree_iterator() : _node() { }

        explicit _Rb_tree_iterator(_Base_ptr __x) : _node(__x) { }

        reference operator*() const
        { return *static_cast<_Link_type>(_node)->_valptr(); }

        pointer operator->() const
        { return static_cast<_Link_type> (_node)->_valptr(); }

        _Self& operator++(){
            _node = _Rb_tree_increment(_node);
            return *this;
        }

        _Self operator++(int){
            _Self __tmp = *this;
            _node = _Rb_tree_increment(_node);
            return __tmp;
        }

        _Self& operator--(){
            _node = _Rb_tree_decrement(_node);
            return *this;
        }

        _Self operator--(int){
            _Self __tmp = *this;
            _node = _Rb_tree_decrement(_node);
            return __tmp;
        }

        friend bool
        operator==(const _Self& __x, const _Self& __y)
        { return __x._node == __y._node; }

        friend bool
        operator!=(const _Self& __x, const _Self& __y)
        { return __x._node != __y._node; }
    };

    
    template<typename _Tp>
    struct _Rb_tree_const_iterator
    {
        typedef _Tp        value_type;
        typedef const _Tp& reference;
        typedef const _Tp* pointer;

        typedef _Rb_tree_iterator<_Tp> iterator;

        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t                  difference_type;

        typedef _Rb_tree_const_iterator<_Tp>              _Self;
        typedef _Rb_tree_node_base::_Const_Base_ptr       _Base_ptr;
        typedef const _Rb_tree_node<_Tp>*                 _Link_type;

        _Base_ptr _node;

        _Rb_tree_const_iterator() : _node() { }

        explicit _Rb_tree_const_iterator(_Base_ptr __x) : _node(__x) { }

        _Rb_tree_const_iterator(const iterator& __it) : _node(__it._node) { }

        iterator _const_cast()
        { return iterator(const_cast<typename iterator::_Base_ptr>(_node)); }

        reference operator*() const 
        { return *static_cast<_Link_type>(_node)->_valptr(); }

        pointer operator->() const
        { return static_cast<_Link_type>(_node)->_valptr(); }

        _Self& operator++(){
            _node = _Rb_tree_increment(_node);
            return *this;
        }

        _Self operator++(int){
            _Self __tmp = *this;
            _node = _Rb_tree_increment(_node);
            return __tmp;
        }

        _Self& operator--(){
            _node = _Rb_tree_decrement(_node);
            return *this;
        }

        _Self operator--(int)
        {
            _Self __tmp = *this;
            _node = _Rb_tree_decrement(_node);
            return __tmp;
        }

        friend bool
        operator==(const _Self& __x, const _Self& __y)
        { return __x._node == __y._node; }

        friend bool
        operator!=(const _Self& __x, const _Self& __y)
        { return __x._node != __y._node; }
    };
    
    
    //辅助头节点
    struct _Rb_tree_header
    {
        _Rb_tree_node_base  _header;
        size_t              _node_count; // Keeps track of size of tree.

        _Rb_tree_header(){
            _header._color = _red;
            _reset();
        }

        _Rb_tree_header(_Rb_tree_header&& __x){
            if (__x._header._parent != nullptr)
                _move_data(__x);
            else{
                _header._color = _red;
                _reset();
            }
        }

        void _move_data(_Rb_tree_header& __from){
            _header._color = __from._header._color;
            _header._parent = __from._header._parent;
            _header._left = __from._header._left;
            _header._right = __from._header._right;
            _header._parent->_parent = &_header;
            _node_count = __from._node_count;
            __from._reset();
        }

        void _reset(){
            _header._parent = 0;
            _header._left = &_header;
            _header._right = &_header;
            _node_count = 0;
        }
    };

    void
   _Rb_tree_insert_and_rebalance(const bool __insert_left,
                                 _Rb_tree_node_base* __x,
                                 _Rb_tree_node_base* __p,
                                 _Rb_tree_node_base& __header);
 
   _Rb_tree_node_base*
   _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z,
                                _Rb_tree_node_base& __header);


    // 红黑树类
    template<typename Key, typename Value, typename KeyOfValue, 
             typename Compare, typename Alloc = istl::allocator< _Rb_tree_node<Value> >>
    class _Rb_tree 
    {
    public:
        typedef Key                               key_type;
        typedef Value                             value_type;
        typedef value_type*                       pointer;
        typedef const value_type*                 const_pointer;
        typedef value_type&                       reference;
        typedef const value_type&                 const_reference;
        typedef size_t                            size_type;
        typedef ptrdiff_t                         difference_type;
        typedef Alloc                             nodeAllocator;

        typedef _Rb_tree_node_base*               _Base_ptr;
        typedef const _Rb_tree_node_base*         _Const_Base_ptr;
        typedef _Rb_tree_node<Value>*             _Link_type;
        typedef const _Rb_tree_node<Value>*       _Const_Link_type;

    private:
        Compare _key_compare;        // 键比较函数
        _Rb_tree_header _impl;

        // 结点分配相关
        _Link_type _create_node(const Value& __value) {
            _Link_type __tmp = nodeAllocator::allocate(1);
            nodeAllocator::construct(__tmp, __value);
            return __tmp;
        }

        void _destroy_node(_Link_type __p) {
            nodeAllocator::destroy(__p);
        }

        void _drop_node(_Link_type __p){
            nodeAllocator::destroy(__p);
            nodeAllocator::deallocate( __p, 1);
        }
        _Link_type _clone_node(_Const_Link_type __x)
         {
           _Link_type __tmp = _create_node(*__x->_valptr());
           __tmp->_color = __x->_color;
           __tmp->_left = 0;
           __tmp->_right = 0;
           return __tmp;
         }

         // 内部结点访问
        _Base_ptr& _root(){ return this->_impl._header._parent; }

        _Const_Base_ptr _root()const{ return this->_impl._header._parent; }

        _Base_ptr& _leftmost(){ return this->_impl._header._left; }

        _Const_Base_ptr _leftmost()const{ return this->_impl._header._left; }

        _Base_ptr& _rightmost(){ return this->_impl._header._right; }

        _Const_Base_ptr _rightmost()const { return this->_impl._header._right; }

        _Link_type _begin() { return static_cast<_Link_type>(this->_impl._header._parent); }
 
        _Const_Link_type _begin() const
        {
            return static_cast<_Const_Link_type>(this->_impl._header._parent);
        }
 
        _Base_ptr _end()  { return &this->_impl._header; }
 
        _Const_Base_ptr _end() const { return &this->_impl._header; }

        static const Key _S_key(_Const_Link_type __x)
        {
         // If we're asking for the key we're presumably using the comparison
         // object, and so this is a good place to sanity check it.
         static_assert(std::is_invocable<Compare&, const Key&, const Key&>{},
                       "comparison object must be invocable "
                       "with two arguments of key type");
         return KeyOfValue()(*__x->_valptr());
       }
    
        static _Link_type
        _S_left(_Base_ptr __x)
        { return static_cast<_Link_type>(__x->_left); }

        static _Const_Link_type
        _S_left(_Const_Base_ptr __x)
        { return static_cast<_Const_Link_type>(__x->_left); }

        static _Link_type
        _S_right(_Base_ptr __x)
        { return static_cast<_Link_type>(__x->_right); }

        static _Const_Link_type
        _S_right(_Const_Base_ptr __x)
        { return static_cast<_Const_Link_type>(__x->_right); }

        static const Key
        _S_key(_Const_Base_ptr __x)
        { return _S_key(static_cast<_Const_Link_type>(__x)); }

        static _Base_ptr
        _S_minimum(_Base_ptr __x)
        { return _Rb_tree_node_base::_S_minimum(__x); }

        static _Const_Base_ptr
        _S_minimum(_Const_Base_ptr __x)
        { return _Rb_tree_node_base::_S_minimum(__x); }

        static _Base_ptr
        _S_maximum(_Base_ptr __x)
        { return _Rb_tree_node_base::_S_maximum(__x); }

        static _Const_Base_ptr
        _S_maximum(_Const_Base_ptr __x)
        { return _Rb_tree_node_base::_S_maximum(__x); }
    
    public:
        // iterator/const_iterator
        typedef _Rb_tree_iterator<value_type>               iterator;
        typedef _Rb_tree_const_iterator<value_type>         const_iterator;
        typedef istl::reverse_iterator_t<iterator>          reverse_iterator;
        typedef istl::reverse_iterator_t<const_iterator>    const_reverse_iterator;
 



    public:
        // allocation/deallocation
        _Rb_tree() = default;
        _Rb_tree(const Compare& __comp) : _key_compare(__comp) {}
        _Rb_tree(const _Rb_tree& __x) : _key_compare(__x._key_compare), _impl(__x._impl){
            if(__x._root()!=0) _root() = _copy(__x);
        }
        _Rb_tree(_Rb_tree&&) = default;

        ~_Rb_tree() {
            _erase(_begin());
        }

        // Accessors.
        Compare
        key_comp() const
        { return _key_compare; }

        iterator
        begin()
        { return iterator(this->_impl._header._left); }

        const_iterator
        begin() const
        { return const_iterator(this->_impl._header._left); }

        iterator
        end()
        { return iterator(&this->_impl._header); }

        const_iterator
        end() const
        { return const_iterator(&this->_impl._header); }

        reverse_iterator
        rbegin()
        { return reverse_iterator(end()); }

        const_reverse_iterator
        rbegin() const
        { return const_reverse_iterator(end()); }

        reverse_iterator
        rend()
        { return reverse_iterator(begin()); }

        const_reverse_iterator
        rend() const
        { return const_reverse_iterator(begin()); }

        bool
        empty() const
        { return _impl._node_count == 0; }

        size_type
        size() const
        { return _impl._node_count; }

        // size_type
        // max_size() const
        // { return 0; }

        // void
        // swap(_Rb_tree& __t)


    private:
        // insert_aux
        std::pair<_Base_ptr, _Base_ptr>
        _get_insert_unique_pos(const key_type& __k);

        std::pair<_Base_ptr, _Base_ptr>
        _get_insert_equal_pos(const key_type& __k);

        // copy_aux
        // todo

        //erase_aux
        void
        _erase_aux(const_iterator __position);

        void
        _erase_aux(const_iterator __first, const_iterator __last);

        void _erase(_Link_type __x); //erase without rebalance


        public:
        // insert
        std::pair<iterator, bool>
        _insert_unique(const value_type& __x);

        iterator
        _insert_equal(const value_type& __x);

        iterator
        _insert_(_Base_ptr __x, _Base_ptr __y, const value_type& __v);


        // insert range
        // todo

        //erase
        iterator
        erase(const_iterator __position)
        {
            assert(__position != end());
            const_iterator __result = __position;
            ++__result;
            _erase_aux(__position);
            return __result._const_cast();
        }

        iterator
        erase(iterator __position)
        {
            assert(__position != end());
            iterator __result = __position;
            ++__result;
            _erase_aux(__position);
            return __result;
        }

        size_type
        erase(const key_type& __x);

        iterator
        erase(const_iterator __first, const_iterator __last)
        {
            _erase_aux(__first, __last);
            return __last._const_cast();
        }

        void clear()
        {
            _erase(_begin());
            _impl._reset();
        }
private:
        iterator
        _lower_bound(_Link_type __x, _Base_ptr __y,
                        const Key& __k);

        const_iterator
        _lower_bound(_Const_Link_type __x, _Const_Base_ptr __y,
                        const Key& __k) const;

        iterator
        _upper_bound(_Link_type __x, _Base_ptr __y,
                        const Key& __k);

        const_iterator
        _upper_bound(_Const_Link_type __x, _Const_Base_ptr __y,
                        const Key& __k) const;

public:
       // find operations.
       iterator
       find(const key_type& __k);
 
       const_iterator
       find(const key_type& __k) const;
 
       size_type
       count(const key_type& __k) const;
 
       iterator
       lower_bound(const key_type& __k)
       { return _lower_bound(_begin(), _end(), __k); }
 
       const_iterator
       lower_bound(const key_type& __k) const
       { return _lower_bound(_begin(), _end(), __k); }
 
       iterator
       upper_bound(const key_type& __k)
       { return _upper_bound(_begin(), _end(), __k); }
 
       const_iterator
       upper_bound(const key_type& __k) const
       { return _upper_bound(_begin(), _end(), __k); }
 
       std::pair<iterator, iterator>
       equal_range(const key_type& __k);
 
       std::pair<const_iterator, const_iterator>
       equal_range(const key_type& __k) const;

    public:
    //debug
        void print_rbtree(){
            _Link_type root = _begin();
            if (root == nullptr) {
                std::cout << "null" << std::endl;
                return;
            }
            std::queue<_Base_ptr> q;
            q.push(root);
            while (!q.empty()) {
                
                
                auto n = q.size();
                for(int i=0; i<n; i++){
                    _Base_ptr node = q.front();
                    q.pop();
                    if (node) {
                        std::cout <<  _S_key(node);
                        if(node->_color == _red) std::cout << "_red" << " ";
                        else std::cout << "_black" << " ";
                        q.push(node->_left);
                        q.push(node->_right);
                    } else {
                        std::cout << "null ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    };

    // insert
    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    std::pair<typename _Rb_tree<Key, Val, KeyOfValue,Compare, Alloc>::_Base_ptr,
              typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_Base_ptr>
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_get_insert_unique_pos(const key_type& __k)
    {
        typedef std::pair<_Base_ptr, _Base_ptr> _Res;
        _Link_type __x = _begin();
        _Base_ptr __y = _end();
        bool __comp = true;
        while (__x != 0){
            __y = __x;
            __comp = _key_compare(__k, _S_key(__x));
            __x = __comp ? _S_left(__x) : _S_right(__x);
        }
        iterator __j = iterator(__y);
        if (__comp)
            {
            if (__j == begin())
                return _Res(__x, __y);
            else
                --__j;
            }
        if (_key_compare(_S_key(__j._node), __k))
            return _Res(__x, __y);
        return _Res(__j._node, 0);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    std::pair<typename _Rb_tree<Key, Val, KeyOfValue,Compare, Alloc>::_Base_ptr,
              typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_Base_ptr>
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_get_insert_equal_pos(const key_type& __k)
    {
        typedef std::pair<_Base_ptr, _Base_ptr> _Res;
        _Link_type __x = _begin();
        _Base_ptr __y = _end();
        while (__x != 0){
            __y = __x;
            __x = _key_compare(__k, _S_key(__x)) ?
                    _S_left(__x) : _S_right(__x);
        }
        return _Res(__x, __y);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    std::pair<typename _Rb_tree<Key, Val, KeyOfValue,Compare, Alloc>::iterator, bool>
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_insert_unique(const Val& __x)
    {
        typedef std::pair<iterator, bool> _Res;
        std::pair<_Base_ptr, _Base_ptr> __res = _get_insert_unique_pos(KeyOfValue()(__x));
 
       if (__res.second)
         {
           return _Res(_insert_(__res.first, __res.second,__x), true);
         }
 
       return _Res(iterator(__res.first), false);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue,Compare, Alloc>::iterator
    _Rb_tree<Key, Val, KeyOfValue,Compare, Alloc>::_insert_equal(const Val &__x)
    {
        std::pair<_Base_ptr, _Base_ptr> __res
         = _get_insert_equal_pos(KeyOfValue()(__x));
       return _insert_(__res.first, __res.second, __x);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_insert_(_Base_ptr __x, _Base_ptr __p, const Val& __v){
        bool __insert_left = (__x != 0 || __p == _end()
                               || _key_compare(KeyOfValue()(__v),
                                                         _S_key(__p)));
        _Link_type __z = _create_node(__v);
        _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,
                                       this->_impl._header);
         ++_impl._node_count;
         return iterator(__z);
    }

    // erase
    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    void
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_erase_aux(const_iterator __position)
     {
       _Link_type __y = static_cast<_Link_type>(_Rb_tree_rebalance_for_erase
                                                (const_cast<_Base_ptr>(__position._node),
                                                this->_impl._header));
       _drop_node(__y);
       --_impl._node_count;
     }
 
    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    void
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_erase_aux(const_iterator __first, const_iterator __last)
     {
       if (__first == begin() && __last == end())
         clear();
       else
         while (__first != __last)
           _erase_aux(__first++);
     }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    void
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_erase(_Link_type __x)
    {   // Erase without rebalancing.
        while (__x != 0)
        {
            _erase(_S_right(__x));
            _Link_type __y = _S_left(__x);
            _drop_node(__x);
            __x = __y;
        }
    }
 
    template<typename Key, typename Val, typename KeyOfValue,typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::size_type
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::erase(const Key& __x)
    {
        std::pair<iterator, iterator> __p = equal_range(__x);
        const size_type __old_size = size();
        _erase_aux(__p.first, __p.second);
        return __old_size - size();
    }

    // find operators
    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_lower_bound(_Link_type __x, _Base_ptr __y, const Key& __k)
    {
        while (__x != 0)
         if (!_key_compare(_S_key(__x), __k))
           __y = __x, __x = _S_left(__x);
         else
           __x = _S_right(__x);
       return iterator(__y);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_lower_bound(_Const_Link_type __x, _Const_Base_ptr __y, const Key& __k)const
    {
        while (__x != 0)
         if (!_key_compare(_S_key(__x), __k))
           __y = __x, __x = _S_left(__x);
         else
           __x = _S_right(__x);
       return const_iterator(__y);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_upper_bound(_Link_type __x, _Base_ptr __y, const Key& __k)
    {
       while (__x != 0)
         if (_key_compare(__k, _S_key(__x)))
           __y = __x, __x = _S_left(__x);
         else
           __x = _S_right(__x);
       return iterator(__y);
    }

    
    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_upper_bound(_Const_Link_type __x, _Const_Base_ptr __y, const Key& __k)const
    {
        while (__x != 0)
         if (_key_compare(__k, _S_key(__x)))
           __y = __x, __x = _S_left(__x);
         else
           __x = _S_right(__x);
       return const_iterator(__y);
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::find(const Key& __k)
    {
       iterator __j = _lower_bound(_begin(), _end(), __k);
       return (__j == end()|| _key_compare(__k,_S_key(__j._node))) ? end() : __j;
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::find(const Key& __k)const
    {
       const_iterator __j = _lower_bound(_begin(), _end(), __k);
       return (__j == end()|| _key_compare(__k,_S_key(__j._node))) ? end() : __j;
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    std::pair<typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator,
              typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator>
     _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::equal_range(const Key& __k)
     {
        _Link_type __x = _begin();
        _Base_ptr __y = _end();
        while (__x != 0)
        {
            if (_key_compare(_S_key(__x), __k))
                __x = _S_right(__x);
            else if (_key_compare(__k, _S_key(__x)))
                __y = __x, __x = _S_left(__x);
            else
            {
                _Link_type __xu(__x);
                _Base_ptr __yu(__y);
                __y = __x, __x = _S_left(__x);
                __xu = _S_right(__xu);
                return std::pair<iterator,
                            iterator>(_lower_bound(__x, __y, __k),
                                        _upper_bound(__xu, __yu, __k));
            }
        }
        return std::pair<iterator, iterator>(iterator(__y),
                                       iterator(__y));
     }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    std::pair<typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator,
              typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator>
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::equal_range(const Key& __k)const
    {
        _Const_Link_type __x = _begin();
        _Const_Base_ptr __y = _end();
        while (__x != 0)
        {
            if (_key_compare(_S_key(__x), __k))
                __x = _S_right(__x);
            else if (_key_compare(__k, _S_key(__x)))
                __y = __x, __x = _S_left(__x);
            else
            {
                _Const_Link_type __xu(__x);
                _Const_Base_ptr __yu(__y);
                __y = __x, __x = _S_left(__x);
                __xu = _S_right(__xu);
                return std::pair<const_iterator,
                            const_iterator>(_lower_bound(__x, __y, __k),
                                            _upper_bound(__xu, __yu, __k));
            }
         }
        return std::pair<const_iterator, const_iterator>(const_iterator(__y),
                                                   const_iterator(__y));
    }

    template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
    typename _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::size_type
    _Rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::count(const Key& __k)const{
        std::pair<const_iterator, const_iterator> __p = equal_range(__k);
        const size_type __n = std::distance(__p.first, __p.second);
        return __n;
    }



} // namespace istl



#endif

