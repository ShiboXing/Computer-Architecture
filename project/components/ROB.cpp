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

bool ROB::commit(CDB &bus, decoder &d) {
    res_record *rr;
    bool has_entries = entries.size() > 0;
    
    while (entries.size() && (rr = entries.back())->written_back && bus.add_entry(*rr)) {
        rr->committed = true;
        
        // write value to the register (R-type)
        if (rr->_op != "fsd" && rr->_op != "bne") {
            int reg_num = GET_REG_NUM(rr->fi);
            REGS[reg_num] = rr->_result;
            
            // let decoder free register if necessary
            d.free_reg(rr->fi);
            d.free_reg(rr->fj);
            d.free_reg(rr->fk);
        }

        // pop ROB
        entries.pop_back();
    }
    
    return has_entries;
}