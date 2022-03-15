#include "structures.h"
#include <iostream>

instruction::instruction(string &ins) {
    int start = 0;
    vector<string> ins_lst;
    
    // repeatedly parse ins string into a vector
    while (start < ins.length()) {
        if (ins[start] != ' ') {
            auto tmp_val = _split_one(ins, &start);
            ins_lst.push_back(tmp_val);
        }
        start++;
    }
    // for (auto i : ins_lst)
    //     cout << i << " ";
    // cout << endl;

    _fill_info(ins_lst);
    
    return;
}

void instruction::_fill_info(vector<string> &ins) {
    // check if ins is memory allocation
    if (ins.size() == 2 && ins[0][0] >= '0' && ins[0][0] <= '9') { 
        MEM[stoi(ins[0])] = stoi(ins[1]);
    } else { // RISC V ISA
        int i = 0;
        string tmp;
        if (ins[i].back() == ':') { // tag (loop: addi R0, R1, 1)
            TAG_TB[ins[i].substr(0, ins[i].length()-1)] = PC;
            i++;
        } 
        
        _info[0] = ins[i];
        _info[1] = ins[i+1];
        _info[2] = ins[i+2];
        _info[3] = ins[i+3];
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
        } else if (ins[i] == ' ' || ins[i] == ',' || ins[i] == '(' || ins[i] == ')') {
            break;
        } 
        tmp += ins[i];
    }
    *start = i;
    return tmp;
}