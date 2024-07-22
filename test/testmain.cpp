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
#include "rbtree.h"

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
    istl::_Rb_tree<int, int, std::pair<int,int>, istl::less<int> > rb;

    return 0;
}