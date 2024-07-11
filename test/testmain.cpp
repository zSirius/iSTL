#include <iostream>
#include "deque.h"
void vectorBenchmark();
int main(){
    //vectorBenchmark();
    istl::deque<int> deq;
    //deq.push_back(0);
    for(int i=0; i<64; i++) {
        deq.push_back(i);
        std::cout << deq[i] << std::endl;
    }
    //for(int i=0; i<64; i++) std::cout << deq[i] << std::endl;
    for(int i=0; i<64; i++) {
        deq.push_front(-i);
        std::cout << deq[0] << std::endl;
    }
    //for(int i=0; i<128; i++) std::cout << deq[i] << std::endl;
    //for(int i=0; i<8; i++) deq.push_front(-i);
    //for(int i=0; i<16; i++) std::cout << deq[i] << std::endl;
    return 0;
}