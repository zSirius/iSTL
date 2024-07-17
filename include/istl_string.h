#ifndef _ISTL_STRING_H_
#define _ISTL_STRING_H_

// #include "ReverseIterator.h"
// #include "allocator.h"
// #include "uninitialized.h"
#include <cstring>
#include <utility>
#include <cstddef>
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

    public://private
        //little endian and 64bit system implement
        union {
            struct {
                unsigned char _buffer_size; // 第一个字节，用于存储短字符串长度（7位）和标志位（1位）
                char _buffer[_SSO_THRESHOLD+1]; // 短字符串存储区
            };
            struct {
                size_t _capacity : 63; // 长字符串的容量
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

    public:
        /* 构造 赋值 析构*/
        string():_capacity(0), _size(0), _start(nullptr){}
        string(const string& str);
        string(string&& str);
        string(const string& str, size_t pos, size_t len = npos);
        string(const char* s);
        string(const char* s, size_t n);
        string(size_t n, char c);
        template <typename InputIterator>
        string(InputIterator first, InputIterator last);

        string& operator= (const string& str);
        string& operator= (string&& str);
        string& operator= (const char* s);
        string& operator= (char c);

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
		void resize(size_t n, char c);
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

    private:
    	template<typename InputIterator>
	    void string_aux(InputIterator first, InputIterator last, std::false_type);

        template<typename InputIterator>
        void allocateAndCopy(InputIterator it, size_t n);
        void allocateAndFillN(size_t n, char c);
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




} // namespace istl


#endif