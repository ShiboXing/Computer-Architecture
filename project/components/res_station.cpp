#include "components.h"

res_station::res_station() {
    for (auto item : RES_CNT) {
        _board[item.first] = new vector<res_record*>();
        for (int i=0; i<item.second; i++) {
            res_record *tmp = new res_record();
            tmp->executed = tmp->written_back = tmp->committed = true; // mark for replacement
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
        if (r_rec->executed) { // empty res_record

            if (r_rec->committed) // delete ONLY when commited, for it could be in other components
                delete r_rec;

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

bool res_station::execute(back_writer &bck_wrter) {
    bool has_records = false;

    for (auto item : _board) {
        auto station = item.second;
        for (res_record *rr : *station) {
            has_records |= !rr->executed;
            if (rr->execute())
                bck_wrter.add_entry(*rr);
        }
    }
    
    return has_records;
}

