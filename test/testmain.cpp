#include <iostream>
#include "list.h"
#include "deque.h"

// void vectorBenchmark();
// void dequeBenchmark();
using namespace std;

int main(){
    //vectorBenchmark();
    //dequeBenchmark();
    
    istl::list<const int> l1;
    for(int i=0; i<5; i++) l1.push_back(i);
    istl::list<const int> l2 = l1;
    
    l1.splice(l1.begin(), l2, ++l2.begin(), l2.end());
    for(auto it = l1.begin(); it != l1.end(); ++it) cout << *it << endl;
    cout << endl;
    for(auto it = l2.begin(); it != l2.end(); ++it) cout << *it << endl;

    cout << "size l1 =" << l1.size() << " l2=" << l2.size();

    return 0;
}