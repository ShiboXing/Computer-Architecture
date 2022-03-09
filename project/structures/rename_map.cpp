#include "structures.h"

rename_map *rename_map::rm_ = nullptr;
rename_map *rename_map::get_map() {
    if (rm_ == nullptr) {
        rm_ = new rename_map();
    }

    return rm_;
}

