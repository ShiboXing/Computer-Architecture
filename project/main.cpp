#include "components/components.h"
#include <iostream>
#include <stdio.h>


using namespace std;

int PC = 0;
unordered_map<string, int> TAG_TB;
unordered_map<int, int> MEM;

int main() {

    ins_table ins_tb;
    fetcher f(4, "ins.dat");
    decoder d(ins_tb);

    string ins_str;
    while ((ins_str = f.fetch_next()).length()) {
        instruction *ins = new instruction(ins_str);
        if (ins->is_mem)
            continue;
        ins_tb.add_ins(*ins, PC);    
        d.rename_last();
    }

    return 0; 
}