#include "components.h"

void BTB::write_entry(int pc, int target, bool outcome, int cycle) {
    assert (targets.size() == predicts.size() && \
        targets.size() == lru_stamps.size() && \
        predicts.size() == lru_stamps.size());
    
    
    if (targets.size() == max_size) {
        auto it = min_element(lru_stamps.begin(), lru_stamps.end(), \
            [](const auto& l, const auto& r) { return l.second < r.second; });
        targets.erase(it->first);
        predicts.erase(it->first);
        lru_stamps.erase(it->first);
        
    }
    targets[pc] = target;
    predicts[pc] = outcome;
    lru_stamps[pc] = cycle;
}  