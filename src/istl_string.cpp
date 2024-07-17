#include "istl_string.h"
#include "uninitialized.h"

namespace istl
{
    
    string::string(const string& str){
        if(str.isSSO()){
            _buffer_size = str._buffer_size;
            strcpy(_buffer, str._buffer);
        }else{
            allocateAndCopy(str._start, str._size);
            setNotSSO();
        }
    }
    
    string::string(string&& str){
        //moveData
        if(str.isSSO()){
            _buffer_size = str._buffer_size;
            strcpy(_buffer, str._buffer); // 使用strcpy复制短字符串，包括'\0'终止符
            // 清空源对象
            str._buffer_size = 0;
            str._buffer[0] = '\0';
        }else{
            _start = str._start;
            _size = str._size;
            _capacity = str._capacity;
            str._start = nullptr;
            str._size = 0;
            str._capacity = 0;
        }
    }

    string::string(const string& str, size_t pos, size_t len){
        len = GetValidLenth(str, pos, len);
        if(len <= _SSO_THRESHOLD){
            _buffer_size = len;
            strncpy(_buffer, str.begin()+pos, len);
            _buffer[len] = '\0';
        }else{
            allocateAndCopy(str._start + pos, len);
            setNotSSO();
        }
    }
    string::string(const char* s){
        size_t len = strlen(s);
        if(len <= _SSO_THRESHOLD){
            _buffer_size = len;
            strncpy(_buffer, s, len);
            _buffer[len] = '\0';
        }else{
            allocateAndCopy(s, len);
            setNotSSO();
        }
    }

    string::string(const char* s, size_t n){
        if(n <= _SSO_THRESHOLD){
            _buffer_size = n;
            strncpy(_buffer, s, n);
            _buffer[n] = '\0';
        }else{
            allocateAndCopy(s, n);
            setNotSSO();
        }
    }

    string::string(size_t n, char c){
        if(n <= _SSO_THRESHOLD){
            _buffer_size = n;
            memset(_buffer, c, n);
            _buffer[n] = '\0';
        }else{
            allocateAndFillN(n, c);
            setNotSSO();
        }
    }

    string::~string(){
        if(isSSO()){
            _buffer_size = 0;
            _buffer[0] = '\0';
        }else{
            destroyAndDeallocate();
        }
    }


    void string::allocateAndFillN(size_t n, char c){
        _start = dataAllocator::allocate(n+1);
        istl::uninitialized_fill_n(_start, n, c);
        _start[n] = '\0';
        _size = _capacity = n;
    }

    void string::destroyAndDeallocate(){
        if(capacity() != 0){
            dataAllocator::destroy(_start, _start + _size + 1);
            dataAllocator::deallocate(_start, capacity() + 1);
            _start = nullptr;
            _size = _capacity = 0;
        }
    }
    
    size_t string::GetValidLenth(const string &str, size_t pos, size_t len)const{
        size_t remaining_length = str.length() - pos;
        if(len == npos || len > remaining_length){
            len = remaining_length;
        }
        return len;
    }

} // namespace istl
