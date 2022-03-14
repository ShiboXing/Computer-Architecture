#include "structures.h"
#include <iostream>

extern int PC;

void ins_table::add_status(string ins) {
    bool *tmp = (bool*) malloc(sizeof(false) * 4);
    memset(tmp, false, 4);
    st_tb[PC] = tmp;
}