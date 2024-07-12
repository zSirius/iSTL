#include <iostream>
#include "deque.h"
void vectorBenchmark();
void dequeBenchmark();
int main(){
    vectorBenchmark();
    dequeBenchmark();
    // istl::deque<int> istl_deq;
    // int n = 1000000;
    // for (size_t i = 0; i < n; ++i) istl_deq.push_back(i);
    return 0;
}