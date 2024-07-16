#include <iostream>
#include <random> // 包含随机数生成设施
#include <ctime>  // 包含时间函数，用于种子生成
#include "list.h"
#include "deque.h"
#include "stack.h"
#include "queue.h"




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
    
    // 使用时间作为随机数种子
    //std::mt19937 rng(static_cast<unsigned int>(std::time(0))); // 随机数引擎

    // // 定义1到100之间的分布
    //std::uniform_int_distribution<int> dist(1, 100);


    //istl::list<int> l1, l2;
    // 生成50个随机数
    // for (int i = 0; i <10; ++i) {
    //     int random_number = dist(rng);
    //     l1.push_back(random_number);
    //     //l1.push_back(i);
    // }

    // for (int i = 0; i <10; ++i) {
    //     int random_number = dist(rng);
    //     l2.push_back(random_number);
    //     //l1.push_back(i);
    // }
    // l1.cbegin();
    // for(auto it = l1.begin(); it != l1.end(); ++it) cout << *it << endl;
    // cout << endl;
    // l1.sort();
    // l2.sort();
    // l1.merge(l2);

    //l1.cbegin();
    
    // l1.merge(l2);
    // istl::list<int> l1 = {1,1,1,2,2,2,4,4,4,4};
    // istl::list<int> l2 = {1,1,1,1,2,2,2,2,8,8,8,9,9,0};
    // l1.remove(1);
    // l2.remove(9);
    // for(auto it = l1.cbegin(); it != l1.cend(); ++it) {
    //     cout << *it << endl;
    // }
    // cout << endl;
    // for(auto it = l2.begin(); it != l2.end(); ++it) cout << *it << endl;

    // cout << "size l1 =" << l1.size() << " l2=" << l2.size();

    istl::stack<int> stk, stk2;
    stk.push(1);
    
    
    stk2.push(999);

    stk.swap(stk2);

    cout << stk.top() << " " << stk2.top()<<endl;





    return 0;
}