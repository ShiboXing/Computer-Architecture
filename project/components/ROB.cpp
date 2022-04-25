#include "components.h"
#include <assert.h>

ROB::ROB(int nr) {
    _max_entries = nr;
}

// stall if no room in ROB
bool ROB::is_full() {
    auto q_size = entries.size();
    assert (q_size <= _max_entries);
    if (q_size == _max_entries) {
        cout << "[CYCLE: " << CYCLE << "] ROB STALL" << endl;
        ROB_STALL++;
        return true;
    }

    return false;
}

// add one res_record to the ROB queue
void ROB::add_entry(res_record &rr) { 
    assert (!is_full());
    entries.push_front(&rr);
}

// send res_record to CDB, update commit status
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

        // pop ROB
        entries.pop_back();
    }
    
    return has_entries;
}

// sequentially mark dependencies 
void ROB::find_dep(res_record &rr) {
    for (auto rob_rr : entries) { // front to back 
        if (rr.qj == NULL && rob_rr->fi == rr.fj) // don't overwrite if already filled
            rr.qj = rob_rr;
        if (rr.qk == NULL && rob_rr->fi == rr.fk) 
            rr.qk = rob_rr;
    }
}

// sequentially mark memory dependencies for the mem instructions whose operands are resolved
void ROB::find_mem_dep(res_record &rr) {
    if (rr._op != "fld" || (rr.qk && !rr.qk->written_back) || rr.qmem != NULL)  // not load || operands are not ready || mem dependency resolved
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

// check if the newly executed branch's target is matched with the following instrution's pc
void ROB::branch_flush(decoder &d, ins_queue &q, fetcher &f, BTB &btb) {
    
    int i = entries.size()-1;
    res_record *rr = NULL;
    for (; i>=0; i--) {
        // locate the mis-predicted branch from back to front 
        /**
         * @brief conditions of miss prediction:
         *  1. the pc of the ins following a branch ins doesn't match its resolved destination in ROB
         *  2. the pc of the back ins of the ins_q doesn't match the branch ins at the front of ROB
         *  3. a branch ins at ROB front will be taken, whereas no ins is found in ins_q (EOF ins is a branch)
         */
        if (entries[i]->_op == "bne" && entries[i]->executed && ( \
                (i-1>=0 && entries[i]->tag != entries[i-1]->_pc) || \
                (i==0 && q.ins_q.size() && entries[i]->tag != q.ins_q.back()->_pc) || \
                (i==0 && entries[i]->tag!=entries[i]->_pc+1 && q.ins_q.size()==0))
            ) {

            rr = entries[i];
            // flush ROB
            while (entries.front() != rr) {
                entries.front()->executed = entries.front()->committed = entries.front()->committed = true; // free them in res_station too
                d.update_commit(rr->fi, true); // let decoder know dest reg is committed
                entries.pop_front();
            }
                    
            // flush & restore decoder mappings
            d.flush_mappings(rr->_pc);

            // flush incorrectly fetched insturctions
            q.branch_flush();

            // update BTB
            btb.write_entry(rr->_pc, rr->tag);

            // set the PC to correctly position
            f.set_pc(rr->tag); 
            return;
        }
    }
}
