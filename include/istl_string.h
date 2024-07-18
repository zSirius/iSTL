#ifndef _ISTL_STRING_H_
#define _ISTL_STRING_H_

// #include "ReverseIterator.h"
// #include "allocator.h"
// #include "uninitialized.h"
#include <cstring>
#include <utility>
#include <cstddef>
#include <iostream>
#include "allocator.h"
#include "ReverseIterator.h"
#include "uninitialized.h"

#define _SSO_THRESHOLD 22
namespace istl
{
    class string
    {
    public:
        typedef char  value_type;
        typedef char* iterator;
        typedef const char *    const_iterator;
        typedef reverse_iterator_t<char*> reverse_iterator;
        typedef reverse_iterator_t<const char*> const_reverse_iterator;
        typedef char&            reference;
        typedef const char&        const_reference;
        typedef size_t            size_type;
        typedef ptrdiff_t        difference_type;
        //npos is a static member constant value with the greatest possible value for an element of type size_t.
        static const size_t npos = -1;

    private:
        //little endian and 64bit system implement
        union {
            struct {
                unsigned char _buffer_size; // 第一个字节，用于存储短字符串长度（7位）和标志位（1位）
                char _buffer[_SSO_THRESHOLD+1]; // 短字符串存储区
            };
            struct {
                size_t _capacity; // 长字符串的容量
                size_t _size; // 长字符串的长度
                char* _start; // 长字符串的起始指针
            };
        };
        typedef istl::allocator<char> dataAllocator;

        bool isSSO() const {
            return (_buffer_size & 0x1) == 0; 
            // 区分长字符串和短字符串, 0为短字符串
            // 系统相关, 小端序系统中使用最低位 大端序需要使用最高位
        }

        void setNotSSO() {
            _capacity = (_capacity << 1) | 0x1;
        }

        void setSize(size_t newsize){
            if(isSSO()) _buffer_size = newsize << 1;
            else _size = newsize;
        }

    public:
        /* 构造 赋值 析构*/
        string():_capacity(0), _size(0), _start(nullptr){}
        string(const string& str);
        string(string&& str);
        string(const string& str, size_t pos, size_t len = npos);
        string(const char* s);
        string(const char* s, size_t n);
        string(size_t n, char ch);
        template <typename InputIterator>
        string(InputIterator first, InputIterator last);

        string& operator= (const string& str);
        string& operator= (string&& str);
        string& operator= (const char* s);
        string& operator= (char ch);

        ~string();


