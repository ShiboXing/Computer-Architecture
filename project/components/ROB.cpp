#include "components.h"
#include <assert.h>

ROB::ROB(int nr) {
    _max_entries = nr;
}

bool ROB::is_full() {
    auto q_size = entries.size();
    assert (q_size <= _max_entries);
    return q_size == _max_entries;
}

void ROB::add_entry(res_record &rr) { 
    assert (!is_full());
    entries.push_front(&rr);
}

bool ROB::commit(CDB &bus) {
    res_record *rr;
    bool has_entries = entries.size() > 0;
    
    while (entries.size() && (rr = entries.back())->written_back && bus.add_entry(*rr)) {
        rr->committed = true;
        
        // write value to the register (R-type)
        if (rr->_op == "add" || 
            rr->_op == "addi" ||
            rr->_op == "fadd" ||
            rr->_op == "fsub" ||
            rr->_op == "fmul" || 
            rr->_op == "fdiv") {
            int reg_num = stoi(rr->fi.substr(1, rr->fi.length()-1));
            REGS[reg_num] = rr->_result;
        }
            
        // pop ROB
        entries.pop_back();
    }
    
    return has_entries;
}