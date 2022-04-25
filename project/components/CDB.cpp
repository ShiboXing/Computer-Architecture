#include "components.h"

CDB::CDB(int bw) {
    bandwidth = bw;
}

bool CDB::add_entry(res_record &rr) {
    if (inflight.size() < bandwidth) {
        inflight.push_front(&rr);
        return true;
    }
    cout << "[CYCLE: " << CYCLE << "] CDB STALL" << endl;
    return false;
}

void CDB::flush() {
    while (inflight.size()) {
        inflight.pop_back();
    }
}