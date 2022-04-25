#include "structures.h"

// empty res station record
res_record::res_record() {
    qj = qk = qmem = NULL;
    fi = fj = fk = _op = "";
    _imm = _result = .0;
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
    } else if (_op != "fld") {
        fj = info[2];
    }

    // fill in Fk (check for register in bne operands)
    if (_op == "bne") {
        fk = info[2];
    } else {
        fk = info[3];
    }
    
    // fill in immediate value
    if ( _op == "fsd" || _op == "fld") { 
        _imm = stoi(info[2]);
    }   

    // fill in tag
    if (_op == "bne") {
        tag = TAGS[info[3]];
    }

    // fill in latency
    cycles_left = INS_LAT[_op];
}

float res_record::_decode(string reg, res_record *ref) {
    int reg_num;
    if (reg.length()) {
        if (reg[0] == 'p') { // if operand needs to be initialize, return 0
            reg_num = GET_REG_NUM(reg);
        } else {
            return stof(reg);
        }
    } else { 
        return .0;
    }
    
    // check if dependecy register is already committed
    if (ref == NULL || ref->committed) {
        return REGS[reg_num];
    } else {
        assert (ref->written_back); // confirm dependency can be forwarded
        return ref->_result;
    }
}  

bool res_record::execute() {

    if (executed || (qj && !qj->written_back) || (qk && !qk->written_back)) // check if operands are resolved
        return false;
    if (_op == "fld" && (!qmem || (qmem != this && qmem->written_back))) // check fld for its mem dependency
        return false;

    if (cycles_left == 1) {
        float fj_res = _decode(fj, qj);
        float fk_res = _decode(fk, qk);

        if (_op == "add" || _op == "fadd" || _op == "addi") {
            _result = fj_res + fk_res;
        } else if (_op == "fsub") {
            _result = fj_res - fk_res;
        } else if (_op == "fmul") {
            _result = fj_res * fk_res;
        } else if (_op == "fdiv") {
            _result = fj_res / fk_res;
        } else if (_op == "fld") {
            if (qmem == this) { // no mem dependecy
                _result = MEM[get_mem_addr()];
            } else {
                _result = qmem->_result;
            }
        } else if (_op == "fsd") {  
            _result = fj_res;
        } else if (_op == "bne") {
            if (fj_res == fk_res) {
                tag = _pc+1;
            }
        }
        
        return executed = true;
    } else {
        cycles_left--;
    }
    
    return false;
}

int res_record::get_mem_addr() {
    assert (_op == "fld" || _op == "fsd");
    assert (!qk || qk->written_back);

    return int(_decode(fk, qk)) + _imm;
}