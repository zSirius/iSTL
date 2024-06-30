#include <iostream>
#include "../include/alloc.h"

int main(){
    size_t bytes = 8;
    void *ptr = istl::alloc::allocate(bytes);
    std::cout << "hello" << std::endl;
    return 0;
}