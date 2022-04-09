#include "components.h"
#include <iostream>

extern int PC;

void ins_queue::add_ins(instruction &ins) {
    bool tmp[3] = {false, false, false}; // Issue, Execute, Write Result
    st_tb[ins._pc] = tmp;
    ins_q.push_front({ins._pc, ins});
    issue_q.push_front(&ins);
}

instruction *ins_queue::pop_unissued() {
    
    instruction *ins = issue_q.back();
    issue_q.pop_back();
    st_tb[ins->_pc][0] = true;
    
    return ins;
}
