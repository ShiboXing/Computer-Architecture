#include "components.h"
#include <iostream>

extern int PC;

void ins_queue::add_ins(instruction &ins) {
    ins_q.push_front(&ins);
}

instruction *ins_queue::pop_unissued() {
    instruction *ins = ins_q.back();
    ins_q.pop_back();
    
    return ins;
}
