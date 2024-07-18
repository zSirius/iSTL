#include <iostream>
#include <random> // 包含随机数生成设施
#include <ctime>  // 包含时间函数，用于种子生成
#include <utility>
#include "list.h"
#include "deque.h"
#include "stack.h"
#include "queue.h"
#include "istl_string.h"
#include "vector.h"

// void vectorBenchmark();
// void dequeBenchmark();
// void listBenchmark();

using namespace std;

// bool isNearDuplicate(int a, int b) {
//     return std::abs(a - b) <= 1; // 如果两个元素的差的绝对值小于等于1，则认为它们是“接近重复”的
// }

// bool isOdd(int n) {
//     return n % 2 != 0;
// }

// bool customCompare(int a, int b) {
//     return a > b; // 从大到小排序
// }



int main(){
    //vectorBenchmark();
    //dequeBenchmark();
    // listBenchmark();
    istl::string s("01234567");
    //istl::string s1("0123456789012345678901234567");
    //s.insert(6, "123567");
    //s.insert(6, 20, 'a');
    cout << s;
    cout << endl;
    cout << s.find("123", 0) << endl;
    cout << "s.size = " << s.size() << " s.capacity = " << s.capacity() << endl;
    // s.resize(33);
    // cout << "s.size = " << s.size() << " s.capacity = " << s.capacity() << endl;

    // s.resize(66);
    // cout << "s.size = " << s.size() << " s.capacity = " << s.capacity() << endl;

    // s.resize(277);
    // cout << "s.size = " << s.size() << " s.capacity = " << s.capacity() << endl;

    // s.resize(510);
    // cout << "s.size = " << s.size() << " s.capacity = " << s.capacity() << endl;

    // s.resize(1300);
    // cout << "s.size = " << s.size() << " s.capacity = " << s.capacity() << endl;


    
    
    //istl::string s2 = "122222";

    // cout << s.isSSO() << endl;
    // cout << &s._capacity << " " << &s._size << " " << &s._start << endl;
    // cout << "Size of istl::string: " << sizeof(s) << endl;
    // cout << "Address of s.sso.size: " << static_cast<void*>(&s._buffer_size) << endl;
    // cout << "Address of s.sso.buffer[0]: " << static_cast<void*>(&s._buffer[0]) << endl;

    // istl::vector<int> v{1,2,3,4,5,6}, v2{90,91,92,93,94};
    // for(auto t : v){
    //     cout << t << " ";
    // }
    // cout << endl;

    // v.reserve(100);

    // v.insert(v.begin()+1, 10, 5);

    // for(auto t : v){
    //     cout << t << " ";
    // }

    return 0;
}