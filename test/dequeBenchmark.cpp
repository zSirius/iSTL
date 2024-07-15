#include <iostream>
#include <deque>
#include <chrono>
#include "deque.h" // 包含你实现的 istl::deque 头文件

using namespace std;
using namespace chrono;

static const int numTests = 5; 
static const size_t testSize = 1e8;

#define MEASURE_TIME(operation, duration) \
    start = high_resolution_clock::now(); \
    operation; \
    stop = high_resolution_clock::now(); \
    duration += duration_cast<microseconds>(stop - start);

void dequeBenchmark() {
    
    std::deque<int> std_deq;
    istl::deque<int> istl_deq;

    // 记录时间
    high_resolution_clock::time_point start, stop;
    microseconds duration;

    // 用于存储平均时间
    double std_deq_push_back_time = 0, istl_deq_push_back_time = 0;
    double std_deq_push_front_time = 0, istl_deq_push_front_time = 0;
    double std_deq_access_time = 0, istl_deq_access_time = 0;
    // double std_deq_insert_begin_time = 0, istl_deq_insert_begin_time = 0;
    // double std_deq_insert_middle_time = 0, istl_deq_insert_middle_time = 0;
    // double std_deq_insert_end_time = 0, istl_deq_insert_end_time = 0;
    // double std_deq_erase_begin_time = 0, istl_deq_erase_begin_time = 0;
    // double std_deq_erase_middle_time = 0, istl_deq_erase_middle_time = 0;
    // double std_deq_erase_end_time = 0, istl_deq_erase_end_time = 0;
    double std_deq_clear_time = 0, istl_deq_clear_time = 0;
    double std_deq_pop_back_time = 0, istl_deq_pop_back_time = 0;
    double std_deq_pop_front_time = 0, istl_deq_pop_front_time = 0;

    for (int i = 0; i < numTests; ++i) {
        std_deq.clear();
        istl_deq.clear();

        // 测试push_back操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_deq.push_back(i), duration);
        std_deq_push_back_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_deq.push_back(i), duration);
        istl_deq_push_back_time += duration.count();

        // 测试push_front操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_deq.push_front(i), duration);
        std_deq_push_front_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_deq.push_front(i), duration);
        istl_deq_push_front_time += duration.count();

        // 测试随机访问操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_deq[i], duration);
        std_deq_access_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_deq[i], duration);
        istl_deq_access_time += duration.count();

        // // 测试插入操作 (在开头、中间、结尾插入)
        // duration = microseconds::zero();
        // MEASURE_TIME(std_deq.insert(std_deq.begin(), -1), duration);
        // std_deq_insert_begin_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(istl_deq.insert(istl_deq.begin(), -1), duration);
        // istl_deq_insert_begin_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(std_deq.insert(std_deq.begin() + testSize / 2, -2), duration);
        // std_deq_insert_middle_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(istl_deq.insert(istl_deq.begin() + testSize / 2, -2), duration);
        // istl_deq_insert_middle_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(std_deq.insert(std_deq.end(), -3), duration);
        // std_deq_insert_end_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(istl_deq.insert(istl_deq.end(), -3), duration);
        // istl_deq_insert_end_time += duration.count();

        // // 测试删除操作 (删除开头、中间、结尾的元素)
        // duration = microseconds::zero();
        // MEASURE_TIME(std_deq.erase(std_deq.begin()), duration);
        // std_deq_erase_begin_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(istl_deq.erase(istl_deq.begin()), duration);
        // istl_deq_erase_begin_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(std_deq.erase(std_deq.begin() + testSize / 2), duration);
        // std_deq_erase_middle_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(istl_deq.erase(istl_deq.begin() + testSize / 2), duration);
        // istl_deq_erase_middle_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(std_deq.erase(std_deq.end() - 1), duration);
        // std_deq_erase_end_time += duration.count();

        // duration = microseconds::zero();
        // MEASURE_TIME(istl_deq.erase(istl_deq.end() - 1), duration);
        // istl_deq_erase_end_time += duration.count();

        // 测试清空操作
        duration = microseconds::zero();
        MEASURE_TIME(std_deq.clear(), duration);
        std_deq_clear_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_deq.clear(), duration);
        istl_deq_clear_time += duration.count();

        // 重新填充数据以测试 pop_back 和 pop_front 操作
        for (size_t i = 0; i < testSize; ++i) {
            std_deq.push_back(i);
            istl_deq.push_back(i);
        }

        // 测试pop_back操作
        duration = microseconds::zero();
        MEASURE_TIME(while (!std_deq.empty()) std_deq.pop_back(), duration);
        std_deq_pop_back_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(while (!istl_deq.empty()) istl_deq.pop_back(), duration);
        istl_deq_pop_back_time += duration.count();

        // 重新填充数据以测试 pop_front 操作
        for (size_t i = 0; i < testSize; ++i) {
            std_deq.push_back(i);
            istl_deq.push_back(i);
        }

        // 测试pop_front操作
        duration = microseconds::zero();
        MEASURE_TIME(while (!std_deq.empty()) std_deq.pop_front(), duration);
        std_deq_pop_front_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(while (!istl_deq.empty()) istl_deq.pop_front(), duration);
        istl_deq_pop_front_time += duration.count();
    }

    // 输出平均时间
    cout << "Average time for std::deque push_back: " << std_deq_push_back_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::deque push_back: " << istl_deq_push_back_time / numTests << " microseconds" << endl;

    cout << "Average time for std::deque push_front: " << std_deq_push_front_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::deque push_front: " << istl_deq_push_front_time / numTests << " microseconds" << endl;

    cout << "Average time for std::deque access: " << std_deq_access_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::deque access: " << istl_deq_access_time / numTests << " microseconds" << endl;

    // cout << "Average time for std::deque insert at begin: " << std_deq_insert_begin_time / numTests << " microseconds" << endl;
    // cout << "Average time for istl::deque insert at begin: " << istl_deq_insert_begin_time / numTests << " microseconds" << endl;

    // cout << "Average time for std::deque insert at middle: " << std_deq_insert_middle_time / numTests << " microseconds" << endl;
    // cout << "Average time for istl::deque insert at middle: " << istl_deq_insert_middle_time / numTests << " microseconds" << endl;

    // cout << "Average time for std::deque insert at end: " << std_deq_insert_end_time / numTests << " microseconds" << endl;
    // cout << "Average time for istl::deque insert at end: " << istl_deq_insert_end_time / numTests << " microseconds" << endl;

    // cout << "Average time for std::deque erase at begin: " << std_deq_erase_begin_time / numTests << " microseconds" << endl;
    // cout << "Average time for istl::deque erase at begin: " << istl_deq_erase_begin_time / numTests << " microseconds" << endl;

    // cout << "Average time for std::deque erase at middle: " << std_deq_erase_middle_time / numTests << " microseconds" << endl;
    // cout << "Average time for istl::deque erase at middle: " << istl_deq_erase_middle_time / numTests << " microseconds" << endl;

    // cout << "Average time for std::deque erase at end: " << std_deq_erase_end_time / numTests << " microseconds" << endl;
    // cout << "Average time for istl::deque erase at end: " << istl_deq_erase_end_time / numTests << " microseconds" << endl;

    cout << "Average time for std::deque clear: " << std_deq_clear_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::deque clear: " << istl_deq_clear_time / numTests << " microseconds" << endl;

    cout << "Average time for std::deque pop_back: " << std_deq_pop_back_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::deque pop_back: " << istl_deq_pop_back_time / numTests << " microseconds" << endl;

    cout << "Average time for std::deque pop_front: " << std_deq_pop_front_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::deque pop_front: " << istl_deq_pop_front_time / numTests << " microseconds" << endl;
}

