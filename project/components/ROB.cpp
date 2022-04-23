#include "components.h"
#include <assert.h>

ROB::ROB(int nr) {
    _max_entries = nr;
}

bool ROB::is_full() {
    auto q_size = entries.size();
    assert (q_size <= _max_entries);
    if (q_size == _max_entries) {
        cout << "ROB is full!" << endl;
        return true;
    }

    return false;
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
        if (rr->_op != "bne") {
            if (rr->_op == "fsd") {
                MEM[rr->get_mem_addr()] = rr->_result;
            } else {
                int reg_num = GET_REG_NUM(rr->fi);
                REGS[reg_num] = rr->_result;

                d.update_commit(rr->fi, true); // let decoder know dest reg is committed
            }
        } 

        free_dep(rr->qj);
        free_dep(rr->qk);

        // pop ROB
        entries.pop_back();
    }
    
    return has_entries;
}


void ROB::find_dep(res_record &rr) {
    for (auto rob_rr : entries) { // front to back 
        if (rr.qj == NULL && rob_rr->fi == rr.fj) // don't overwrite if already filled
            rr.qj = rob_rr;
        if (rr.qk == NULL && rob_rr->fi == rr.fk) 
            rr.qk = rob_rr;
    }

    add_ref(rr.qj);
    add_ref(rr.qk);
}

void ROB::free_dep(res_record *rr) {
    if (rr == NULL) 
        return;

    garbage_collector[rr]--;
    if (garbage_collector[rr] == 0) {
        garbage_collector.erase(rr);
        delete rr; // free dependecy rr 
    }
}

void ROB::add_ref(res_record *rr) {
    if (rr) {
        if (garbage_collector.find(rr) == garbage_collector.end()) {
            garbage_collector[rr] = 1;
        } else {
            garbage_collector[rr]++;
        }
    }
}

void ROB::find_mem_dep(res_record &rr) {
    if (rr._op != "fld" || (rr.qk && !rr.qk->written_back) || rr.qmem != NULL)  // not load || operands are not ready || mem dependency found or no dependency
        return;

    bool started = false;
    bool has_dep = false;
    for (res_record *rob_rr : entries) {
        if (!started) {
            if (rob_rr == &rr) {
                started = true;
            }
        } else if (rob_rr->_op == "fsd") {
            if (rob_rr->fk[0] != 'p' && rr.get_mem_addr() == rob_rr->get_mem_addr()) {
                rr.qmem = rob_rr;
            } else {
                has_dep = true;
            }
        }
    }

    if (!has_dep) 
        rr.qmem = &rr;
}
