#include "components.h"

// initialize the station of each functional unit, which is a vector of res_records
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

// find a res_record whose executed flag is true, and replace it with the new one containing the instruction
bool res_station::issue(instruction &ins, ROB &rob) {

    // check if the corresponding function unit's reservation station is full
    string op = ins._info[0];
    string type = TYPE_MAP[op];

    for (int i=0; i<_board[type]->size(); i++) {
        res_record *r_rec = (*_board[type])[i];
        if (r_rec->executed) { // empty res_record

            res_record* tmp = new res_record(ins._info, ins._pc);
            (*_board[type])[i] = tmp; 
            
            
            rob.find_dep(*tmp); // mark the dependencies in record
            rob.add_entry(*tmp); // put it in rob
            return true;
        }
    }
    
    // reservation stations is full
    return false;   
}

// let each the res_record execute if it is possible
bool res_station::execute(back_writer &bck_wrter, ROB &rob, bool *branch_executed) {
    bool has_records = false;
    for (auto item : _board) {
        auto station = item.second;
        for (res_record *rr : *station) {
            has_records |= !rr->executed;
            rob.find_mem_dep(*rr);
            if (rr->execute()) {
                bck_wrter.add_entry(*rr);
                if (rr->_op == "bne")
                    *branch_executed = true;
            }
        }
    }
    
    return has_records;
}

// add the instruction to its corresponding reservation station, if it's not full
bool res_station::can_issue(instruction &ins) {
    string op = ins._info[0];
    string type = TYPE_MAP[op];
    
    for (int i=0; i<_board[type]->size(); i++) {
        res_record *r_rec = (*_board[type])[i];
        if (r_rec->executed) { 
            return true;
        }
    }

    cout << "[CYCLE: " << CYCLE << "] " << type << " RESERVATION STALL" << endl;
    RES_STALL++;
    return false;
}
