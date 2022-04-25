#include "components.h"

// add branch target and the branch outcome to BTB mapping
void BTB::write_entry(int pc, int target) {
    assert (targets.size() == predicts.size());

    if (target == pc+1) {
        if (targets.find(pc) != targets.end()) {
            predicts[pc] = false;
        }
    } else {
        targets[pc] = target;
        predicts[pc] = true;
    }

    assert (targets.size() == predicts.size() && targets.size() <= max_size);
}

// fetch prediction result from BTB
int BTB::predict(int pc) {
    if (targets.find(pc) == targets.end() || !predicts[pc])
        return pc+1;

    return targets[pc];
}