        /* 迭代器 容量相关*/
        iterator begin(){ return isSSO() ? &_buffer[0] : _start; }
        const_iterator begin() const{ return isSSO() ? &_buffer[0] : _start; }
        iterator end(){ return isSSO() ? &_buffer[0]+(_buffer_size >> 1) : _start + _size; }
        const_iterator end() const{ return isSSO() ? &_buffer[0]+(_buffer_size >> 1) : _start + _size ; }
        reverse_iterator rbegin(){ return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const{ return const_reverse_iterator(end()); }
        reverse_iterator rend(){ return reverse_iterator(begin()); }
        const_reverse_iterator rend() const{ return const_reverse_iterator(begin()); }
        const_iterator cbegin() const{ return begin(); }
        const_iterator cend() const{ return end(); }
        const_reverse_iterator crbegin() const{ return const_reverse_iterator(end()); }
        const_reverse_iterator crend() const{ return const_reverse_iterator(begin()); }
        size_t size() const{ return isSSO() ? _buffer_size >> 1 : _size; }
        size_t length() const{ return size(); }
        size_t capacity() const{ return isSSO() ? _SSO_THRESHOLD : (_capacity >> 1); } //64bit小端系统

        // 容量相关
        void clear(){ clearData(); }
        bool empty() const{ return size() == 0; }
        void resize(size_t n);
		void resize(size_t n, char ch);
		void reserve(size_t n = 0);

        void shrink_to_fit();


        char& operator[] (size_t pos){ return *(begin() + pos); }
		const char& operator[] (size_t pos) const{ return *(begin() + pos); }
		char& back(){ return *(end() - 1); }
		const char& back() const{ return *(end() - 1); }
		char& front(){ return *(begin()); }
		const char& front() const{ return *(begin()); }

        //get "const char *" 
        const char* c_str()const;
        //const char* data()const;

        //insert
        string& insert(size_t index, const string& str);
		string& insert(size_t index, const string& str, size_t subindex, size_t sublen = npos);
		string& insert(size_t index, const char* s);
		string& insert(size_t index, const char* s, size_t n);
		string& insert(size_t index, size_t n, char ch);
		iterator insert(iterator pos, size_t n, char ch);
		iterator insert(iterator pos, char ch);
        template <class InputIterator>
		iterator insert(iterator pos, InputIterator first, InputIterator last);

        string& operator+= (const string& str);
		string& operator+= (const char* s);
		string& operator+= (char ch);

        string& append(const string& str);
		string& append(const string& str, size_t subpos, size_t sublen = npos);
		string& append(const char* s);
		string& append(const char* s, size_t n);
		string& append(size_t n, char ch);
		template <class InputIterator>
		string& append(InputIterator first, InputIterator last);

        string& erase(size_t pos = 0, size_t len = npos);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

        void push_back(char ch){ insert(end(), ch); }
        void pop_back(){ erase(end()-1, end()); }

        string& replace(size_t pos, size_t len, const string& str);
		string& replace(iterator i1, iterator i2, const string& str);
		string& replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen = npos);
		string& replace(size_t pos, size_t len, const char* s);
		string& replace(iterator i1, iterator i2, const char* s);
		string& replace(size_t pos, size_t len, const char* s, size_t n);
		string& replace(iterator i1, iterator i2, const char* s, size_t n);
		string& replace(size_t pos, size_t len, size_t n, char ch);
		string& replace(iterator i1, iterator i2, size_t n, char ch);
		template <class InputIterator>
		string& replace(iterator i1, iterator i2, InputIterator first, InputIterator last);

        void swap(string &str);
        //size_t copy()...

        //find
        size_t find(const string& str, size_t pos = 0) const;
		size_t find(const char* s, size_t pos = 0) const;
		size_t find(const char* s, size_t pos, size_t n) const;
		size_t find(char ch, size_t pos = 0) const;
        /* https://en.cppreference.com/w/cpp/string/basic_string/rfind */
        size_t rfind(const string& str, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos = npos) const;
		size_t rfind(const char* s, size_t pos, size_t n) const;
		size_t rfind(char ch, size_t pos = npos) const;
        /* https://en.cppreference.com/w/cpp/string/basic_string/find_first_of */
        size_t find_first_of(const string& str, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos = 0) const;
		size_t find_first_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_of(char ch, size_t pos = 0) const;

        size_t find_last_of(const string& str, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos = npos) const;
		size_t find_last_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_of(char ch, size_t pos = npos) const;

        size_t find_first_not_of(const string& str, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos = 0) const;
		size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_first_not_of(char ch, size_t pos = 0) const;

		size_t find_last_not_of(const string& str, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos = npos) const;
		size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
		size_t find_last_not_of(char ch, size_t pos = npos) const;

        //compare
        int compare(const string& str) const;
		int compare(size_t pos, size_t len, const string& str) const;
		int compare(size_t pos, size_t len, const string& str,
			size_t subpos, size_t sublen = npos) const;
		int compare(const char* s) const;
		int compare(size_t pos, size_t len, const char* s) const;
		int compare(size_t pos, size_t len, const char* s, size_t n) const;



