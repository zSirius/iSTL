#include <iostream>
#include <vector>
#include <chrono>
#include "vector.h" // 包含你实现的 istl::vector 头文件

using namespace std;
using namespace chrono;

static const int numTests = 100;
static const size_t testSize = 1000000;

#define MEASURE_TIME(operation, duration) \
    start = high_resolution_clock::now(); \
    operation; \
    stop = high_resolution_clock::now(); \
    duration += duration_cast<microseconds>(stop - start);

void vectorBenchmark() {
    
    std::vector<int> std_vec;
    istl::vector<int> istl_vec;

    // 记录时间
    high_resolution_clock::time_point start, stop;
    microseconds duration;

    // 用于存储平均时间
    double std_vec_push_back_time = 0, istl_vec_push_back_time = 0;
    double std_vec_access_time = 0, istl_vec_access_time = 0;
    double std_vec_insert_begin_time = 0, istl_vec_insert_begin_time = 0;
    double std_vec_insert_middle_time = 0, istl_vec_insert_middle_time = 0;
    double std_vec_insert_end_time = 0, istl_vec_insert_end_time = 0;
    double std_vec_insert_batch_time = 0, istl_vec_insert_batch_time = 0;
    double std_vec_erase_begin_time = 0, istl_vec_erase_begin_time = 0;
    double std_vec_erase_middle_time = 0, istl_vec_erase_middle_time = 0;
    double std_vec_erase_end_time = 0, istl_vec_erase_end_time = 0;
    double std_vec_erase_batch_time = 0, istl_vec_erase_batch_time = 0;
    double std_vec_clear_time = 0, istl_vec_clear_time = 0;
    double std_vec_pop_back_time = 0, istl_vec_pop_back_time = 0;

    for (int i = 0; i < numTests; ++i) {
        std_vec.clear();
        istl_vec.clear();

        // 测试插入操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_vec.push_back(i), duration);
        std_vec_push_back_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_vec.push_back(i), duration);
        istl_vec_push_back_time += duration.count();

        // 测试随机访问操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_vec[i], duration);
        std_vec_access_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_vec[i], duration);
        istl_vec_access_time += duration.count();

        // 测试插入操作 (在开头、中间、结尾插入)
        duration = microseconds::zero();
        MEASURE_TIME(std_vec.insert(std_vec.begin(), -1), duration);
        std_vec_insert_begin_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.insert(istl_vec.begin(), -1), duration);
        istl_vec_insert_begin_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(std_vec.insert(std_vec.begin() + testSize / 2, -2), duration);
        std_vec_insert_middle_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.insert(istl_vec.begin() + testSize / 2, -2), duration);
        istl_vec_insert_middle_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(std_vec.insert(std_vec.end(), -3), duration);
        std_vec_insert_end_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.insert(istl_vec.end(), -3), duration);
        istl_vec_insert_end_time += duration.count();

        // 测试批量插入
        std::vector<int> insertData(testSize, 42);
        istl::vector<int> insertDataIstl(testSize, 42);

        duration = microseconds::zero();
        MEASURE_TIME(std_vec.insert(std_vec.begin(), insertData.begin(), insertData.end()), duration);
        std_vec_insert_batch_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.insert(istl_vec.begin(), insertDataIstl.begin(), insertDataIstl.end()), duration);
        istl_vec_insert_batch_time += duration.count();

        // 测试删除操作 (删除开头、中间、结尾的元素)
        duration = microseconds::zero();
        MEASURE_TIME(std_vec.erase(std_vec.begin()), duration);
        std_vec_erase_begin_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.erase(istl_vec.begin()), duration);
        istl_vec_erase_begin_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(std_vec.erase(std_vec.begin() + testSize / 2), duration);
        std_vec_erase_middle_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.erase(istl_vec.begin() + testSize / 2), duration);
        istl_vec_erase_middle_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(std_vec.erase(std_vec.end() - 1), duration);
        std_vec_erase_end_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.erase(istl_vec.end() - 1), duration);
        istl_vec_erase_end_time += duration.count();

        // 测试批量删除
        duration = microseconds::zero();
        MEASURE_TIME(std_vec.erase(std_vec.begin(), std_vec.begin() + testSize / 10), duration);
        std_vec_erase_batch_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.erase(istl_vec.begin(), istl_vec.begin() + testSize / 10), duration);
        istl_vec_erase_batch_time += duration.count();

        // 测试清空操作
        duration = microseconds::zero();
        MEASURE_TIME(std_vec.clear(), duration);
        std_vec_clear_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_vec.clear(), duration);
        istl_vec_clear_time += duration.count();

        // 重新填充数据以测试 pop_back 操作
        for (size_t i = 0; i < testSize; ++i) {
            std_vec.push_back(i);
            istl_vec.push_back(i);
        }

        // 测试删除操作
        duration = microseconds::zero();
        MEASURE_TIME(while (!std_vec.empty()) std_vec.pop_back(), duration);
        std_vec_pop_back_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(while (!istl_vec.empty()) istl_vec.pop_back(), duration);
        istl_vec_pop_back_time += duration.count();
    }

    // 输出平均时间
    cout << "Average time for std::vector push_back: " << std_vec_push_back_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector push_back: " << istl_vec_push_back_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector access: " << std_vec_access_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector access: " << istl_vec_access_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector insert at begin: " << std_vec_insert_begin_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector insert at begin: " << istl_vec_insert_begin_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector insert at middle: " << std_vec_insert_middle_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector insert at middle: " << istl_vec_insert_middle_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector insert at end: " << std_vec_insert_end_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector insert at end: " << istl_vec_insert_end_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector batch insert: " << std_vec_insert_batch_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector batch insert: " << istl_vec_insert_batch_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector erase at begin: " << std_vec_erase_begin_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector erase at begin: " << istl_vec_erase_begin_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector erase at middle: " << std_vec_erase_middle_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector erase at middle: " << istl_vec_erase_middle_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector erase at end: " << std_vec_erase_end_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector erase at end: " << istl_vec_erase_end_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector batch erase: " << std_vec_erase_batch_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector batch erase: " << istl_vec_erase_batch_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector clear: " << std_vec_clear_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector clear: " << istl_vec_clear_time / numTests << " microseconds" << endl;

    cout << "Average time for std::vector pop_back: " << std_vec_pop_back_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::vector pop_back: " << istl_vec_pop_back_time / numTests << " microseconds" << endl;

}