#include "structures.h"
#include <iostream>

void ins_table::add_ins(Tp &ts) {
    bool *tmp = (bool*) malloc(sizeof(false) * 4);
    memset(tmp, false, 4);
    ins[&ts] = tmp;
}