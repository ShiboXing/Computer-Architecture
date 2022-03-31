#include "structures.h"

res_record::res_record(vector<string> &info) {

    // fill op code
    _op = info[0];

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

    // fill in Fk (guard for register in bne operands)
    if ((_op != "addi" && _op != "bne") || (info[2][0] == 'p')) {
        fk = info[3];
    }
    
    // fill in immediate value
    if (_op == "addi") {
        _imm = stof(info[3]);
    } else if ( _op == "fsd" || _op == "fld" || _op == "bne") {
        if (info[2][0] == 'p') // guard for register in bne operands
            _imm = stof(info[2]);
    }   
}