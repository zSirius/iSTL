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

    // istl::uninitialized_fill_n(p,32, 123456);

    // for(int i=0; i<32; i++) cout << p[i] << " ";

    tclass p[6];
    p[1].set(999999);
    tclass *q = (tclass *)malloc(5 * sizeof(tclass));

    istl::uninitialized_fill_n(q, 5, p[1]);
    for(int i=0; i<5; i++) q[i].get();
    istl::uninitialized_fill(q, q+5, p[1]);

    return 0;
}