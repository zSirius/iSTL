#include <iostream>
#include <list>
#include <chrono>
#include <random>
#include "list.h" 

using namespace std;
using namespace chrono;

static const int numTests = 20; 
static const size_t testSize = 1e5;

#define MEASURE_TIME(operation, duration) \
    start = high_resolution_clock::now(); \
    operation; \
    stop = high_resolution_clock::now(); \
    duration += duration_cast<microseconds>(stop - start);


/*
这个性能测试代码包含了以下操作的性能比较：
1. push_back 2. push_front 3. 随机访问（通过迭代器） 4. insert 5. erase
6. clear 7. pop_back 8. pop_front 9. splice 10. remove 
11. unique 12. merge 13. sort 14. reverse
*/
void listBenchmark() {
    
    std::list<int> std_list;
    istl::list<int> istl_list;

    // 记录时间
    high_resolution_clock::time_point start, stop;
    microseconds duration;

    // 用于存储平均时间
    double std_list_push_back_time = 0, istl_list_push_back_time = 0;
    double std_list_push_front_time = 0, istl_list_push_front_time = 0;
    double std_list_access_time = 0, istl_list_access_time = 0;
    double std_list_insert_time = 0, istl_list_insert_time = 0;
    double std_list_erase_time = 0, istl_list_erase_time = 0;
    double std_list_clear_time = 0, istl_list_clear_time = 0;
    double std_list_pop_back_time = 0, istl_list_pop_back_time = 0;
    double std_list_pop_front_time = 0, istl_list_pop_front_time = 0;
    double std_list_splice_time = 0, istl_list_splice_time = 0;
    double std_list_remove_time = 0, istl_list_remove_time = 0;
    double std_list_unique_time = 0, istl_list_unique_time = 0;
    double std_list_merge_time = 0, istl_list_merge_time = 0;
    double std_list_sort_time = 0, istl_list_sort_time = 0;
    double std_list_reverse_time = 0, istl_list_reverse_time = 0;

    for (int i = 0; i < numTests; ++i) {
        std_list.clear();
        istl_list.clear();

        // 测试push_back操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_list.push_back(i), duration);
        std_list_push_back_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_list.push_back(i), duration);
        istl_list_push_back_time += duration.count();

        // 测试push_front操作
        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) std_list.push_front(i), duration);
        std_list_push_front_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(for (size_t i = 0; i < testSize; ++i) istl_list.push_front(i), duration);
        istl_list_push_front_time += duration.count();

        // 测试随机访问操作
        duration = microseconds::zero();
        MEASURE_TIME({
            auto it = std_list.begin();
            for (size_t i = 0; i < testSize / 2; ++i) ++it;
        }, duration);
        std_list_access_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME({
            auto it = istl_list.begin();
            for (size_t i = 0; i < testSize / 2; ++i) ++it;
        }, duration);
        istl_list_access_time += duration.count();

        // 测试插入操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.insert(std_list.begin(), -1), duration);
        std_list_insert_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.insert(istl_list.begin(), -1), duration);
        istl_list_insert_time += duration.count();

        // 测试删除操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.erase(std_list.begin()), duration);
        std_list_erase_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.erase(istl_list.begin()), duration);
        istl_list_erase_time += duration.count();

        // 测试清空操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.clear(), duration);
        std_list_clear_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.clear(), duration);
        istl_list_clear_time += duration.count();

        // 重新填充数据以测试 pop_back 和 pop_front 操作
        for (size_t i = 0; i < testSize; ++i) {
            std_list.push_back(i);
            istl_list.push_back(i);
        }

        // 测试pop_back操作
        duration = microseconds::zero();
        MEASURE_TIME(while (!std_list.empty()) std_list.pop_back(), duration);
        std_list_pop_back_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(while (!istl_list.empty()) istl_list.pop_back(), duration);
        istl_list_pop_back_time += duration.count();

        // 重新填充数据以测试 pop_front 操作
        for (size_t i = 0; i < testSize; ++i) {
            std_list.push_back(i);
            istl_list.push_back(i);
        }

        // 测试pop_front操作
        duration = microseconds::zero();
        MEASURE_TIME(while (!std_list.empty()) std_list.pop_front(), duration);
        std_list_pop_front_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(while (!istl_list.empty()) istl_list.pop_front(), duration);
        istl_list_pop_front_time += duration.count();

        // 测试splice操作
        std::list<int> std_list2(testSize / 2, 1);
        istl::list<int> istl_list2(testSize / 2, 1);

        duration = microseconds::zero();
        MEASURE_TIME(std_list.splice(std_list.begin(), std_list2), duration);
        std_list_splice_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.splice(istl_list.begin(), istl_list2), duration);
        istl_list_splice_time += duration.count();

        // 测试remove操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.remove(testSize / 2), duration);
        std_list_remove_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.remove(testSize / 2), duration);
        istl_list_remove_time += duration.count();

        // 重新填充数据以测试 unique 操作
        for (size_t i = 0; i < testSize; ++i) {
            std_list.push_back(i % 100);
            istl_list.push_back(i % 100);
        }

        // 测试unique操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.unique(), duration);
        std_list_unique_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.unique(), duration);
        istl_list_unique_time += duration.count();

        // 测试merge操作
        std::list<int> std_list3(testSize / 2, 1);
        istl::list<int> istl_list3(testSize / 2, 1);

        duration = microseconds::zero();
        MEASURE_TIME(std_list.merge(std_list3), duration);
        std_list_merge_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.merge(istl_list3), duration);
        istl_list_merge_time += duration.count();

        // 重新填充数据以测试 sort 操作
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(-10000, 10000);
        for (size_t i = 0; i < testSize; ++i) {
            int random_number = dis(gen);
            std_list.push_back(random_number);
            istl_list.push_back(random_number);
        }

        // 测试sort操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.sort(), duration);
        std_list_sort_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.sort(), duration);
        istl_list_sort_time += duration.count();

        // 测试reverse操作
        duration = microseconds::zero();
        MEASURE_TIME(std_list.reverse(), duration);
        std_list_reverse_time += duration.count();

        duration = microseconds::zero();
        MEASURE_TIME(istl_list.reverse(), duration);
        istl_list_reverse_time += duration.count();
    }

    // 输出平均时间
    cout << "Average time for std::list push_back: " << std_list_push_back_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list push_back: " << istl_list_push_back_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list push_front: " << std_list_push_front_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list push_front: " << istl_list_push_front_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list access: " << std_list_access_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list access: " << istl_list_access_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list insert: " << std_list_insert_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list insert: " << istl_list_insert_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list erase: " << std_list_erase_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list erase: " << istl_list_erase_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list clear: " << std_list_clear_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list clear: " << istl_list_clear_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list pop_back: " << std_list_pop_back_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list pop_back: " << istl_list_pop_back_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list pop_front: " << std_list_pop_front_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list pop_front: " << istl_list_pop_front_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list splice: " << std_list_splice_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list splice: " << istl_list_splice_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list remove: " << std_list_remove_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list remove: " << istl_list_remove_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list unique: " << std_list_unique_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list unique: " << istl_list_unique_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list merge: " << std_list_merge_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list merge: " << istl_list_merge_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list sort: " << std_list_sort_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list sort: " << istl_list_sort_time / numTests << " microseconds" << endl;

    cout << "Average time for std::list reverse: " << std_list_reverse_time / numTests << " microseconds" << endl;
    cout << "Average time for istl::list reverse: " << istl_list_reverse_time / numTests << " microseconds" << endl;
}
