// #ifndef _ISTL_STRING_H_
// #define _ISTL_STRING_H_
// #include "ReverseIterator.h"
// #include "allocator.h"
// #include "uninitialized.h"
// #include <utility>
// #include <cstring>

// namespace istl
// {
//     class string
//     {
//     public:
//         typedef char  value_type;
//         typedef char* iterator;
//         typedef const char *    const_iterator;
//         typedef reverse_iterator_t<char*> reverse_iterator;
//         typedef reverse_iterator_t<const char*> const_reverse_iterator;
//         typedef char&            reference;
//         typedef const char&        const_reference;
//         typedef size_t            size_type;
//         typedef ptrdiff_t        difference_type;
//         //npos is a static member constant value with the greatest possible value for an element of type size_t.
//         static const size_t npos = -1;

//     private:
//         char *start_;
//         char *finish_;
//         char *endOfStorage_;
//         typedef istl::allocator<char> dataAllocator;

//     public:
//         /* 构造 赋值 析构*/
//         string() :start_(nullptr), finish_(nullptr), endOfStorage_(nullptr){}
//         string(const string& str);
//         string(string&& str);
//         string(const string& str, size_t pos, size_t len = npos);
//         string(const char* s);
//         string(const char* s, size_t n);
//         string(size_t n, char c);
//         template <class InputIterator>
//         string(InputIterator first, InputIterator last);

//         string& operator= (const string& str);
//         string& operator= (string&& str);
//         string& operator= (const char* s);
//         string& operator= (char c);

//         ~string();

//         /* 迭代器 容量相关*/
//         iterator begin(){ return start_; }
//         const_iterator begin() const{ return start_; }
//         iterator end(){ return finish_; }
//         const_iterator end() const{ return finish_; }
//         reverse_iterator rbegin(){ return reverse_iterator(finish_); }
//         const_reverse_iterator rbegin() const{ return const_reverse_iterator(finish_); }
//         reverse_iterator rend(){ return reverse_iterator(start_); }
//         const_reverse_iterator rend() const{ return const_reverse_iterator(start_); }
//         const_iterator cbegin() const{ return start_; }
//         const_iterator cend() const{ return finish_; }
//         const_reverse_iterator crbegin() const{ return const_reverse_iterator(finish_); }
//         const_reverse_iterator crend() const{ return const_reverse_iterator(start_); }
//         size_t size() const{ return finish_ - start_; }
//         size_t length() const{ return size(); }
//         size_t capacity() const{ return endOfStorage_ - start_; }

//         void clear(){
//             dataAllocator::destroy(start_, finish_);
//             // ？？start_ = finish_;
//             finish_ = start_;
//         }

//         bool empty() const{ return begin() == end(); }
//         void resize(size_t n);
// 		void resize(size_t n, char c);
// 		void reserve(size_t n = 0);

//         void shrink_to_fit();


//         char& operator[] (size_t pos){ return *(start_ + pos); }
// 		const char& operator[] (size_t pos) const{ return *(start_ + pos); }
// 		char& back(){ return *(finish_ - 1); }
// 		const char& back() const{ return *(finish_ - 1); }
// 		char& front(){ return *(start_); }
// 		const char& front() const{ return *(start_); }

//         string& insert(size_t index, const string& str);
// 		string& insert(size_t index, const string& str, size_t subindex, size_t sublen = npos);
// 		string& insert(size_t index, const char* s);
// 		string& insert(size_t index, const char* s, size_t n);
// 		string& insert(size_t index, size_t n, char c);
// 		iterator insert(iterator pos, size_t n, char c);
// 		iterator insert(iterator pos, char c);
//         template <class InputIterator>
// 		iterator insert(iterator pos, InputIterator first, InputIterator last);

//         void push_back(char c){ insert(end(), c); }

//         string& append(const string& str);
// 		string& append(const string& str, size_t subpos, size_t sublen = npos);
// 		string& append(const char* s);
// 		string& append(const char* s, size_t n);
// 		string& append(size_t n, char c);
// 		template <class InputIterator>
// 		string& append(InputIterator first, InputIterator last);
// 		string& operator+= (const string& str);
// 		string& operator+= (const char* s);
// 		string& operator+= (char c);

