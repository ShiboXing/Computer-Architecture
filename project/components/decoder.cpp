#include "components.h"

decoder::decoder(ins_queue &ins_tb_ref) : ins_tb(ins_tb_ref){
    for (int i=31; i>=0; i--) {
       _free_lst.push_back(i);
    }
}

void decoder::rename_last() {
    // get the last register
    auto last_ins = &(ins_tb.ins_q.front().second._info);
    string op = (*last_ins)[0];

    // iterate the instruction to check how many new registers are needed
    vector<string> n_regs(4);
   
    for (int i=3; i>=0; i--) { 
        /* perform renaming backwards,
        consider: F0, F0, F4 */
        string reg_str = (*last_ins)[i];
        
        if (reg_str[0] == 'R' || reg_str[0] == 'F') {
            // create new mapping here
            if ((i == 1 && op != "fsd" && op != "bne") // always use a new reg for dest
                || _reg_lst.find(reg_str) == _reg_lst.end()) { // if mapping not found, assign a new reg
                
                /**
                 * @brief TODO: push back the unused free registers 
                 * 
                 */
                if (_free_lst.size() == 0) {// not enough free regs, stall!
                    cout << "not enough free regs" << endl;
                    return;
                }
                n_regs[i] =  "p" + to_string(_free_lst.back()); // collect all unnamed regs
                _free_lst.pop_back();
            } else { // rename with old mapping
                n_regs[i] = _reg_lst[reg_str];
            }
        } else if (reg_str[0] == '$') {
            n_regs[i] = reg_str.substr(1, reg_str.length()-1);
        }
    }

    // execute the renaming, update the register mapping
    for (int i=3; i>=0; i--) {
        if (n_regs[i] != "") {
            _reg_lst[string((*last_ins)[i])] = n_regs[i];
            (*last_ins)[i] = n_regs[i];
        }
    }
}