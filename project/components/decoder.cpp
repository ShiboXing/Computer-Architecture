#include "components.h"

decoder::decoder(ins_table &ins_tb_ref) : ins_tb(ins_tb_ref){
    
    for (int i=31; i>=0; i--) {
       _free_lst.push_back(i);
    }
}

void decoder::rename_last() {
    // get the last register
    auto last_ins = ins_tb.ins_q.front().second._info;

    // iterate the instruction to check how many new registers are needed
    vector<string> n_regs;
    for (int i=1; i<4; i++) {
        string reg_str = last_ins[i];
        if ((reg_str[0] == 'R' || reg_str[0] == 'F') && _reg_lst.find(reg_str) == _reg_lst.end())
            n_regs.push_back(reg_str);
    }

    // not enough free regs
    if (n_regs.size() > _free_lst.size()) 
        return;
    else {
        for (auto reg_str : n_regs) {
            _reg_lst[reg_str] = "p" + to_string(_free_lst.back());
            _free_lst.pop_back();
        }
    }

    // last ins's dest reg
    string dest = last_ins[1];
    // iterate backwards to check for fake data hazards
    for (int i=1; i<ins_tb.ins_q.size(); i++) {
        auto curr = ins_tb.ins_q[i].second._info;
        if (last_ins[0] != "fsd" && last_ins[0] != "bne" && dest[0] != '$' && (curr[1] == dest || curr[2] == dest || curr[3] == dest)){
            // rename
            _reg_lst[dest] = "p" + to_string(_free_lst.back());
            _free_lst.pop_back();
            break;
        }
    }
}