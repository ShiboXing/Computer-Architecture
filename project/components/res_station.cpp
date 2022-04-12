#include "components.h"

res_station::res_station() {
    for (auto item : RES_CNT) {
        _board[item.first] = new vector<res_record*>();
        for (int i=0; i<item.second; i++) {
            res_record *tmp = new res_record();
            tmp->valid = false;
            _board[item.first]->push_back(tmp);
        }
    }
}

// return whether successfully issued 
bool res_station::issue(instruction &ins, ROB &rob) {

    // check if the corresponding function unit's reservation station is full
    string op = ins._info[0];
    string type = TYPE_MAP[op];

    for (int i=0; i<_board[type]->size(); i++) {
        res_record *r_rec = (*_board[type])[i];
        if (!r_rec->valid) { // empty res_record
            delete r_rec; // delete old res_record
            res_record* tmp = new res_record(ins._info, ins._pc);
            (*_board[type])[i] = tmp; 
            
            // mark the true data dependencies on its reservation station
            _find_dep(*tmp, rob);

            // push it into ROB
            rob.add_entry(*tmp);
            return true;
        }
    }
    
    // reservation stations is ful
    return false;   
}

bool res_station::_find_dep(res_record &rr, ROB &rob) {

    for (res_record *rob_rr : rob.entries) {
        if (rr.qj == NULL && rob_rr->fi == rr.fj)
            rr.qj = rob_rr;
        if (rr.qk == NULL && rob_rr->fi == rr.fk) 
            rr.qk = rob_rr;
    }
    
    if (_deps.find(rr.fj) != _deps.end())
        rr.qj = _deps[rr.fj];
    if (_deps.find(rr.fk) != _deps.end())
        rr.qk = _deps[rr.fk];

    return true;
}

void res_station::execute(CDB &bus) {
    for (auto item : _board) {
        auto station = item.second;
        for (res_record *rr : *station) {
            rr->execute();
        }
    }
}

