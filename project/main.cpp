#include "components/components.h"
#include "structures/structures.h"
#include <iostream>
#include <stdio.h>


using namespace std;

int PC = 0;
unordered_map<int, string> TAG_TB;

int main() {
    PC = 4;

    fetcher f(4, "ins.dat");
    auto ins_str = f.fetch_next();
    instruction ins(ins_str);

    // cout << INS_SET["add"] <<endl;

    return 0; 
}