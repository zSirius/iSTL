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
#include "map.h"
#include "priority_queue.h"

void vectorBenchmark();
void dequeBenchmark();
void listBenchmark();
void mapBenchmark();
void priorityQueueBenchmark();

using namespace std;



int main(){
    vectorBenchmark();
    dequeBenchmark();
    listBenchmark();
    mapBenchmark();
    priorityQueueBenchmark();

    return 0;
}