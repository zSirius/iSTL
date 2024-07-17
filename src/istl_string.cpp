#include "istl_string.h"
#include "uninitialized.h"
#include <iostream>

namespace istl
{
    
    string::string(const string& str){
        copyData(str.c_str(), str.size());
    }
    
    string::string(string&& str){
        moveData(str);
    }

    string::string(const string& str, size_t pos, size_t len){
        len = GetValidLenth(str, pos, len);
        copyData(str.c_str()+pos, len);
    }
    string::string(const char* s){
        copyData(s, strlen(s));
    }

    string::string(const char* s, size_t n){
        copyData(s, n);
    }

    string::string(size_t n, char c){
        if(n <= _SSO_THRESHOLD){
            _buffer_size = n << 1;
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

    string& string::operator= (char ch){
		clearData();
		_buffer_size = 1 << 1;
        _buffer[0] = ch;
        _buffer[1] = '\0';
		return *this;
	}

    string::~string(){
        clearData();
    }

    void string::resize(size_t n){
        resize(n, value_type());
    }

    void string::resize(size_t n, char ch){
        size_t size_ = size();
        //std::cout << "n = " << n << " size_ = " << size_<< std::endl;
        if(isSSO() && n <= _SSO_THRESHOLD){
            _buffer_size = n << 1;
            _buffer[n] = '\0';
            if(n > size_){
                memset(_buffer + size_, ch, n - size_);
            }
        }else{
            if (n < size_){
                dataAllocator::destroy(_start + n, _start + size_);
                _size = n;
                _start[n] = '\0';
            }
            else if (n > size_ && n <= capacity()){
                auto lengthOfInsert = n - size_;
                istl::uninitialized_fill_n(_start + size_, lengthOfInsert, ch);
                _size = n;
                _start[n] = '\0';
            }else if (n > capacity()){
                auto lengthOfInsert = n - size_;
                size_t newCapacity = getNewCapacity(n);
                //std::cout << " newcap = " << newCapacity<< std::endl;
                iterator newStart = dataAllocator::allocate(newCapacity);
                iterator newFinish = istl::uninitialized_copy(begin(), end(), newStart);
                newFinish = istl::uninitialized_fill_n(newFinish, lengthOfInsert, ch);

                destroyAndDeallocate();
                _start = newStart;
                _size = n;
                _capacity = newCapacity - 1;
                setNotSSO();
            }
        }
    }

    void string::reserve(size_t n){
		if (n <= capacity()) return;
		iterator newStart = dataAllocator::allocate(n+1);
		iterator newFinish = istl::uninitialized_move(begin(), end()+1, newStart);
		destroyAndDeallocate();
		_start = newStart; 
		_capacity = n;
        setNotSSO();
	}

    void string::shrink_to_fit(){
        if(!isSSO() && capacity() > size()){
            iterator newstart = dataAllocator::allocate(size()+1);
            iterator newfinish = istl::uninitialized_move(_start, _start + _size + 1, newstart);
            destroyAndDeallocate();
            _start = newstart;
            _capacity = _size;
            setNotSSO();
        }
    }

    const char* string::c_str()const{
        if(isSSO()) return _buffer;
        else return _start;
    }

    /* insert */
    string& string::insert(size_t index, const string& str){
		insert(_start + index, str.begin(), str.end());
		return *this;
    }

    string& string::insert(size_t index, const string& str, size_t subindex, size_t sublen){
		sublen = GetValidLenth(str, subindex, sublen);
		insert(begin() + index, str.begin() + subindex, str.begin() + subindex + sublen);
		return *this;
	}

    string& string::insert(size_t index, const char* s){
		insert(begin() + index, s, s + strlen(s));
		return *this;
	}

    string& string::insert(size_t index, const char* s, size_t n){
		insert(begin() + index, s, s + n);
		return *this;
	}

    string& string::insert(size_t index, size_t n, char ch){
		insert(begin() + index, n, ch);
		return *this;
	}
    string::iterator string::insert(iterator pos, size_t count, char ch){
        difference_type len = pos - begin();
        if(count == 0) return pos;
        difference_type SpaceLeft = capacity() - size();
        difference_type SpaceNeed = count;

        if(SpaceLeft >= SpaceNeed){
            memmove(pos+count, pos, size()-len+1);
            istl::uninitialized_fill_n(pos, count, ch);
            setSize(size()+count);
        }else{
            size_t oldsize = size();
            difference_type newCapacity = getNewCapacity(capacity() + count);
            iterator newstart = dataAllocator::allocate(newCapacity);

            memmove(newstart, begin(), len);
            istl::uninitialized_fill_n(newstart+len, count, ch);
            memmove(newstart+len+SpaceNeed, pos, size()-len+1);

            if(!isSSO()){
                destroyAndDeallocate();
            }
            _start = newstart;
            _size = oldsize + count;
            _capacity = newCapacity-1;
            setNotSSO();
        }
        return begin() + len;
	}

    string::iterator string::insert(iterator p, char ch){
		return insert(p, 1, ch);
	}

    //运算符重载
    std::ostream& operator <<(std::ostream& os, const string& str){
		for (const auto ch : str){
			os << ch;
		}
		return os;
	}



    void string::allocateAndFillN(size_t n, char ch){
        _start = dataAllocator::allocate(n+1);
        istl::uninitialized_fill_n(_start, n, ch);
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
            _buffer_size = len << 1;
            strncpy(_buffer, src, len);
            _buffer[len] = '\0';
        }else{
            allocateAndCopy(src, len);
            setNotSSO();
        }
    }

    string::size_type string::getNewCapacity(size_type needCapacity)const{
        /*
        内存分配策略
        构造函数初始化时：
        if needCapacity > _SSO_THREHOLD: 分配needCapacity + 16
        else 在栈上(_buffer)分配 - SSO技术
        内存扩张策略:
        if needCapacity > _SSO_THREHOLD && needCapacity < 2*Capacity:
            newCapacity = 2 * Capacity
        else if needCapacity > 2*Capacity
            newCapacity = (needCapacity + 15) & (~15);
        */
		size_type oldCapacity = capacity() + 1;
        size_type newCapacity = needCapacity < (2*oldCapacity) ? (2*oldCapacity) : ((needCapacity + 15) & (~15));
		return newCapacity;
	}

} // namespace istl
