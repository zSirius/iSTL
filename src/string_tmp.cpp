// #include "string.h"
// #include <algorithm>

// namespace istl
// {
//     string::string(const string& str){
// 		allocateAndCopy(str.start_, str.finish_);
// 	}

//     string::string(string&& str){
// 		moveData(str);
// 	}

//     string::string(const char* s){
// 		allocateAndCopy(s, s + strlen(s));
// 	}

//     string::string(const char* s, size_t n){
// 		allocateAndCopy(s, s + n);
// 	}

//     string::string(size_t n, char c){
// 		allocateAndFillN(n, c);
// 	}

//     string::string(const string& str, size_t pos, size_t len){
// 		len = changeVarWhenEuqalNPOS(len, str.size(), pos);
// 		allocateAndCopy(str.start_ + pos, str.start_ + pos + len);
// 	}

//     string::~string(){ 
// 		destroyAndDeallocate(); 
// 	}

//     string& string::operator= (const string& str){
// 		if (this != &str){
// 			destroyAndDeallocate();
// 			allocateAndCopy(str.start_, str.finish_);
// 		}
// 		return *this;
// 	}

// 	string& string::operator= (string&& str){
// 		if (this != &str){
//             destroyAndDeallocate();
// 			moveData(str);
// 		}
// 		return *this;
// 	}

//     string& string::operator= (const char* s){
// 		destroyAndDeallocate();
// 		allocateAndCopy(s, s + strlen(s));
// 		return *this;
// 	}

//     string& string::operator= (char c){
// 		destroyAndDeallocate();
// 		allocateAndFillN(1, c);
// 		return *this;
// 	}

//     void string::resize(size_t n){
// 		resize(n, value_type());
// 	}

//     void string::resize(size_t n, char c){
// 		if (n < size()){
// 			dataAllocator::destroy(start_ + n, finish_);
// 			finish_ = start_ + n;
// 		}
// 		else if (n > size() && n <= capacity()){
// 			auto lengthOfInsert = n - size();
// 			finish_ = istl::uninitialized_fill_n(finish_, lengthOfInsert, c);
// 		}
// 		else if (n > capacity()){
// 			auto lengthOfInsert = n - size();
// 			iterator newStart = dataAllocator::allocate(getNewCapacity(lengthOfInsert));
// 			iterator newFinish = istl::uninitialized_copy(begin(), end(), newStart);
// 			newFinish = istl::uninitialized_fill_n(newFinish, lengthOfInsert, c);

// 			destroyAndDeallocate();
// 			start_ = newStart;
// 			finish_ = newFinish;
// 			endOfStorage_ = start_ + n;
// 		}
// 	}

//     void string::reserve(size_t n){
// 		if (n <= capacity()) return;
// 		iterator newStart = dataAllocator::allocate(n);
// 		iterator newFinish = istl::uninitialized_move(begin(), end(), newStart);
// 		destroyAndDeallocate();
// 		start_ = newStart;
// 		finish_ = newFinish;
// 		endOfStorage_ = start_ + n;
// 	}

//     void string::moveData(string& str){
// 		start_ = str.start_;
// 		finish_ = str.finish_;
// 		endOfStorage_ = str.endOfStorage_;
// 		str.start_ = str.finish_ = str.endOfStorage_ = nullptr;
// 	}

//     void string::shrink_to_fit(){
//         iterator newstart = dataAllocator::allocate(size());
//         iterator newfinish = istl::uninitialized_move(start_, finish_, newstart);
//         destroyAndDeallocate();
//         start_ = newstart;
//         endOfStorage_ = finish_ = newfinish;
//     }

//     string& string::insert(size_t index, const string& str){
// 		insert(start_ + index, str.begin(), str.end());
// 		return *this;
//     }

//     string& string::insert(size_t index, const string& str, size_t subindex, size_t sublen){
// 		sublen = changeVarWhenEuqalNPOS(sublen, str.size(), subindex);
// 		insert(begin() + index, str.begin() + subindex, str.begin() + subindex + sublen);
// 		return *this;
// 	}

//     string& string::insert(size_t index, const char* s){
// 		insert(begin() + index, s, s + strlen(s));
// 		return *this;
// 	}

//     string& string::insert(size_t index, const char* s, size_t n){
// 		insert(begin() + index, s, s + n);
// 		return *this;
// 	}

//     string& string::insert(size_t index, size_t n, char c){
// 		insert(begin() + index, n, c);
// 		return *this;
// 	}

//     string::iterator string::insert(iterator pos, size_t count, char c){
//         difference_type idx = pos - begin();
//         iterator vpos = begin() + idx;
//         if(count == 0) return vpos;
//         difference_type SpaceLeft = _endOfStorage - _finish;
//         difference_type SpaceNeed = count;

//         if(SpaceLeft >= SpaceNeed){
//             auto ptr = end() - 1;
//             for(; ptr >= pos; --ptr){
//                 dataAllocator::construct(vpos+SpaceNeed, std::move(*ptr));
//             }
//             istl::uninitialized_fill_n(vpos, count, c);
//             _finish += count;
//         }else{
//             difference_type newCapacity = getNewCapacity(count);

//             T *newstart = dataAllocator::allocate(newCapacity);
//             T *newfinish = istl::uninitialized_move(begin(), vpos, newstart);
//             newfinish = istl::uninitialized_fill_n(newfinish, count, c);
//             newfinish = istl::uninitialized_move(vpos, end(), newfinish);

//             destroyAndDeallocate();
//             _start = newstart, _finish = newfinish;
//             _endOfStorage = _start + newCapacity;
//         }
//         return begin() + idx;
// 	}

//     string::iterator string::insert(iterator p, char c){
// 		return insert(p, 1, c);
// 	}

//     void string::allocateAndFillN(size_t n, char c){
// 		start_ = dataAllocator::allocate(n);
// 		istl::uninitialized_fill_n(start_, n, c);
// 		finish_ = endOfStorage_ = start_ + n;
// 	}
//     void string::destroyAndDeallocate(){
//         if(capacity() != 0){
//             dataAllocator::destroy(start_, finish_);
//             dataAllocator::deallocate(start_, endOfStorage_ - start_);
//             start_ = finish_ = endOfStorage_ = nullptr;
//         }
// 	}

//     string::size_type string::getNewCapacity(size_type len)const{
//         //可以在这里修改内存扩张策略
// 		size_type oldCapacity = endOfStorage_ - start_;
// 		auto res = std::max(oldCapacity, len);
// 		auto newCapacity = oldCapacity + res;
// 		return newCapacity;
// 	}

//     size_t string::changeVarWhenEuqalNPOS(size_t var, size_t minuend, size_t minue)const{
// 		return (var == npos ? minuend - minue : var);
// 	}

    


// } // namespace istl
