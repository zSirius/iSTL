#include <iostream>
#include <map>
#include <chrono>
#include "map.h"  // 包含你实现的 istl::map 头文件

using namespace std;
using namespace chrono;

const int numTests = 20;
const size_t testSize = 100000;

#define MEASURE_TIME(operation, duration) \
    start = high_resolution_clock::now(); \
    operation; \
    stop = high_resolution_clock::now(); \
    duration += duration_cast<microseconds>(stop - start);

void mapBenchmark() {
    std::map<int, int> std_map;
    istl::map<int, int> istl_map;

    // 记录时间
    high_resolution_clock::time_point start, stop;
    microseconds duration;

    // 用于存储平均时间
    double std_map_insert_time = 0, istl_map_insert_time = 0;
    double std_map_erase_time = 0, istl_map_erase_time = 0;
    double std_map_find_time = 0, istl_map_find_time = 0;

    for (int i = 0; i < numTests; ++i) {
        std_map.clear();
        istl_map.clear();

        // 测试插入操作
        duration = microseconds::zero();
        MEASURE_TIME(for (int j = 0; j < testSize; ++j) std_map.insert({j, j}), duration);
        std_map_insert_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (int j = 0; j < testSize; ++j) istl_map.insert({j, j}), duration);
        istl_map_insert_time += duration.count();

        // 测试查找操作
        duration = microseconds::zero();
        MEASURE_TIME(for (int j = 0; j < testSize; ++j) std_map.find(j), duration);
        std_map_find_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (int j = 0; j < testSize; ++j) istl_map.find(j), duration);
        istl_map_find_time += duration.count();

        // 测试删除操作
        duration = microseconds::zero();
        MEASURE_TIME(for (int j = 0; j < testSize; ++j) std_map.erase(j), duration);
        std_map_erase_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (int j = 0; j < testSize; ++j) istl_map.erase(j), duration);
        istl_map_erase_time += duration.count();
    }

    // 输出平均时间
    cout << "Average time for std::map insert: " << std_map_insert_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::map insert: " << istl_map_insert_time / numTests << " microseconds" << endl;

    cout << "Average time for std::map find: " << std_map_find_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::map find: " << istl_map_find_time / numTests << " microseconds" << endl;

    cout << "Average time for std::map erase: " << std_map_erase_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::map erase: " << istl_map_erase_time / numTests << " microseconds" << endl;
}

// 主函数，用于运行性能测试
int main(int argc, char **argv) {

    // 运行特定的性能测试
    mapBenchmark();

    return 0;
}