#include "structures.h"
#include <iostream>

instruction::instruction(int pc_ind, string &ins) {
    initialize_operand1 = initialize_operand2 = "";
    _pc = pc_ind;
    int start = 0;
    vector<string> ins_lst;
    is_mem = false;
    
    // repeatedly parse ins string into a vector
    while (start < ins.length()) {
        if (ins[start] != ' ') {
            string tmp_val = _split_one(ins, &start);
            ins_lst.push_back(tmp_val);
        }
        start++;
    }

    _fill_info(ins_lst);
}

void instruction::_fill_info(vector<string> &ins) {
    // check if ins is memory allocation
    if (ins.size() == 2 && ins[0][0] >= '0' && ins[0][0] <= '9') { 
        MEM[stoi(ins[0])] = stoi(ins[1]);
        is_mem = true;
    } else { // RISC V ISA
        int i = 0;
        string tmp;
        if (ins[i].back() == ':') { // tag (loop: addi R0, R1, 1)
            TAG_TB[ins[i].substr(0, ins[i].length()-1)] = _pc;
            i++;
        } 

        _info.push_back(ins[i]);
        _info.push_back(ins[i+1]);
        _info.push_back(ins[i+2]);
        _info.push_back(ins[i+3]);
    }
}

// get the next parsable symbol in instruction string, from index 'start'
// then returns the next end index of the next parsable symbol
string instruction::_split_one(string ins, int *start) {
    string tmp = "";
    int i;
    for (i=*start; i<ins.length(); i++) {
        if (ins[i] == ':') { // tag
            tmp += ins[i];
            break;
        } else if (ins[i] == ' ' || ins[i] == ',' || ins[i] == '(' || ins[i] == ')' || ins[i] == '\t') {
            break;
        } 
        tmp += ins[i];
    }
    *start = i;
    return tmp;
}