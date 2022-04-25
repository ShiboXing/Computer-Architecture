#include "components.h"

CDB::CDB(int bw) {
    bandwidth = bw;
}
// add to bus if not full
bool CDB::add_entry(res_record &rr) {
    if (inflight.size() < bandwidth) {
        inflight.push_front(&rr);
        return true;
    }
    cout << "[CYCLE: " << CYCLE << "] CDB STALL" << endl;
    return false;
}

// clear the bus by commiting all its instructions
void CDB::flush() {
    while (inflight.size()) {
        inflight.pop_back();
    }
}