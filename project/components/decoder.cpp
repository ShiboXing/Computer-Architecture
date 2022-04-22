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

    // need a free reg for dest
    string new_preg = "p" + to_string(_free_lst.back());
    _free_lst.pop_back();

    auto last_ins = &(ins._info);
    string op = (*last_ins)[0];
    int first_operand = 2;

    // name operands if mapping exists
    for (int i=first_operand; i<4; i++) {
        string dest_areg = (*last_ins)[i];
        if (dest_areg[0] == 'R' || dest_areg[0] == 'F') {
            if (areg_2_preg.find(dest_areg) == areg_2_preg.end()) { // no previous mapping, operand has no value
                (*last_ins)[i] = "0";
            } else { // use previous mapping, operand has value
                (*last_ins)[i] = areg_2_preg[dest_areg];
            }
        }
    }
    
    // rename dest
    if (op != "fsd" && op != "bne") {
        string dest_areg = (*last_ins)[1];
        areg_2_preg[dest_areg] = new_preg;
        preg_2_areg[new_preg] = dest_areg;
        (*last_ins)[1] = new_preg;
    } else {
        first_operand = 1;
    }

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

bool decoder::can_rename() {
    if (_free_lst.size() == 0) {
        cout << "not enough free regs, reclaiming cached free regs" << endl;
        return false;
    }
    return true;
}