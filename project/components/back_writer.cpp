#include "components.h"

// attempt to send to bus
bool back_writer::write_back(CDB &bus) {
    bool has_entries = entries.size() > 0;
    
    // iterate from the head of the queue to write back res_record
    for (auto it = entries.rbegin(); it != entries.rend(); it++) { 
        if (!(*it)->written_back && bus.add_entry(**it)) {
            (*it)->written_back = true;
        }
        if ((*it)->written_back && *it == entries.back()) {
            entries.pop_back();
        }
    }

    return has_entries;   
}

// add to the write back buffer
void back_writer::add_entry(res_record &res) {
    entries.push_front(&res);
}