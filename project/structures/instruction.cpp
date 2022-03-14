#include "structures.h"
#include <iostream>

instruction::instruction(string &ins) {
    int start = 0;
    
    while (start < ins.length()) {
        string tmp_val = "";
        if (ins[start] != ' ') {
            start = _split_one(ins, &tmp_val, start);
            cout << tmp_val << endl;
        }
        start++;
        
    }

}

int instruction::_split_one(string ins, string *tmp_val, int start=0) {
    string tmp = "";
    int i;
    for (i=start; i<ins.length(); i++) {
        tmp.append(ins[i], 1);
        if (ins[i] == ':') {
            TAG_TB[PC] = tmp;
            *tmp_val = tmp;
            return i+1;
        } else if (ins[i] == ' ' || ins[i] == ',' || ins[i] == '(' || ins[i] == ')') {
            *tmp_val = tmp;
            return i+1;
        } 
    }

    return i;
}