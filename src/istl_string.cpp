#include "istl_string.h"
#include "uninitialized.h"

namespace istl
{
    
    string::string(const string& str){
        // if(str.isSSO()){
        //     _buffer_size = str._buffer_size;
        //     strcpy(_buffer, str._buffer);
        // }else{
        //     allocateAndCopy(str._start, str._size);
        //     setNotSSO();
        // }
        copyData(str.c_str(), str.size());
    }
    
    string::string(string&& str){
        moveData(str);
    }

    string::string(const string& str, size_t pos, size_t len){
        len = GetValidLenth(str, pos, len);
        // if(len <= _SSO_THRESHOLD){
        //     _buffer_size = len;
        //     strncpy(_buffer, str.begin()+pos, len);
        //     _buffer[len] = '\0';
        // }else{
        //     allocateAndCopy(str._start + pos, len);
        //     setNotSSO();
        // }
        copyData(str.c_str()+pos, len);
    }
    string::string(const char* s){
        // size_t len = strlen(s);
        // if(len <= _SSO_THRESHOLD){
        //     _buffer_size = len;
        //     strncpy(_buffer, s, len);
        //     _buffer[len] = '\0';
        // }else{
        //     allocateAndCopy(s, len);
        //     setNotSSO();
        // }
        copyData(s, strlen(s));
    }

    string::string(const char* s, size_t n){
        // if(n <= _SSO_THRESHOLD){
        //     _buffer_size = n;
        //     strncpy(_buffer, s, n);
        //     _buffer[n] = '\0';
        // }else{
        //     allocateAndCopy(s, n);
        //     setNotSSO();
        // }
        copyData(s, n);
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
    
    string& string::operator= (const string& str){
		if (this != &str){
            clearData();
            // if(str.isSSO()){
            //     _buffer_size = str._buffer_size;
            //     strcpy(_buffer, str._buffer);
            // }else{
            //     allocateAndCopy(str._start, str.size());
            //     setNotSSO();
            // }
            copyData(str.c_str(), str.size());
		}
		return *this;
	}

	string& string::operator= (string&& str){
		if (this != &str){
            clearData();
			moveData(str);
		}
		return *this;
	}

    string& string::operator= (const char* s){
		clearData();
		copyData(s, strlen(s));
		return *this;
	}

    string& string::operator= (char c){
		clearData();
		_buffer_size = 1;
        _buffer[0] = c;
        _buffer[1] = '\0';
		return *this;
	}

    string::~string(){
        clearData();
    }

    const char* string::c_str()const{
        if(isSSO()) return _buffer;
        else return _start;
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

    void string::moveData(string& str){
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

    void string::clearData(){
        if(isSSO()){
            _buffer_size = 0;
            _buffer[0] = '\0';
        }else{
            destroyAndDeallocate();
        }
    }

    void string::copyData(const char* src, size_t len){
        if(len <= _SSO_THRESHOLD){
            _buffer_size = len;
            strncpy(_buffer, src, len);
            _buffer[len] = '\0';
        }else{
            allocateAndCopy(src, len);
            setNotSSO();
        }
    }

} // namespace istl
