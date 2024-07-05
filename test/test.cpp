#include <iostream>
#include <ctime>
#include <type_traits>

#include "uninitialized.h"

using namespace std;

class tclass{
public:
    tclass(int v=0):val(v){}
    void get(){ cout << val << endl; }
    void set(int v){ val = v;}
private:
        int val;
};

int main() {
    // int *p = (int *)malloc(32 * sizeof(int));
    // int *q = (int *)malloc(32 * sizeof(int));

    // istl::uninitialized_fill(p, p+32, 123);

    // for(int i=0; i<32; i++) cout << p[i] << " ";

    tclass p[32];
    p[1].set(999);
    tclass *q = (tclass *)malloc(32 * sizeof(tclass));

    istl::uninitialized_fill_n(q, 31, p[1]);
    for(int i=0; i<32; i++) q[i].get();

    return 0;
}