// #ifndef _VECTOR_H_
// #define _VECTOR_H_

// #include<stddef.h>
// #include<assert.h>
// #include "allocator.h"

// namespace istl
// {
//     template <typename T, class Alloc = allocator<T>>
//     class vector
//     {
//     private:
//         T* _start;
//         T* _finish;
//         T* _endOfStorage;

//         typedef Alloc dataAllocator;
//     public:
//         typedef T                               value_type;
//         typedef T*                              iterator;
//         typedef const T* const_iterator;
//         //typedef reverse_iterator_t<T*>				reverse_iterator;
// 		//typedef reverse_iterator_t<const T*>				const_reverse_iterator;
//         typedef T*                              pointer;
//         typedef const t*                        const_pointer;
//         typedef T&                              reference;
//         typedef const T&                        const_reference;
//         typedef size_t                          size_type;
//         typedef ptrdiff_t                       difference_type;

//         //构造、析构、复制函数
//         vector():_start(nullptr), _finish(nullptr), _endOfStorage(nullptr){}
        
//         explicit vector(const size_type n);
//         vector(const size_type n, const value_type& value);
//         //vector(size_t n, const value_type& val = value_type());
		
//         template<class InputIterator>
// 		vector(InputIterator first, InputIterator last);
        
//         vector(const vector& v);
//         vector(vector&& v);
// 		vector& operator = (const vector& v);
// 		vector& operator = (vector&& v);

//         ~vector();


//         /*迭代器相关*/
//         iterator begin(){ return _start; }
//         iterator end(){ return _finish; }
//         const_iterator begin()const{ return _start; }
//         const_iterator end()const{ return _finish; }

//         /*基本函数*/
//         size_t capacity(){ return _endOfStorage-_start; }
//         size_t size(){ return _finish-_start; }
//         void reserve(size_t n);
//         void resize(size_t n, const value_type& val = value_type());
//         bool empty(){ return _start == _finish; }

//         /*增删改查*/
//         void push_back(const value_type& val);
//         void pop_back();
//         iterator insert(iterator pos, const value_type& val);
//         iterator erase(iterator pos);

//         /*运算符重载*/
//         value_type& operator[](size_t i){ return *(_start+i); }
//         value_type& operator[](size_t i)const{ return *(_start+i); }
//         //比较操作相关
// 		bool operator == (const vector& v)const;
// 		bool operator != (const vector& v)const;
//     };

//     /*构造、析构函数*/
//     template<typename T>
//     vector<T>::vector(const vector& v){
//         reserve(v.capacity());
//         for(size_t i=0; i<v.size(); i++){
//             push_back(v[i]);
//         }
//     }

//     template<typename T>
//     vector<T>::vector(size_t n, const value_type& val){
//         _start = new value_type[n];
//         _endOfStorage = _finish = _start + n;
//         for(size_t i=0; i<n; i++){
//             _start[i] = val;
//         }
//     }
    
//     template<typename T>
//     vector<T>::~vector(){
//         delete[] _start;
//         _start = _finish = _endOfStorage = nullptr;
//     }

//     /*基本函数*/
//     template<typename T>
//     void vector<T>::reserve(size_t n){
//         if(capacity() >= n) return;
//         iterator new_start = new T[n];
//         size_t old_size = size();
//         //元素迁移
//         if(_start != nullptr){
//             for(size_t i=0; i<size(); i++){
//                 new_start[i] = _start[i];
//             }
//         }
//         delete[] _start;
//         _start = new_start;
//         _finish = _start + old_size;
//         _endOfStorage = _start + n;
//     }

//     template<typename T>
//     void vector<T>::resize(size_t n, const value_type& val){
//         if(n<size()){
//             _finish = _start + n;
//         }else{
//             reserve(n);
//             while(_finish != _start+n){
//                 *_finish = val;
//                 ++_finish;
//             }
//         }
//     }

//     template<typename T>
//     void vector<T>::push_back(const value_type& val){
//         if(_finish == _endOfStorage){
//             int newcapacity = capacity() == 0 ? 2 : 2*capacity();
//             reserve(newcapacity);
//         }
//         *_finish = val;
//         ++_finish;
//     }

//     template<typename T>
//     void vector<T>::pop_back(){
//         assert(size()>0);
//         --_finish;
//     }

//     template<typename T>
//     typename vector<T>::iterator vector<T>::insert(iterator pos, const value_type& val){
//         assert(pos>=_start && pos<=_finish);
//         if(_finish == _endOfStorage){
//             size_t site = pos - _start;
//             int newcapacity = capacity() == 0 ? 2 : 2*capacity();
//             reserve(newcapacity);
//             pos = _start + site;
//         }
//         iterator ptr = _finish - 1;
//         while(ptr >= pos){
//             *(ptr+1) = *ptr;
//             ptr--;
//         }
//         *pos = val;
//         ++_finish;
//         return pos;
//     }

//     template<typename T>
//     typename vector<T>::iterator vector<T>::erase(iterator pos){
//         assert(pos>=_start && pos<_finish);
//         iterator ptr = pos+1;
//         while(ptr < _finish){
//             *(ptr-1) = *ptr;
//             ptr++;
//         }
//         --_finish;
//         return pos;
//     }

//     /*运算符重载*/

    

    

// } // namespace istl

// #endif