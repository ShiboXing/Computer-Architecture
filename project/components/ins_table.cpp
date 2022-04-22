#include "components.h"
#include <iostream>

extern int PC;

void ins_queue::add_ins(instruction &ins) {
    ins_q.push_front(&ins);
}
