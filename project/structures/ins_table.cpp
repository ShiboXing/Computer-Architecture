#include "structures.h"
#include <iostream>

void ins_table::add_status(Tp &ts) {
    bool *tmp = (bool*) malloc(sizeof(false) * 4);
    memset(tmp, false, 4);
    st_tb[&ts] = tmp;
}