//         string& erase(size_t pos = 0, size_t len = npos);
// 		iterator erase(iterator p);
// 		iterator erase(iterator first, iterator last);

//         void pop_back(){ erase(end() - 1, end()); }

//         string& replace(size_t pos, size_t len, const string& str);
// 		string& replace(iterator i1, iterator i2, const string& str);
// 		string& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen = npos);
// 		string& replace(size_t pos, size_t len, const char* s);
// 		string& replace(iterator i1, iterator i2, const char* s);
// 		string& replace(size_t pos, size_t len, const char* s, size_t n);
// 		string& replace(iterator i1, iterator i2, const char* s, size_t n);
// 		string& replace(size_t pos, size_t len, size_t n, char c);
// 		string& replace(iterator i1, iterator i2, size_t n, char c);
// 		template <class InputIterator>
// 		string& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);

//         void swap(string& str){
// 			std::swap(start_, str.start_);
// 			std::swap(finish_, str.finish_);
// 			std::swap(endOfStorage_, str.endOfStorage_);
// 		}

//         size_t copy(char* dest, size_t len, size_t pos = 0) const{
// 			auto ptr = istl::uninitialized_copy(begin() + pos, begin() + pos + len, dest);
// 			return (size_t)(ptr - dest);
// 		}

//         size_t find(const string& str, size_t pos = 0) const;
// 		size_t find(const char* s, size_t pos = 0) const;
// 		size_t find(const char* s, size_t pos, size_t n) const;
// 		size_t find(char c, size_t pos = 0) const;
// 		size_t rfind(const string& str, size_t pos = npos) const;
// 		size_t rfind(const char* s, size_t pos = npos) const;
// 		size_t rfind(const char* s, size_t pos, size_t n) const;
// 		size_t rfind(char c, size_t pos = npos) const;
// 		size_t find_first_of(const string& str, size_t pos = 0) const;
// 		size_t find_first_of(const char* s, size_t pos = 0) const;
// 		size_t find_first_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_first_of(char c, size_t pos = 0) const;
// 		size_t find_last_of(const string& str, size_t pos = npos) const;
// 		size_t find_last_of(const char* s, size_t pos = npos) const;
// 		size_t find_last_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_last_of(char c, size_t pos = npos) const;
// 		size_t find_first_not_of(const string& str, size_t pos = 0) const;
// 		size_t find_first_not_of(const char* s, size_t pos = 0) const;
// 		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_first_not_of(char c, size_t pos = 0) const;
// 		size_t find_last_not_of(const string& str, size_t pos = npos) const;
// 		size_t find_last_not_of(const char* s, size_t pos = npos) const;
// 		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
// 		size_t find_last_not_of(char c, size_t pos = npos) const;


//         int compare(const string& str) const;
// 		int compare(size_t pos, size_t len, const string& str) const;
// 		int compare(size_t pos, size_t len, const string& str,
// 			size_t subpos, size_t sublen = npos) const;
// 		int compare(const char* s) const;
// 		int compare(size_t pos, size_t len, const char* s) const;
// 		int compare(size_t pos, size_t len, const char* s, size_t n) const;

//         string substr(size_t pos = 0, size_t len = npos) const{
//             //len = changeVarWhenEuqalNPOS(len, size(), pos);
//             return string(begin() + pos, begin() + pos + len);
// 		}


