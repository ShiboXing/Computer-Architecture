#include "structures.h"

res_record::res_record(vector<string> &info, int pc_ind) {

    _pc = pc_ind;
    _op = info[0];
    qj = qk = NULL;

    // fill in Fi 
    if (_op != "bne" && _op != "fsd") { 
        fi = info[1];
    }

    // fill in Fj
    if (_op == "bne" || _op == "fsd") {
        fj = info[1];
    } else if (_op == "fld") {
        fj = info[3];
    } else {
        fj = info[2];
    }

    // fill in Fk (check for register in bne operands)
    if (_op == "bne") {
        if (info[2][0] == 'p') {
            fk = info[2];
        }
    } else if (_op != "fld" && _op != "fsd" && info[3][0] == 'p') {
        fk = info[3];
    }
    
    // fill in immediate value
    if (_op == "addi") {
        _imm = stof(info[3]);                   // guard for reg in bne
    } else if ( _op == "fsd" || _op == "fld" || (_op == "bne" && info[2][0] != 'p')) { 
            _imm = stof(info[2]);
    }   

    // fill in tag
    if (_op == "bne") {
        tag = TAG_TB[info[3]];
    }

    // fill in latency
    cycles_left = INS_LAT[_op];
}

// empty res station record
res_record::res_record() {
    cycles_left = 0;
    qj = qk = NULL;
}