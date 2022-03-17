#include "components.h"

decoder::decoder(ins_table &ins_tb_ref) : ins_tb(ins_tb_ref){
    
    for (int i=31; i>=0; i--) {
       free_list.push_back(i);
    }
}

void decoder::rename_last() {
    // get the last register
    auto last_regs = ins_tb.ins_q.back().second._info;
    int n_reg_cnt = 0;

    /**
     * TODO: only check operands that are registers
     */
    // iterate the instruction to check how many new registers are needed
    for (int i=1; i<4; i++) {
        if (reg_lst.find(last_regs[i]) == reg_lst.end())
            n_reg_cnt++;
    }

    
    // not enough free regs
    if (n_reg_cnt > free_list.size())
        return;
    else {
        
    }
    
    /** 
     * TODO: exclude store instruction and read-only ins
     */
    // last ins's dest register
    string *reg = &last_regs[1];

    // iterate backwards to check for fake data hazards
    for (int i=ins_tb.ins_q.size()-1; i>=0; i--) {
        auto curr = ins_tb.ins_q[i].second._info;
        if (curr[1] == *reg || curr[2] == *reg || curr[3] == *reg) {
            // rename
            reg_lst[*reg] = "p" + to_string(free_list.back());
            free_list.pop_back();
            break;
        }
    }
}