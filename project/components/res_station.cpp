#include "components.h"

res_station::res_station() {
    for (auto item : RES_CNT) {
        _board[item.first] = new vector<res_record*>();
        for (int i=0; i<item.second; i++) {
            _board[item.first]->push_back(new res_record());
        }
    }
}

// return whether successfully issued 
bool res_station::issue(instruction &ins) {

    // check if the corresponding function unit's reservation station is full
    string op = ins._info[0];
    string type = TYPE_MAP[op];

    for (int i=0; i<_board[type]->size(); i++) {
        res_record **r_rec = &(*_board[type])[i];
        if ((*r_rec)->cycles_left == 0) { // empty or completed res_record
            res_record* tmp = new res_record(ins._info, ins._pc);
            *r_rec = tmp; // assign to res station
            // mark the true data dependencies on its reservation station
            _find_dep(*tmp);
            _deps[tmp->fi] = tmp;
            return true;
        }
    }
    
    return false;   
}

bool res_station::_find_dep(res_record &rr) {

    if (_deps.find(rr.fj) != _deps.end())
        rr.qj = _deps[rr.fj];
    if (_deps.find(rr.fk) != _deps.end())
        rr.qk = _deps[rr.fk];

    return true;
}

void res_station::execute() {
    for (auto item : _board) {
        auto station = item.second;
        for (res_record rr : station) {
            
        }
    }
}


