#include "vector.h"
#include <iostream>

int main(){
    istl::vector<int> v(5,1000);
    v.push_back(101);
    v.push_back(102);
    v.push_back(103);
    v.pop_back();
    
    istl::vector<int>::iterator p = v.begin();
    v.insert(p, 99);
    p += 2;
    v.erase(p);
    for(int i=0; i<v.size(); i++) std::cout << v[i] << std::endl;
    return 0;
}