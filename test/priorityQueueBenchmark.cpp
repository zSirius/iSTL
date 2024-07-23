#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <chrono>
#include "priority_queue.h" 

using namespace std;
using namespace chrono;

static const int numTests = 20;
static const size_t testSize = 100;

#define MEASURE_TIME(operation, duration) \
    start = high_resolution_clock::now(); \
    operation; \
    stop = high_resolution_clock::now(); \
    duration += duration_cast<microseconds>(stop - start);

void priorityQueueBenchmark() {
    priority_queue<int, vector<int>, greater<int>> std_queue;
    istl::priority_queue<int, istl::vector<int>, greater<int>> istl_queue;

    // 记录时间
    high_resolution_clock::time_point start, stop;
    microseconds duration;

    // 用于存储平均时间
    double std_queue_push_time = 0, istl_queue_push_time = 0;
    double std_queue_pop_time = 0, istl_queue_pop_time = 0;
    double std_queue_top_time = 0, istl_queue_top_time = 0;

    for (int i = 0; i < numTests; ++i) {
        while (!std_queue.empty()) std_queue.pop();
        while (!istl_queue.empty()) istl_queue.pop();

        // 测试push操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t j = 0; j < testSize; ++j) std_queue.push(j), duration);
        std_queue_push_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t j = 0; j < testSize; ++j) istl_queue.push(j), duration);
        istl_queue_push_time += duration.count();

        // 测试top操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t j = 0; j < testSize; ++j) std_queue.top(), duration);
        std_queue_top_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t j = 0; j < testSize; ++j) istl_queue.top(), duration);
        istl_queue_top_time += duration.count();

        // 测试pop操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t j = 0; j < testSize; ++j) std_queue.pop(), duration);
        std_queue_pop_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t j = 0; j < testSize; ++j) istl_queue.pop(), duration);
        istl_queue_pop_time += duration.count();
    }

    // 输出平均时间
    cout << "Average time for std::priority_queue push: " << std_queue_push_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::priority_queue push: " << istl_queue_push_time / numTests << " microseconds" << endl;

    cout << "Average time for std::priority_queue top: " << std_queue_top_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::priority_queue top: " << istl_queue_top_time / numTests << " microseconds" << endl;

    cout << "Average time for std::priority_queue pop: " << std_queue_pop_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::priority_queue pop: " << istl_queue_pop_time / numTests << " microseconds" << endl;
}
