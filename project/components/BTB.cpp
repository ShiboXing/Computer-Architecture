#include "components.h"

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

int BTB::predict(int pc) {
    if (targets.find(pc) == targets.end() || !predicts[pc])
        return pc+1;

    return targets[pc];
}