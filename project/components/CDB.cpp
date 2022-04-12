#include "components.h"

CDB::CDB(int bw) {
    bandwidth = bw;
}

bool CDB::add_entry(res_record &rr) {
    if (inflight.size() < bandwidth) {
        inflight.push_front(&rr);
        return true;
    }

    return false;
}