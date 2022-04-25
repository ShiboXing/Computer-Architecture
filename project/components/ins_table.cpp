#include "components.h"
#include <iostream>

extern int PC;

// add instruction to instruction queue
void ins_queue::add_ins(instruction &ins) {
    ins_q.push_front(&ins);
}

// empty the queue upon flushing
void ins_queue::branch_flush() {
    ins_q.clear();
}