#include "structures.h"

void back_writer::add_result(res_record &rr) {
    _buffer.push_front(&rr);
}