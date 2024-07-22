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

struct GetFirst {
    template <typename T1, typename T2>
    T1 operator()(const std::pair<T1, T2>& p) const {
        return p.first;
    }
};

struct ComparePairs {
    template <typename T1, typename T2>
    bool operator()(const T1& key1, const T2& key2) const {
        return key1 < key2;
    }
};

int main(){
    istl::_Rb_tree<int, std::pair<int,int>, GetFirst, ComparePairs > rb;

    // 设置随机数种子
    srand(time(0));
    std::vector<int> v;

    // 生成并打印20个1到100之间的随机数
    for (int i = 0; i < 100; ++i) {
        int randomNumber = rand() % 100 + 1; // 生成1到100之间的随机数
        cout << i << ":"<< randomNumber << endl;
        v.push_back(randomNumber);
        rb._insert_unique(std::make_pair(randomNumber, i));
    }
    cout << endl;


    // for(int i=0; i<v.size(); i+=10){
    //     rb.erase(v[i]);
    // }

    for(int i=0; i<v.size(); i++){
        auto it = rb.find(v[i]);
        cout << it->first <<endl;
    }

    rb.print_rbtree();

    return 0;
}