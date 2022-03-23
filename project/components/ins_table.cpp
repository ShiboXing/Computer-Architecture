#include "components.h"
#include <iostream>

extern int PC;

void ins_queue::add_ins(instruction &ins, int pc_ind) {
    bool tmp[4] = {false, false, false}; // Issue, Execute, Write Result
    st_tb[pc_ind] = tmp;
    ins_q.push_front({pc_ind, ins});
}

// instruction &ins_table::operator[](int ind) {
//     return ins_q[ind].second;
// }

