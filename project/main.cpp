#include "components/components.h"
#include "structures/structures.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
    fetcher f(4, "ins.dat");
    auto rm = *rename_map::get_map();
    ins_table i_t;
    auto tp = chrono::system_clock::now();
    
    i_t.add_status(tp);
    
    
    return 0; 
}