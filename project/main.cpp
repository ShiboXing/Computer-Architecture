#include "components/components.h"
#include <iostream>
#include <stdio.h>

using namespace std;

// initialize extern vars
int PC = 0;
int CYCLE = 0;
int ROB_STALL = 0;
int RES_STALL = 0;
int CDB_STALL = 0;
int DECODE_STALL = 0;
float REGS[32] = { 0 };
unordered_map<string, int> TAG_TB;
unordered_map<int, int> MEM;
unordered_map<string, int> TAGS;

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

    CDB bus(NB);
    ROB rob(NR);
    BTB btb;
    back_writer bck_wrter;
    res_station rs;
    ins_queue ins_tb;
    decoder d;
    fetcher f;
    ofstream MEM_STREAM("mem.out");
    
    string codepth = argv[5];
    f.add_codepth(codepth); // open code file
    f.scan_tags(); // store all branch tags 
    f.add_codepth(codepth); // open code file for execution
    
    bool program_started = false;
    
    while (true) {
        bool running = false;

        // COMMIT 
        running |= rob.commit(bus, d);
        
        // WRITE BACK
        running |= bck_wrter.write_back(bus);

        // EXECUTE 
        running |= rs.execute(bck_wrter, rob);
        rob.branch_flush(d, ins_tb, f, btb); // flush rob for potential miss prediction

        // DECODE, ISSUE
        for (int i=0; i<NW; i++) {
            if (ins_tb.ins_q.size()) {
                running = true;
                instruction *ins = ins_tb.ins_q.back();
                if (d.can_rename() && rs.can_issue(*ins) && !rob.is_full()) {
                    d.rename(*ins);
                    rs.issue(*ins, rob);
                    ins_tb.ins_q.pop_back();
                } else {
                    break;
                }
            }
        }       
        d.free_regs(); 
       
        
        // FETCH
        for (int i=0; i<NF; i++) {
            string ins_str;
            if ((ins_str = f.fetch_next()).length()) {
                running = true;

                // perform branch prediction
                int target = btb.predict(PC);
                instruction *ins = new instruction(PC, ins_str);
                // perform branch fetch or increment PC
                if (target != PC+1)
                    f.set_pc(target);
                else
                    PC++;

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
        if (program_started) 
            CYCLE++;
    }

    cout << endl;
    cout << "RESERVATION STATION STALLS: \033[31m" << RES_STALL << "\033[0m" << endl;
    cout << "ROB STALLS: \033[31m" << ROB_STALL << "\033[0m" << endl;
    cout << "DECODE STALLS: \033[31m" << DECODE_STALL << "\033[0m" << endl;
    cout << "CDB STALLS: \033[31m" << CDB_STALL << "\033[0m" << endl;
    cout << "TOTAL CYCLES: \033[32m" << CYCLE  << "\033[0m" << endl;
    
    // print out regs
    d.print_regs();
    
    // print out memory
    MEM_STREAM << "MEM CONTENT: " << endl;
    for (auto itm : MEM) {
        MEM_STREAM << itm.first << ":  " << itm.second << endl;
    }
    MEM_STREAM.close();
    
    return 0; 
}