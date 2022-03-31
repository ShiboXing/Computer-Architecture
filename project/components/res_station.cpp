#include "components.h"

res_station::res_station() {
    for (auto item : RES_CNT) {
        _board[item.first] = new vector<res_record*>();
    }
}

// return whether successfully issued 
bool res_station::issue(instruction &ins) {
    if (_board[ins._type]->size() == RES_CNT[ins._type])
        return false;  
    
    // check if the corresponding function unit's reservation station is full
    string op = ins._info[0];
    string type = TYPE_MAP[op];
    if (_board[type]->size() == RES_CNT[type]) {
        cout << type << " reservation is full" << endl;
        return false;
    }
    
    // insert record into corresponding function unit's res station
    res_record* tmp = new res_record(ins._info);
    _board[type]->push_back(tmp);

    // mark the true data dependencies on its reservation station
    find_dep(*tmp);
    
    return true;   
}

bool res_station::find_dep(res_record  &rr) {

    // iterate the board and find dependency
    for (auto &item: _board) {
        for (res_record *tmp_rec: *item.second) {
            if (tmp_rec->fi == rr.fj) {
                rr.qj = item.first;
                rr.rj = false;
            }
            if (tmp_rec->fi == rr.fk) {
                rr.qk = item.first;
                rr.rj = false;
            }
        }
    } 
    
    return true;
}



