#include "components.h"

#define SORT_FREELIST(lst) sort(_free_lst.begin(), _free_lst.end(), greater<int>()); 

decoder::decoder() {
    for (int i=31; i>=0; i--) {
       _free_lst.push_back(i);
    }

    decode_stream = new ofstream("decode.out");
}

void decoder::_output_mapping(vector<string> &info) {
    for (string i : info) {
        *decode_stream << i << " ";
    }
    *decode_stream << "    free_list: ";
    for (int f : _free_lst) {
        *decode_stream << f << " ";
    }
    *decode_stream << endl;
}

bool decoder::rename(instruction &ins) {
    // get the last register
    auto last_ins = &(ins._info);
    string op = (*last_ins)[0];

    // iterate the instruction to check how many new registers are needed
    vector<string> n_regs(4);
    vector<int> cached_free_regs;
   
    for (int i=3; i>=1; i--) { 
        /* perform renaming backwards,
        consider: F0, F0, F4 */
        string reg_str = (*last_ins)[i];
        
        if (reg_str[0] == 'R' || reg_str[0] == 'F') {
            // create new mapping here
            if ((i == 1 && op != "fsd" && op != "bne") // always use a new reg for dest
                || areg_2_preg.find(reg_str) == areg_2_preg.end()) { // if mapping not found, assign a new reg

                // not enough free regs, stall!
                if (_free_lst.size() == 0) {
                    cout << "not enough free regs, reclaiming cached free regs" << "(" << cached_free_regs.size() << ")" << endl;
                    // push back unused free registers
                    _free_lst.insert(_free_lst.end(), cached_free_regs.begin(), cached_free_regs.end());
                    SORT_FREELIST(_free_lst);
                    return false;
                }

                n_regs[i] =  "p" + to_string(_free_lst.back()); // collect all unnamed regs
                cached_free_regs.push_back(_free_lst.back());
                _free_lst.pop_back();
                
            } else { // rename with old mapping
                n_regs[i] = areg_2_preg[reg_str];
            }
        } else if (reg_str[0] == '$') { // dollar sign is constant value
            n_regs[i] = reg_str.substr(1, reg_str.length()-1);
        }
    }

    // execute the renaming, update the register mapping
    for (int i=3; i>=1; i--) {
        if (n_regs[i] != "") {
            string areg = (*last_ins)[i];
            string *old_reg = &areg_2_preg[areg];
            
            // update forward mapping
            *old_reg = n_regs[i];
            (*last_ins)[i] = n_regs[i];
            // update backward mapping
            preg_2_areg[n_regs[i]] = areg;
        }
    }
    sort(_free_lst.begin(), _free_lst.end(), greater<int>()); 
    _output_mapping(*last_ins);
    
    return true;
}

void decoder::print_regs() {

    ofstream reg_stream("regs.out");
    // print out registers
    reg_stream << "register content: " << endl;
    for (auto itm : areg_2_preg) {
        int reg = GET_REG_NUM(itm.second);
        reg_stream << "p" << reg << "(" << itm.first << "):  " << REGS[reg] << endl;
    }
}

void decoder::free_reg(string reg) {
    
    string areg = preg_2_areg[reg];
    if (areg_2_preg[areg] != reg) {
        int reg_num = GET_REG_NUM(reg);
        _free_lst.push_back(reg_num);
        SORT_FREELIST(_free_lst);
    }   
}