        //运算符重载
        friend std::ostream& operator <<(std::ostream& os, const string&str);
		friend std::istream& operator >> (std::istream& is, string& str);
		friend string operator+ (const string& lhs, const string& rhs);
		friend string operator+ (const string& lhs, const char* rhs);
		friend string operator+ (const char* lhs, const string& rhs);
		friend string operator+ (const string& lhs, char rhs);
		friend string operator+ (char lhs, const string& rhs);
		friend bool operator== (const string& lhs, const string& rhs);
		friend bool operator== (const char*   lhs, const string& rhs);
		friend bool operator== (const string& lhs, const char*   rhs);
		friend bool operator!= (const string& lhs, const string& rhs);
		friend bool operator!= (const char*   lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const char*   rhs);
		friend bool operator<  (const string& lhs, const string& rhs);
		friend bool operator<  (const char*   lhs, const string& rhs);
		friend bool operator<  (const string& lhs, const char*   rhs);
		friend bool operator<= (const string& lhs, const string& rhs);
		friend bool operator<= (const char*   lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const char*   rhs);
		friend bool operator>  (const string& lhs, const string& rhs);
		friend bool operator>  (const char*   lhs, const string& rhs);
		friend bool operator>  (const string& lhs, const char*   rhs);
		friend bool operator>= (const string& lhs, const string& rhs);
		friend bool operator>= (const char*   lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const char*   rhs);
		// friend void swap(string& x, string& y);
		// friend std::istream& getline(std::istream& is, string& str, char delim);
		// friend std::istream& getline(std::istream& is, string& str);

    private:
    	template<typename InputIterator>
	    void string_aux(InputIterator first, InputIterator last, std::false_type);

        template<typename InputIterator>
        void allocateAndCopy(InputIterator it, size_t n);
        void allocateAndFillN(size_t n, char ch);
        void destroyAndDeallocate();
        size_t GetValidLenth(const string &str, size_t pos, size_t len)const;
        void moveData(string &str);
        void clearData();
        void copyData(const char* src, size_t len);
        size_type getNewCapacity(size_type len)const;
    
    };


    template<typename InputIterator>
    void string::string_aux(InputIterator first, InputIterator last, std::false_type){
        difference_type n = last - first;
        if(n <= _SSO_THRESHOLD){
            _buffer_size = n;
            strncpy(_buffer, first, n);
            _buffer[n] = '\0';
        }else{
            allocateAndCopy(first, last - first);
            setNotSSO();
        }
    }

    template<typename InputIterator>
    string::string(InputIterator first, InputIterator last){
        string_aux(first, last, typename std::is_integral<InputIterator>::type());
    }

    template<typename InputIterator>
    void string::allocateAndCopy(InputIterator it, size_t n){
        InputIterator first = it;
        InputIterator last = first + n;
        difference_type needCapacity = (last - first + 1 + 15) & (~15);
        _start = dataAllocator::allocate(needCapacity);
        iterator finish = istl::uninitialized_copy(first, last, _start);
        _size = n; //不包含'\0'
        _start[n] = '\0';
        _capacity = needCapacity - 1;
    }

	template <typename InputIterator>
	typename string::iterator 
	string::insert(iterator pos, InputIterator first, InputIterator last){
       	difference_type len = pos - begin();
        if(first == last) return pos;
        difference_type SpaceLeft = capacity() - size();
        difference_type SpaceNeed = last - first;
        

        if(SpaceLeft >= SpaceNeed){
            memmove(pos+SpaceNeed, pos, size()-len+1);
            memmove(pos, first, SpaceNeed);
            setSize(size() + SpaceNeed);
        }else{
            size_t oldsize = size();
            difference_type newCapacity = getNewCapacity(size()+SpaceNeed);
            iterator newstart = dataAllocator::allocate(newCapacity);

            memmove(newstart, begin(), len);
            memmove(newstart+len, first, SpaceNeed);
            memmove(newstart+len+SpaceNeed, pos, size()-len+1);
            
            if(!isSSO()){
                destroyAndDeallocate();
            }
            
            _start = newstart;
            _size = oldsize + SpaceNeed;
            _capacity = newCapacity-1;
            setNotSSO();
        }
        return begin()+len;
	}

    template <class InputIterator>
	string& string::append(InputIterator first, InputIterator last){
        insert(end(), first, last);
        return *this;
    }

    template <class InputIterator>
    string& string::replace(iterator i1, iterator i2, InputIterator first, InputIterator last){
        iterator ptr = erase(i1, i2);
        insert(ptr, first, last);
        return *this;
    }


} // namespace istl


#endif