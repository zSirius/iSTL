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
                if(!isSSO()){
                    destroyAndDeallocate();
                }
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

    // +=
    string& string::operator+= (const string& str){
        insert(size(), str);
        return *this;
    }

    string& string::operator+= (const char* s){
        return insert(size(), s);
        return *this;
    }

    string& string::operator+= (char ch){
        insert(end(), ch);
        return *this;
    }

    string& string::append(const string& str){
        (*this) += str;
        return *this;
    }

    string& string::append(const string& str, size_t subindex, size_t sublen){
        insert(size(), str, subindex, sublen);
        return *this;
    }

    string& string::append(const char* s){
        (*this) += s;
        return *this;
    }

    string& string::append(const char* s, size_t n){
        insert(size(), s, n);
        return *this;
    }

    string& string::append(size_t n, char ch){
        insert(size(),n, ch);
        return *this;
    }

    //erase
    typename string::iterator string::erase(iterator first, iterator last){
        size_t oldsize = size();
        difference_type offset = last - first;
        for(iterator it = last; it <=end(); ++it){
            *(it-offset) = *(it);
        }
        setSize(oldsize - offset);
        return first;
    }

    typename string::iterator string::erase(iterator position){
        return erase(position, position+1);
    }

    string& string::erase(size_t pos, size_t len){
        len = GetValidLenth(*this, pos, len);
        erase(begin()+pos, begin()+pos+len);
        return *this;
    }

    // replace
    string& string::replace(size_t pos, size_t len, const string& str){
        return replace(begin()+pos, begin()+pos+len, str.begin(), str.end());
    }
    string& string::replace(iterator i1, iterator i2, const string& str){
        return replace(i1, i2, str.begin(), str.end());
    }
    string& string::replace(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen){
        sublen = GetValidLenth(str, subpos, sublen);
        return replace(begin()+pos, begin()+pos+len, str.begin()+subpos, str.begin()+subpos+sublen);
    }
    string& string::replace(size_t pos, size_t len, const char* s){
        return replace(begin()+pos, begin()+pos+len, s, s+strlen(s));
    }
    string& string::replace(iterator i1, iterator i2, const char* s){
        return replace(i1, i2, s, s+strlen(s));
    }
    string& string::replace(size_t pos, size_t len, const char* s, size_t n){
        return replace(begin()+pos, begin()+pos+len, s, s+n);
    }
    string& string::replace(iterator i1, iterator i2, const char* s, size_t n){
        return replace(i1, i2, s, s+n);
    }
    string& string::replace(size_t pos, size_t len, size_t n, char ch){
        return replace(begin()+pos, begin()+pos+len, n, ch);
    }
    string& string::replace(iterator i1, iterator i2, size_t n, char ch){
        iterator ptr = erase(i1, i2);
        insert(ptr, n, ch);
        return *this;
    }

    //find
    size_t string::find(const string& str, size_t pos) const {
        return find(str.c_str(), pos, str.size());
    }

    size_t string::find(const char* s, size_t pos) const {
        return find(s, pos, std::strlen(s));
    }
    size_t string::find(const char* s, size_t pos, size_t n) const {
        if (pos > size()) return istl::string::npos;
        const char* h = c_str() + pos;
        size_t remaining_length = size() - pos;

        if (n == 0) return pos;

        while (remaining_length >= n) {
            h = (const char*)std::memchr(h, s[0], remaining_length);
            if (!h) break;
            if (std::memcmp(h, s, n) == 0) {
                return h - c_str();
            }
            h++;
            remaining_length = size() - (h - c_str());
        }
        return istl::string::npos;
    }

    size_t string::find(char ch, size_t pos) const {
        if (pos >= size()) return istl::string::npos;
        const char* h = c_str() + pos;
        const char* result = (const char*)std::memchr(h, ch, size() - pos);
        return result ? result - c_str() : std::string::npos;
    }

    size_t string::rfind(const string& str, size_t pos) const {
        return rfind(str.c_str(), pos, str.size());
    }

    size_t string::rfind(const char* s, size_t pos) const {
        return rfind(s, pos, std::strlen(s));
    }

    size_t string::rfind(const char* s, size_t pos, size_t n) const {
        if (n == 0) return (pos < size()) ? pos : size();
        if (size() < n) return istl::string::npos;

        pos = std::min(pos, size() - n);
        const char* start = c_str();
        const char* p;

        for (p = start + pos; p >= start; --p) {
            if (*p == *s && std::memcmp(p, s, n) == 0) {
                return p - start;
            }
        }
        return std::string::npos;
    }

    size_t string::rfind(char ch, size_t pos) const {
        if (empty()) return istl::string::npos;

        pos = std::min(pos, size() - 1);
        const char* start = c_str();

        for (const char* p = start + pos; p >= start; --p) {
            if (*p == ch) {
                return p - start;
            }
        }
        return std::string::npos;
    }

    size_t string::find_first_of(const string& str, size_t pos) const {
        return find_first_of(str.c_str(), pos, str.size());
    }

    size_t string::find_first_of(const char* s, size_t pos) const {
        return find_first_of(s, pos, std::strlen(s));
    }

    size_t string::find_first_of(const char* s, size_t pos, size_t n) const {
        if (pos >= size()) return istl::string::npos;
        const char *data = c_str();

        for (size_t i = pos; i < size(); ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (data[i] == s[j]) {
                    return i;
                }
            }
        }

        return istl::string::npos;
    }

    size_t string::find_first_of(char ch, size_t pos) const {
        if (pos >= size()) return istl::string::npos;
        const char *data = c_str();

        for (size_t i = pos; i < size(); ++i) {
            if (data[i] == ch) {
                return i;
            }
        }

        return istl::string::npos;
    }

    size_t string::find_last_of(const string& str, size_t pos) const {
        return find_last_of(str.c_str(), pos, str.size());
    }

    size_t string::find_last_of(const char* s, size_t pos) const {
        return find_last_of(s, pos, std::strlen(s));
    }

    size_t string::find_last_of(const char* s, size_t pos, size_t n) const {
        if (empty() || n == 0) return istl::string::npos;
        if (pos >= size()) pos = size() - 1;
        const char *data = c_str();
        
        for (size_t i = pos + 1; i-- > 0; ) {
            for (size_t j = 0; j < n; ++j) {
                if (data[i] == s[j]) {
                    return i;
                }
            }
        }
        return istl::string::npos;
    }

    size_t string::find_last_of(char ch, size_t pos) const {
        if (empty()) return istl::string::npos;
        if (pos >= size()) pos = size() - 1;
        const char *data = c_str();

        for (size_t i = pos + 1; i-- > 0; ) {
            if (data[i] == ch) {
                return i;
            }
        }
        return std::string::npos;
    }

    size_t string::find_first_not_of(const string& str, size_t pos) const {
        return find_first_not_of(str.c_str(), pos, str.size());
    }

    size_t string::find_first_not_of(const char* s, size_t pos) const {
        return find_first_not_of(s, pos, std::strlen(s));
    }

    size_t string::find_first_not_of(const char* s, size_t pos, size_t n) const {
        if (pos >= size()) return istl::string::npos;
        const char *data = c_str();

        for (size_t i = pos; i < size(); ++i) {
            bool found = false;
            for (size_t j = 0; j < n; ++j) {
                if (data[i] == s[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return i;
            }
        }

        return istl::string::npos;
    }

    size_t string::find_first_not_of(char ch, size_t pos) const {
        if (pos >= size()) return istl::string::npos;
        const char *data = c_str();

        for (size_t i = pos; i < size(); ++i) {
            if (data[i] != ch) {
                return i;
            }
        }

        return istl::string::npos;
    }

    size_t string::find_last_not_of(const string& str, size_t pos) const {
        return find_last_not_of(str.c_str(), pos, str.size());
    }

    size_t string::find_last_not_of(const char* s, size_t pos) const {
        return find_last_not_of(s, pos, std::strlen(s));
    }

    size_t string::find_last_not_of(const char* s, size_t pos, size_t n) const {
        if (empty()) return istl::string::npos;
        if (pos >= size()) pos = size() - 1;
        const char *data = c_str();

        for (size_t i = pos + 1; i-- > 0; ) {
            bool found = false;
            for (size_t j = 0; j < n; ++j) {
                if (data[i] == s[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return i;
            }
        }

        return istl::string::npos;
    }

    size_t string::find_last_not_of(char ch, size_t pos) const {
        if (empty()) return std::string::npos;
        if (pos >= size()) pos = size() - 1;
        const char *data = c_str();

        for (size_t i = pos + 1; i-- > 0; ) {
            if (data[i] != ch) {
                return i;
            }
        }

        return istl::string::npos;
    }

    //compare
    int string::compare(const string& str)const{
        return std::strcmp(c_str(), str.c_str());
    }

    int string::compare(size_t pos, size_t len, const string& str)const{
        return compare(pos, len, str, 0, str.size());
    }

    int string::compare(size_t pos, size_t len, const string& str, size_t subpos, size_t sublen) const{
        len = GetValidLenth(*this, pos, len);
        sublen = GetValidLenth(str, subpos, sublen);
        int ret = strncmp(c_str()+pos, str.c_str()+subpos, std::min(len, sublen));
        if(ret == 0){
            if(len > sublen) ret = 1;
            else if(len < sublen) ret = -1;
        }
        return ret;
    }

    int string::compare(const char* s) const{
        return compare(0, size(), s, std::strlen(s));
    }

    int string::compare(size_t pos, size_t len, const char* s) const{
        return compare(pos, len, s, std::strlen(s));
    }

    int string::compare(size_t pos, size_t len, const char* s, size_t n) const{
        len = GetValidLenth(*this, pos, len);
        n = std::min(n, std::strlen(s));
        int ret = strncmp(c_str()+pos, s, std::min(len, n));
        if(ret == 0){
            if(len > n) ret = 1;
            else if(len < n) ret = -1;
        }
        return ret;
    }

    //运算符重载
    std::ostream& operator <<(std::ostream& os, const string& str){
		for (const auto ch : str){
			os << ch;
		}
		return os;
	}

    std::istream& operator >> (std::istream& is, string& str){
        char ch;
        while(is.get(ch)){
            if(!isspace(ch)) break;
        }
        is.putback(ch);
        str.clear();
        while(is.get(ch)){
            if(ch!=EOF && !isspace(ch)){
                str.push_back(ch);
            }else{
                break;;
            }
        }
        return is;
    }

    string operator+ (const string& lhs, const string& rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (const string& lhs, const char* rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (const char* lhs, const string& rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (const string& lhs, char rhs){
		string res(lhs);
		return res += rhs;
	}
	string operator+ (char lhs, const string& rhs){
		string res(1, lhs);
		return res += rhs;
	}

    bool operator== (const string& lhs, const string& rhs){
		if (lhs.size() == rhs.size()){
            int ret = lhs.compare(rhs);
            return ret == 0;
		}
		return false;
	}

    bool operator== (const char*   lhs, const string& rhs){
		if(std::strlen(lhs) == rhs.size()){
            int ret = rhs.compare(lhs);
            return ret == 0;
        }
        return false;
	}

    bool operator== (const string& lhs, const char*   rhs){
        return rhs == lhs;
	}
    bool operator!= (const string& lhs, const string& rhs){
		return !(lhs == rhs);
	}
    bool operator!= (const char*   lhs, const string& rhs){
		return !(lhs == rhs);
	}
	bool operator!= (const string& lhs, const char*   rhs){
		return !(lhs == rhs);
	}
    bool operator< (const string& lhs, const string& rhs){
		return lhs.compare(rhs) < 0;
	}
	bool operator< (const char*   lhs, const string& rhs){
		return rhs.compare(lhs) > 0;
	}
	bool operator< (const string& lhs, const char*   rhs){
		return lhs.compare(rhs) < 0;
	}
    bool operator<= (const string& lhs, const string& rhs){
		return lhs.compare(rhs) <= 0;
	}
	bool operator<= (const char*   lhs, const string& rhs){
		return rhs.compare(lhs) >= 0;
	}
	bool operator<= (const string& lhs, const char*   rhs){
		return lhs.compare(rhs) <= 0;
	}
	bool operator>  (const string& lhs, const string& rhs){
		return !(lhs <= rhs);
	}
	bool operator>  (const char*   lhs, const string& rhs){
		return !(lhs <= rhs);
	}
	bool operator>  (const string& lhs, const char*   rhs){
		return !(lhs <= rhs);
	}
    bool operator>= (const string& lhs, const string& rhs){
		return !(lhs < rhs);
	}
	bool operator>= (const char*   lhs, const string& rhs){
		return !(lhs < rhs);
	}
	bool operator>= (const string& lhs, const char*   rhs){
		return !(lhs < rhs);
	}

    void string::swap(string& str){
        if(isSSO()){
            std::swap(_buffer_size, str._buffer_size);
            for(int i=0; i<=_SSO_THRESHOLD; i++){
                std::swap(_buffer[i], str._buffer[i]);
            }
        }else{
            std::swap(_start, str._start);
            std::swap(_size, str._size);
            std::swap(_capacity, str._capacity);
        }
    }

    typename string::size_type string::copy(char* dest, size_type count, size_type pos) const{
        count = GetValidLenth(*this, pos, count);
        const char* src = begin()+pos;
        for(int i=0; i<count; i++){
            *dest = *src;
            ++dest, ++src;
        }
        return count;
    }

    void swap(string& x, string& y){
        x.swap(y);
    }

    std::istream& getline(std::istream& is, string& str, char delim){
		char ch;
		str.clear();
		while(is.get(ch)){
			if (ch == delim)
		        break;
			else
				str.push_back(ch);
		}
		return is;
	}
	std::istream& getline(std::istream& is, string& str){
		return getline(is, str, '\n');
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
        if(isSSO()) ++oldCapacity; //24
        size_type newCapacity = needCapacity < (2*oldCapacity) ? (2*oldCapacity) : ((needCapacity + 15) & (~15));
		return newCapacity;
	}

} // namespace istl
