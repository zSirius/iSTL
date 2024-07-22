#include "rbtree.h"

namespace istl
{
    static _Rb_tree_node_base*
    local_Rb_tree_increment(_Rb_tree_node_base* __x){
        if(__x->_right != nullptr){
            __x = __x->_right;
            while(__x->_left != nullptr){
                __x = __x->_left;
            }
        }else{
            _Rb_tree_node_base* __y = __x->_parent;
            while(__x == __y->_right){
                __x = __y;
                __y = __y->_parent;
            }
            if(__x->_right != __y){
                /*
                when init __x has no right node, have max node is itself, __x->_right == __y:
                  O (help head node)
                  |
                 / 
                6 (root node)
                */
                __x = __y;
            }
        }
        return __x;
    }

    _Rb_tree_node_base* 
    _Rb_tree_increment(_Rb_tree_node_base* __x){
        return local_Rb_tree_increment(__x);
    }

    const _Rb_tree_node_base* 
    _Rb_tree_increment(const _Rb_tree_node_base* __x){
        return local_Rb_tree_increment(const_cast<_Rb_tree_node_base*>(__x));
    }

    static _Rb_tree_node_base*
    local_Rb_tree_decrement(_Rb_tree_node_base* __x){
        if(__x->_color == _red && __x->_parent->_parent == __x){
            __x = __x->_right;
        }else if(__x->_left != nullptr){
            __x = __x->_left;
            while(__x->_right != nullptr){
                __x = __x->_right;
            }
        }else{
            _Rb_tree_node_base* __y = __x->_parent;
            while(__x == __y->_left){
                __x = __y;
                __y = __y->_parent;
            }
            if(__x->_right != __y) __x = __y;
        }
        return __x;
    }

    _Rb_tree_node_base*
    _Rb_tree_decrement(_Rb_tree_node_base* __x){
        return local_Rb_tree_decrement(__x);
    }

    const _Rb_tree_node_base*
    _Rb_tree_decrement(const _Rb_tree_node_base* __x){
        return local_Rb_tree_decrement(const_cast<_Rb_tree_node_base*>(__x));
    }

    static void
    local_Rb_tree_rotate_left(_Rb_tree_node_base* const __x,
		                      _Rb_tree_node_base*& __root)
    {
        _Rb_tree_node_base *const __y = __x->_right;
        
        __x->_right = __y->_left;
        if(__y->_left != 0){
            __y->_left->_parent = __x;
        }
        __y->_parent = __x->_parent;

        if(__x == __root){
            __root = __y;
        }else if(__x == __x->_parent->_left){
            __x->_parent->_left = __y;
        }else{
            __x->_parent->_right = __y;
        }
        __y->_left = __x;
        __x->_parent = __y;
    }

    static void
    local_Rb_tree_rotate_right(_Rb_tree_node_base* const __x,
		                      _Rb_tree_node_base*& __root)
    {
        _Rb_tree_node_base* const __y = __x->_left;

        __x->_left = __y->_right;
        if(__y->_right != 0){
            __y->_right->_parent = __x;
        }
        __y->_parent = __x->_parent;

        if(__x == __root){
            __root = __y;
        }else if(__x == __x->_parent->_left){
            __x->_parent->_left = __y;
        }else{
            __x->_parent->_right = __y;
        }
        __y->_right = __x;
        __x->_parent = __y;
    }

    void
    _Rb_tree_insert_and_rebalance(const bool          __insert_left,
                                  _Rb_tree_node_base* __x,
                                  _Rb_tree_node_base* __p,
                                  _Rb_tree_node_base& __header)
    {
        /*
        __insert_left: whether insert in left
        __x: node to be inserted
        __p: parent
        */
        _Rb_tree_node_base *& __root = __header._parent;
        
        // initialize x
        __x->_parent = __p;
        __x->_left = 0;
        __x->_right = 0;
        __x->_color = _red;

        // insert
        if(__insert_left){
            __p->_left = __x;
            
            if(__p == &__header){ //First node is always inserted left.
                __header._parent = __x;
                __header._right = __x;
            }else if(__p == __header._left){
                __header._left = __x; // maintain leftmost pointing to min node
            }
        }else{
            __p->_right = __x;
            
            if(__p == __header._right){
                __header._right = __x;  // maintain rightmost pointing to max node
            }
        }
        //rebalance
        while( __x != __root && __x->_parent->_color == _red){
            _Rb_tree_node_base* const __xpp = __x->_parent->_parent;

            if(__x->_parent == __xpp->_left){
                _Rb_tree_node_base* const __y = __xpp->_right;
                if(__y && __y->_color == _red){
                    __x->_parent->_color = _black;
                    __y->_color = _black;
                    __xpp->_color = _red;
                    __x = __xpp;

                }else{
                    if(__x == __x->_parent->_right){
                        __x = __x->_parent;
                        local_Rb_tree_rotate_left(__x, __root);
                    }
                    __x->_parent->_color = _black;
                    __xpp->_color = _red;
                    local_Rb_tree_rotate_right(__xpp, __root);
                }
            }else{
                _Rb_tree_node_base* const __y =__xpp->_left;
                if(__y && __y->_color == _red){
                    __x->_parent->_color = _black;
                    __y->_color = _black;
                    __xpp->_color = _red;
                    __x = __xpp;
                }else{
                    if(__x == __x->_parent->_left){
                        __x = __x->_parent;
                        local_Rb_tree_rotate_right(__x, __root);
                    }
                    __x->_parent->_color = _black;
                    __xpp->_color = _red;
                    local_Rb_tree_rotate_left(__xpp, __root);

                }
            }
        }
        __root->_color = _black;
    }

