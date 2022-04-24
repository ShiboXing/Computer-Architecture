#include "components.h"

void BTB::write_entry(int pc, int target) {
    assert (targets.size() == predicts.size());
    
    targets[pc] = target;
    predicts[pc] = true;
}

int BTB::predict(int pc) {
    if (targets.find(pc) == targets.end())
        return pc+1;
        
    return targets[pc];
}