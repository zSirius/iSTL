#include <iostream>
#include <ctime>
#include "../include/alloc.h"
using namespace std;


int main(){
    // size_t bytes = 8;
    // for(int i=bytes; i<=128; i+=8){
    //     void *ptr = istl::alloc::allocate(i);
    //     //cout << ptr << " ";
    //     int j=i/8-1;
    //     istl::alloc::print_list(j);
    // }

    std::srand(std::time(0));
    for(int i=0; i<1e7; i++){
        int random_number = std::rand() % 128 + 1;
        std::cout << "[" << i << "]: " << random_number << std::endl;
        void *ptr = istl::alloc::allocate(random_number);
    }

    for(int i=0; i<16; i++){
        istl::alloc::print_list(i);
    }

    return 0;
}