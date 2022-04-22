#include "components.h"

decoder::decoder() {
    for (int i=31; i>=0; i--) {
       _free_lst.insert("p" + to_string(i));
    }

    decode_stream = new ofstream("decode.out");
}

void decoder::_output_mapping(vector<string> &info, vector<string> &aregs) {
    for (string i : aregs) {
        *decode_stream << i << " ";
    }
    *decode_stream << "    ";
    for (string i : info) {
        *decode_stream << i << " ";
    }
    *decode_stream << "    free_list: ";
    for (auto it=_free_lst.begin(); it!=_free_lst.end(); it++) {
        *decode_stream << *it << " ";
    }
    *decode_stream << endl;
}

bool decoder::rename(instruction &ins) {

    // need a free reg for dest
    string new_preg = *_free_lst.begin();
    _free_lst.erase(new_preg);

    vector<string> *last_ins = &ins._info;
    vector<string> aregs(*last_ins);
    string op = (*last_ins)[0];
    int first_operand = (op == "fsd" || op == "bne") ? 1 : 2;

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
    if (first_operand == 2) {
        string dest_areg = (*last_ins)[1];
        areg_2_preg[dest_areg] = new_preg;
        preg_2_areg[new_preg] = dest_areg;
        (*last_ins)[1] = new_preg;
    }

    _output_mapping(*last_ins, aregs);

    return true;
}

void decoder::print_regs() {

    ofstream reg_stream("regs.out");
    // print out registers
    reg_stream << "register content: " << endl;
    for (auto itm : areg_2_preg) {
        reg_stream  << itm.first << " (" << itm.second << "):  " << REGS[GET_REG_NUM(itm.second)] << endl;
    }
}

void decoder::free_reg(string reg) {
    if (reg[0] != 'p') // not a reg
        return;

    string areg = preg_2_areg[reg];
    if (areg_2_preg[areg] != reg) {
        _free_lst.insert(reg);
    }   
}

bool decoder::can_rename() {
    if (_free_lst.size() == 0) {
        cout << "no free register left" << endl;
        return false;
    }
    return true;
}