//         //运算符重载
//         friend std::ostream& operator <<(std::ostream& os, const string&str);
// 		friend std::istream& operator >> (std::istream& is, string& str);
// 		friend string operator+ (const string& lhs, const string& rhs);
// 		friend string operator+ (const string& lhs, const char* rhs);
// 		friend string operator+ (const char* lhs, const string& rhs);
// 		friend string operator+ (const string& lhs, char rhs);
// 		friend string operator+ (char lhs, const string& rhs);
// 		friend bool operator== (const string& lhs, const string& rhs);
// 		friend bool operator== (const char*   lhs, const string& rhs);
// 		friend bool operator== (const string& lhs, const char*   rhs);
// 		friend bool operator!= (const string& lhs, const string& rhs);
// 		friend bool operator!= (const char*   lhs, const string& rhs);
// 		friend bool operator!= (const string& lhs, const char*   rhs);
// 		friend bool operator<  (const string& lhs, const string& rhs);
// 		friend bool operator<  (const char*   lhs, const string& rhs);
// 		friend bool operator<  (const string& lhs, const char*   rhs);
// 		friend bool operator<= (const string& lhs, const string& rhs);
// 		friend bool operator<= (const char*   lhs, const string& rhs);
// 		friend bool operator<= (const string& lhs, const char*   rhs);
// 		friend bool operator>  (const string& lhs, const string& rhs);
// 		friend bool operator>  (const char*   lhs, const string& rhs);
// 		friend bool operator>  (const string& lhs, const char*   rhs);
// 		friend bool operator>= (const string& lhs, const string& rhs);
// 		friend bool operator>= (const char*   lhs, const string& rhs);
// 		friend bool operator>= (const string& lhs, const char*   rhs);
// 		friend void swap(string& x, string& y);
// 		friend std::istream& getline(std::istream& is, string& str, char delim);
// 		friend std::istream& getline(std::istream& is, string& str);

// 	private:
// 		template<class InputIterator>
// 		void string_aux(InputIterator first, InputIterator last, std::false_type);

// 		void moveData(string& str);
// 		template<class InputIterator>
// 		void allocateAndCopy(InputIterator first, InputIterator last);
// 		void allocateAndFillN(size_t n, char c);
// 		void destroyAndDeallocate();

// 		size_type getNewCapacity(size_type len)const;
// 		size_t changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const;

//     };

// 	template <class InputIterator>
// 	typename string::iterator 
// 	string::insert(iterator pos, InputIterator first, InputIterator last){
//        	difference_type idx = pos - begin();
//         iterator vpos = begin() + idx;
//         if(first == last) return vpos;
//         difference_type SpaceLeft = _endOfStorage - _finish;
//         difference_type SpaceNeed = last - first;
        

//         if(SpaceLeft >= SpaceNeed){
//             if(_finish - pos > SpaceNeed){
//                 istl::uninitialized_copy(_finish - SpaceNeed, _finish, _finish);
//                 std::copy_backward(vpos, _finish-SpaceNeed, _finish);
//                 std::copy(first, last, vpos);
//             }else{
//                 difference_type dif = _finish - vpos;
//                 iterator InsertItTail = first + dif;
//                 iterator tmp = istl::uninitialized_copy(InsertItTail, last, _finish);
//                 istl::uninitialized_copy(vpos, _finish, tmp);
//                 std::copy(first, InsertItTail, vpos);
//             }
//             _finish += SpaceNeed;
//         }else{
//             difference_type newCapacity = getNewCapacity(last - first);
//             T *newstart = dataAllocator::allocate(newCapacity);
//             T *newfinish = istl::uninitialized_move(begin(), vpos, newstart);
//             newfinish = istl::uninitialized_copy(first, last, newfinish);
//             newfinish = istl::uninitialized_move(vpos, end(), newfinish);

//             destroyAndDeallocateAll();
//             _start = newstart;
//             _finish = newfinish;
//             _endOfStorage = _start + newCapacity;
//         }
//         return begin()+idx;
// 	}

// 	template<class InputIterator>
// 	void string::string_aux(InputIterator first, InputIterator last, std::false_type){
// 		allocateAndCopy(first, last);
// 	}

// 	template<class InputIterator>
// 	string::string(InputIterator first, InputIterator last){
// 		string_aux(first, last, typename std::is_integral<InputIterator>::type());
// 	}

// 	template<class InputIterator>
// 	void string::allocateAndCopy(InputIterator first, InputIterator last){
// 		start_ = dataAllocator::allocate(last - first);
// 		finish_ = TinySTL::uninitialized_copy(first, last, start_);
// 		endOfStorage_ = finish_;
// 	}
        
    
// } // namespace istl


// #endif