#ifndef _ISTL_RB_TREE_H_
#define _ISTL_RB_TREE_H_
#include <utility>
#include <memory>
#include <iostream>
#include <type_traits>
#include "iterator.h"
#include "allocator.h"

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

        _Val* _valptr(){ return std::__addressof(_value_field); }
        const _Val* _valptr() { return std::__addressof(_value_field); }

        _Rb_tree_node(const _Val& __value) : _M_value_field(__value) {
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
                _M_move_data(__x);
            else{
                _header._color = _red;
                _reset();
            }
        }

        void _M_move_data(_Rb_tree_header& __from){
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

// 透明比较
// #if __cplusplus >= 201402L
//    template<typename _Cmp, typename _SfinaeType, typename = __void_t<>>
//      struct __has_is_transparent
//      { };
 
//    template<typename _Cmp, typename _SfinaeType>
//      struct __has_is_transparent<_Cmp, _SfinaeType,
//                                  __void_t<typename _Cmp::is_transparent>>
//      { typedef void type; };
 
//    template<typename _Cmp, typename _SfinaeType>
//      using __has_is_transparent_t
//        = typename __has_is_transparent<_Cmp, _SfinaeType>::type;
//  #endif

    // 红黑树类
    template<typename Key, typename Value, typename KeyOfValue, 
             typename Compare, typename Alloc = istl::allocator< _Rb_tree_node<KeyOfValue> >>
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
            _Node_traits::deallocate( __p, 1);
        }
        _Link_type _clone_node(_Const_Link_type __x)
         {
           _Link_type __tmp = _create_node(*__x->_valptr());
           __tmp->_color = __x->_color;
           __tmp->_left = 0;
           __tmp->_right = 0;
           return __tmp;
         }

         // 结点访问
        _Base_ptr& _root(){ return this->_impl._header._parent; }

        _Const_Base_ptr _root()const{ return this->_impl._header._parent; }

        _Base_ptr& _M_leftmost(){ return this->_impl._header._left; }

        _Const_Base_ptr _M_leftmost()const{ return this->_impl._header._left; }

        _Base_ptr& _M_rightmost(){ return this->_impl._header._right; }

        _Const_Base_ptr _M_rightmost()const { return this->_impl._header._right; }

        _Link_type _begin() { return static_cast<_Link_type>(this->_M_impl._M_header._M_parent); }
 
       _Const_Link_type _M_begin() const{
         return static_cast<_Const_Link_type>(this->_M_impl._M_header._M_parent);
       }
 
       _Base_ptr _M_end()  { return &this->_M_impl._M_header; }
 
       _Const_Base_ptr _M_end() const { return &this->_M_impl._M_header; }

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

        void clear() {
            // 清空树的实现
        }

        // 插入节点
        std::pair<_Link_type, bool> _M_insert_unique(const Value& __v);

        // 删除节点
        void _M_erase(_Link_type __x);

    protected:

    };


} // namespace istl



#endif

