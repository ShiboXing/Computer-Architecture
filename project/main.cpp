#include "components/components.h"
#include <iostream>
#include <stdio.h>


using namespace std;

int PC = 0;
unordered_map<string, int> TAG_TB;
unordered_map<int, int> MEM;

int main() {

    ins_queue ins_tb;
    fetcher f(4, "ins.dat");
    decoder d(ins_tb);
    res_station rs;

    string ins_str;
    while (true) {
        int i = 0;
        while (i < NF) {
            if ((ins_str = f.fetch_next()).length()) {
                instruction *ins = new instruction(PC, ins_str);
                if (ins->is_mem) {
                    delete ins;
                    continue;
                } else {
                    ins_tb.add_ins(*ins, PC);
                    rs.issue(*ins);
                    d.rename_last();
                }
            } else
                break;
            i++;
        }
    }

    return 0; 
}