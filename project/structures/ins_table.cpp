#include "structures.h"
#include <iostream>

extern int PC;

void ins_table::add_ins(instruction &ins, int pc_ind) {
    bool tmp[4] = {false, false, false, false};
    st_tb[pc_ind] = tmp;
    ins_q.push_front({pc_ind, ins});
}

// instruction &ins_table::operator[](int ind) {
//     return ins_q[ind].second;
// }

