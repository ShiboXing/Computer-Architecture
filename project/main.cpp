#include "components/components.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int PC = 0;
float REGS[32] = { 0 };
unordered_map<string, int> TAG_TB;
unordered_map<int, int> MEM;

int main() {

    ins_queue ins_tb;
    CDB cdb(NB);
    back_writer bck_wrter;
    res_station rs;
    fetcher f("ins1.dat");
    decoder d;


    while (true) {
        bool ended = true;

        // write back
        

        // execute 
        rs.execute(bck_wrter);

        // decode, issue
        for (int i=0; i<NW; i++) {
            if (ins_tb.issue_q.size()) {
                ended = false;  
                instruction *ins = ins_tb.pop_unissued();
                d.rename(*ins);
                rs.issue(*ins);
            }
        }        
        
        // fetch
        for (int i=0; i<NF; i++) {
            string ins_str;
            if ((ins_str = f.fetch_next()).length()) {
                ended = false;
                instruction *ins = new instruction(PC, ins_str);
                if (ins->is_mem) { // load memory content and skip
                    delete ins;
                    break;
                } else { // handle instructions
                    ins_tb.add_ins(*ins);
                }
            } else {
                break;
            }
        }

        // program ends if all stages are idle
        if (ended) {
            break;
        }
    }

    return 0; 
}