    _Rb_tree_node_base*
    _Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z,
			                     _Rb_tree_node_base& __header)
    {
        // __z is node to erase
        _Rb_tree_node_base *& __root = __header._parent;
        _Rb_tree_node_base *& __leftmost = __header._left;
        _Rb_tree_node_base *& __rightmost = __header._right;
        _Rb_tree_node_base* __y = __z; //__y is successor of __z, is node to delete actually, but __y may == __z
        _Rb_tree_node_base* __x = 0; //__x is __y's child, __y have only one child
        _Rb_tree_node_base* __x_parent = 0;

        if(__y->_left == 0){
            __x = __y->_right;
        }else if(__y->_right == 0){
            __x = __y->_left;
        }else{
            // __z has two non-null children.  
            // Set __y to __z's successor.  __x might be null.
            __y = __z->_right;
            while(__y->_left != 0){
                __y = __y->_left;
            }
            __x = __y->_right;
        }
        if(__y != __z){
            //relink y in place of z.
            __z->_left->_parent = __y;
            __y->_left = __z->_left;
            if(__y != __z->_right){
                __x_parent = __y->_parent;
                if(__x) __x->_parent = __y->_parent;
                __y->_parent->_left = __x;
                __y->_right = __z->_right;
                __z->_right->_parent = __y;
            }else{
                __x_parent = __y;
            }
            if(__root == __z){
                __root = __y;
            }else if(__z->_parent->_left == __z){
                __z->_parent->_left = __y;
            }else{
                __z->_parent->_right = __y;
            }
            __y->_parent = __z->_parent;
            std::swap(__y->_color, __z->_color);
            __y = __z;// __y now points to node to be actually deleted
        }else{ // __y == __z
            __x_parent = __y->_parent;
            if(__x){
                __x->_parent = __y->_parent;
            }
            if(__root == __z){
                __root = __x;
            }else if(__z->_parent->_left == __z){
                __z->_parent->_left = __x;
            }else{
                __z->_parent->_right = __x;
            }
            if(__leftmost == __z){
                if(__z->_right == 0) __leftmost = __z->_parent;
                else __leftmost = _Rb_tree_node_base::_S_minimum(__x);
            }
            if(__rightmost == __z){
                if(__z->_left == 0) __rightmost = __z->_parent;
                else __rightmost = _Rb_tree_node_base::_S_maximum(__x);
            }
        }
        // if __y->color == _black, delete it directly
        // if __y->color == _red, need to be adjusted for sibling
        if(__y->_color != _red){
            while(__x != __root && (__x == 0 || __x->_color == _black)){
                if(__x == __x_parent->_left){
                    _Rb_tree_node_base* __w = __x_parent->_right; //sibling
                    if(__w->_color == _red){
                        __w->_color = _black;
                        __x_parent->_color = _red;
                        local_Rb_tree_rotate_right(__x_parent, __root);
                        __w = __x_parent->_right;
                    }

                    if((__w->_left == 0 || __w->_left->_color == _black)&&
                    (__w->_right == 0 || __w->_right->_color == _black)){
                        __w->_color = _red;
                        __x = __x_parent;
                        __x_parent = __x_parent->_parent;
                    }else{
                        if(__w->_right == 0 || __w->_right->_color == _black){
                            __w->_left->_color = _black;
                            __w->_color = _red;
                            local_Rb_tree_rotate_right(__w, __root);
                            __w = __x_parent->_right;
                        }
                        __w->_color = __x_parent->_color;
                        __x_parent->_color = _black;
                        if(__w->_right) __w->_right->_color = _black;
                        local_Rb_tree_rotate_left(__x_parent, __root);
                        break;
                    }
                }else{
                    _Rb_tree_node_base* __w = __x_parent->_left;
	                if(__w->_color == _red){
                        __w->_color = _black;
                        __x_parent->_color = _red;
                        local_Rb_tree_rotate_right(__x_parent, __root);
                        __w = __x_parent->_left;
                    }
                    if((__w->_right == 0 || __w->_right->_color == _black)&&
                       (__w->_left == 0 ||__w->_left->_color == _black)){
                        __w->_color = _red;
                        __x = __x_parent;
                        __x_parent = __x_parent->_parent;
                    }else{
                        if (__w->_left == 0 || __w->_left->_color == _black){
                            __w->_right->_color = _black;
                            __w->_color = _red;
                            local_Rb_tree_rotate_left(__w, __root);
                            __w = __x_parent->_left;
                        }
                        __w->_color = __x_parent->_color;
                        __x_parent->_color = _black;
                        if (__w->_left) __w->_left->_color = _black;
                        local_Rb_tree_rotate_right(__x_parent, __root);
                        break;
                    }
                }
            }
            if(__x) __x->_color = _black;
        }
        return __y;
    }
    
    // unsigned int
    // _Rb_tree_black_count(const _Rb_tree_node_base* __node, const _Rb_tree_node_base* __root){
    //     if(__node == 0) return 0;
    //     unsigned int __sum = 0;
    //     do{
    //         if(__node->_color == _black) ++__sum;
    //         if(__node == __root) break;
    //         __node == __node->_parent;
    //     }while(1);
    //     return __sum;
    // }
    



} // namespace istl
