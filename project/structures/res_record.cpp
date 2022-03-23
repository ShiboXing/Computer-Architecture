#include "structures.h"

res_record::res_record(string &op_code) {
    _op = op_code;
    // Vj, Vk, Qj, Qk, A
    for (int i=0; i<5; i++) {  
        _reg_info.push_back("");
    }
}