#ifndef _ISTL_MAP_H_
#define _ISTL_MAP_H_
#include <algorithm>
#include <stdexcept>
#include "allocator.h"
#include "rbtree.h"

namespace istl
{
    template<typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = istl::allocator<std::pair<const Key, T>>>
    class map
    {
    public:
        typedef Key                                     key_type;
        typedef T                                       mapped_type;
        typedef std::pair<const Key, T>                 value_type;
        typedef Compare                                 key_compare;
        typedef Alloc                                   dataAllocator;

        typedef size_t                                  size_type;

    private:

        struct GetKeyOfVal {
            const Key& operator()(const value_type& p) const {
                return p.first;
            }
        };

        typedef _Rb_tree<key_type, value_type, GetKeyOfVal, Compare>  rbtree;

    private:
        rbtree _tree;

    public:
        typedef _Rb_tree_iterator<value_type>               iterator;
        typedef _Rb_tree_const_iterator<value_type>         const_iterator;
        typedef istl::reverse_iterator_t<iterator>          reverse_iterator;
        typedef istl::reverse_iterator_t<const_iterator>    const_reverse_iterator;

        // allocation/deallocation
        map() = default;
        ~map() = default;

        // element access
        T& operator[] (const key_type& key) { 
            iterator it = (find(key));
            if(it == end()){
                it = insert(std::make_pair(key, mapped_type())).first;
            }
            return it->second;
        }
        T& at(const key_type& key) {
            iterator it = find(key);
            if(it == end()){
                throw std::out_of_range("Key not found");
            }
            return it->second;
        }
        const T& at(const key_type& key)const { 
            const_iterator it = find(key);
            if(it == end()){
                throw std::out_of_range("Key not found");
            }
            return it->second;
        }


        // iterators
        iterator begin() { return _tree.begin(); }
        iterator end() { return _tree.end(); }
        const_iterator begin()const { return _tree.begin(); }
        const_iterator end()const { return _tree.end(); }
        const_iterator cbegin()const { return _tree.begin(); }
        const_iterator cend()const { return _tree.end(); }

        // capacity
        bool empty()const { return _tree.empty(); }
        size_type size()const { return _tree.size(); }
        //max_size

        // modifiers
        void clear() { _tree.clear(); }
        std::pair<iterator, bool> insert(const value_type& val) { return _tree._insert_unique(val); }
        void insert(std::initializer_list<value_type> ilist){
            for(const value_type& value : ilist){
                insert(value);
            }
        }

        iterator erase(iterator pos) { return _tree.erase(pos); }
        iterator erase(iterator first, iterator last) { return _tree.erase(first, last); }
        iterator erase(const_iterator first, const_iterator last) { return _tree.erase(first, last); }
        size_type erase(const key_type& key){ return _tree.erase(key); }

        // lookup
        size_type count(const key_type& key)const { return _tree.count(key); }
        iterator find(const key_type& key) { return _tree.find(key); }
        const_iterator find(const key_type& key)const { return _tree.find(key); }

        std::pair<iterator, iterator> equal_range(const key_type& key) { return _tree.equal_range(key); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type& key)const { return _tree.equal_range(key); }
        iterator lower_bound(const key_type& key) { return _tree.lower_bound(key); }
        const_iterator lower_bound(const key_type& key)const { return _tree.lower_bound(key); }
        iterator upper_bound(const key_type& key) { return _tree.upper_bound(key); }
        const_iterator upper_bound(const key_type& key)const { return _tree.upper_bound(key); }

        // observers
        key_compare key_comp()const { return _tree.key_comp(); }
    
    public://debug
        void printrbt(){ _tree.print_rbtree(); }


    };
    
    
} // namespace istl


#endif