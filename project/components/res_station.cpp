#include "components.h"

res_station::res_station() {
    for (auto item : RES_CNT) {
        _board[item.first] = new vector<res_record*>();
    }
}

bool res_station::issue(instruction &ins) {
    if (_board[ins._type]->size() == RES_CNT[ins._type])
        return false;  

    res_record* tmp = new res_record(ins._info[0]);
    
    return true;   
}



