#include "structures.h"

// empty res station record
res_record::res_record() {
    qj = qk = NULL;
    fi = fj = fk = _op =  "";
    _imm = _result = 0;
    executed = written_back = committed = false;
}

res_record::res_record(vector<string> &info, int pc_ind) : res_record() {

    _pc = pc_ind;
    _op = info[0];
    qj = qk = NULL;
    written_back = committed = false;

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

float res_record::_decode(string reg, res_record *ref) {
    int reg_num;
    if (reg[0] == 'p') // decode if it is a physical register 
        reg_num = stoi(reg.substr(1, reg.size()-1));
    else 
        return .0;
    
    // check if dependecy register is already committed
    if (ref == NULL || !ref->committed)
        return REGS[reg_num];
    else
        return ref->_result;
}  

bool res_record::execute() {
      
    // dependencies are not resolved or has executed
    if (executed || (qj && !qj->written_back) || (qk && !qk->written_back)) {
        return false;
    }

    if (cycles_left == 0) {
        float fj_num = _decode(fj, qj);
        float fk_num = _decode(fk, qk);

        if (_op == "add" || _op == "fadd") {
            _result = fj_num + fk_num;
        } else if (_op == "addi" ) {
            _result = fj_num + _imm;
        } else if (_op == "fsub") {
            _result = fj_num - fk_num;
        } else if (_op == "fmul") {
            _result = fj_num * fk_num;
        } else if (_op == "fdiv") {
            _result = fj_num / fk_num;
        } else if (_op == "fld") {
            _result = MEM[fj_num + _imm];
        }
    
        return executed = true;
    } else {
        cycles_left--;
    }
    
    return false;
}
