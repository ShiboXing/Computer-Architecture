#include "components.h"

decoder::decoder() {
    for (int i=31; i>=0; i--) {
       _free_lst.push_back(i);
    }
}

void decoder::rename(instruction &ins) {
    // get the last register
    auto last_ins = &(ins._info);
    string op = (*last_ins)[0];

    // iterate the instruction to check how many new registers are needed
    vector<string> n_regs(4);
    vector<int> cached_free_regs;
   
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
                    cout << "not enough free regs, reclaiming cached free regs" << "(" << cached_free_regs.size() << ")" << endl;
                    _free_lst.insert(_free_lst.end(), cached_free_regs.begin(), cached_free_regs.end());
                    sort(_free_lst.begin(), _free_lst.end());
                    return;
                }
                n_regs[i] =  "p" + to_string(_free_lst.back()); // collect all unnamed regs
                cached_free_regs.push_back(_free_lst.back());
                _free_lst.pop_back();
            } else { // rename with old mapping
                n_regs[i] = _reg_lst[reg_str];
            }
        } else if (reg_str[0] == '$') { // dollar sign is constant value
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