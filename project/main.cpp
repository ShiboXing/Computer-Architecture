#include "components/components.h"
#include "structures/structures.h"
#include <iostream>
#include <stdio.h>


using namespace std;

int PC = 0;
unordered_map<string, int> TAG_TB;
unordered_map<int, int> MEM;

int main() {
    fetcher f(4, "ins.dat");
    string ins_str;
    while ((ins_str = f.fetch_next()).length()) {
        instruction ins(ins_str);
    }

    return 0; 
}