#include <iostream>
#include <ctime>
#include <type_traits>
#include "../include/vector.h"
#include "../include/alloc.h"
using namespace std;

class test
{
private:
static int a;
};
int test:: a = 100;



int main(){
    // size_t bytes = 8;
    // for(int i=bytes; i<=128; i+=8){
    //     void *ptr = istl::alloc::allocate(i);
    //     //cout << ptr << " ";
    //     int j=i/8-1;
    //     istl::alloc::print_list(j);
    // }

    // std::srand(std::time(0));
    // for(int i=0; i<1e3; i++){
    //     int random_number = std::rand() % 128 + 1;
    //     std::cout << "[" << i << "]: " << random_number << std::endl;
    //     void *ptr = istl::alloc::allocate(random_number);
    // }

    // for(int i=0; i<16; i++){
    //     istl::alloc::print_list(i);
    // }
    cout << is_pod<test>::value << endl;
    return 0;
}