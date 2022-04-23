#include "components/components.h"
#include <iostream>
#include <stdio.h>

using namespace std;

// initialize extern vars
int PC = 0;
int CYCLE = 0;
float REGS[32] = { 0 };
unordered_map<string, int> TAG_TB;
unordered_map<int, int> MEM;

// declare extern tomasulo parameters
int NF;
int NW;
int NR;
int NB;

int main(int argc, char** argv) {

    if (argc != 6) {
        cout << "usage : ./main [NF] [NW] [NR] [NB] [path_to_test_file] " << endl;
        exit(0);
    }
    
    NF = atoi(argv[1]);
    NW = atoi(argv[2]);
    NR = atoi(argv[3]);
    NB = atoi(argv[4]);

    string code_pth = argv[5];
    
    CDB bus(NB);
    back_writer bck_wrter;
    ROB reorder_buffer(NR);
    res_station rs;
    ins_queue ins_tb;
    decoder d;
    fetcher f(code_pth);
    ofstream MEM_STREAM("mem.out");

    bool program_started = false;

    while (true) {
        bool running = false;

        // COMMIT 
        running |= reorder_buffer.commit(bus, d);
        
        // WRITE BACK
        running |= bck_wrter.write_back(bus);

        // EXECUTE 
        running |= rs.execute(bck_wrter, reorder_buffer);

        // DECODE, ISSUE
        d.free_regs();
        for (int i=0; i<NW; i++) {
            if (ins_tb.ins_q.size()) {
                running = true;
                instruction *ins = ins_tb.ins_q.back();
                if (d.can_rename() && rs.can_issue(*ins) && !reorder_buffer.is_full()) {
                    d.rename(*ins);
                    rs.issue(*ins, reorder_buffer);
                    ins_tb.ins_q.pop_back();
                } else {
                    break;
                }
            }
        }        
        
        // FETCH
        for (int i=0; i<NF; i++) {
            string ins_str;
            if ((ins_str = f.fetch_next()).length()) {
                running = true;
                instruction *ins = new instruction(PC, ins_str);
                if (ins->is_mem) { // load memory content and skip
                    delete ins;
                    break;
                } else { // handle instructions
                    program_started = true;
                    ins_tb.add_ins(*ins);
                }
            } else {
                break;
            }
        }

        // clear CDB
        bus.flush();

        // program ends if all stages are idle
        if (!running) {
            break;
        }

        // increment cycle count
        if (program_started) {
            CYCLE++;
            cout << "cycle: " << CYCLE << endl;
        } else {
            cout << "loading memory: " << endl;
        }        
    }

    // print out regs
    d.print_regs();
    
    // print out memory
    MEM_STREAM << "memory content: " << endl;
    for (auto itm : MEM) {
        MEM_STREAM << itm.first << ":  " << itm.second << endl;
    }
    MEM_STREAM.close();
    
    return 0; 
}