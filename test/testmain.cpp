#include <iostream>
#include <random> // 包含随机数生成设施
#include <ctime>  // 包含时间函数，用于种子生成
#include <utility>
#include "list.h"
#include "deque.h"
#include "stack.h"
#include "queue.h"
#include "istl_string.h"

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
    //istl::string s("1234567");
    istl::string s("123456789012345678901234567", 3);
    istl::string s2 = "122222";
    cout << s.isSSO() << endl;
    cout << &s._capacity << " " << &s._size << " " << &s._start << endl;
    cout << "Size of istl::string: " << sizeof(s) << endl;
    cout << "Address of s.sso.size: " << static_cast<void*>(&s._buffer_size) << endl;
    cout << "Address of s.sso.buffer[0]: " << static_cast<void*>(&s._buffer[0]) << endl;


    return 0;
}