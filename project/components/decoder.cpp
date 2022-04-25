#include "components.h"

decoder::decoder() {
    for (int i=31; i>=0; i--) {
       _free_lst.push_back("p" + to_string(i));
    }

    decode_stream = new ofstream("decode.out");
}

void decoder::_output_mapping(vector<string> &info, vector<string> &aregs, bool is_branch, int pc) {
    *decode_stream << "[CYCLE " << CYCLE << "] ";
    for (string i : aregs) {
        *decode_stream << i << " ";
    }
    *decode_stream << "    ";
    for (string i : info) {
        *decode_stream << i << " ";
    }
    *decode_stream << "    free_list: ";
    for (string reg : _free_lst) {
        *decode_stream << reg << " ";
    }
    *decode_stream << endl;
    if (is_branch) {
        cout << "\033[32m ========== BRANCH [PC: " << pc << "]========== \033[0m" << endl;
        *decode_stream << "\033[32m ========== BRANCH [PC: " << pc << "]========== \033[0m" << endl;
    }
}

bool decoder::rename(instruction &ins) {

    // need a free reg for dest
    string new_preg = _free_lst.back();

    vector<string> *last_ins = &ins._info;
    vector<string> aregs(*last_ins);
    string op = (*last_ins)[0];
    int first_operand = (op == "fsd" || op == "bne") ? 1 : 2;

    // name operands if mapping exists
    for (int i=first_operand; i<4; i++) {
        string op_areg = (*last_ins)[i];
        if (op_areg[0] == 'R' || op_areg[0] == 'F') {
            if (areg_2_preg.find(op_areg) == areg_2_preg.end()) { // no previous mapping, operand has no value
                (*last_ins)[i] = "0";
            } else { // use previous mapping, operand has value
                (*last_ins)[i] = areg_2_preg[op_areg];
            }
        } else if (op_areg[0] == '$') {
            (*last_ins)[i] = to_string(GET_REG_NUM(op_areg));
        }
    }
    
    // rename dest
    if (first_operand == 2) {
        string dest_areg = (*last_ins)[1];
        areg_2_preg[dest_areg] = new_preg;
        preg_2_areg[new_preg] = dest_areg;
        (*last_ins)[1] = new_preg;
        
        _free_lst.pop_back(); // remove the new reg from free list
        update_commit(new_preg, false);
    }

    _output_mapping(*last_ins, aregs, op == "bne", ins._pc);

    if (op == "bne") {
        f_snapshots[ins._pc] = _free_lst;
        a_snapshots[ins._pc] = areg_2_preg;
        p_snapshots[ins._pc] = preg_2_areg;
    }

    return true;
}

void decoder::flush_mappings(int pc) {

    _free_lst = f_snapshots[pc];
    areg_2_preg = a_snapshots[pc];
    preg_2_areg = p_snapshots[pc];

    f_snapshots.erase(pc);
    a_snapshots.erase(pc);
    p_snapshots.erase(pc);
    
    cout << "\033[31m ========== FLUSH [PC: " << pc << "]=========== \033[0m" << endl;
    *decode_stream << "\033[31m ========== FLUSH [PC: " << pc << "]=========== \033[0m" << endl;
}

void decoder::update_commit(string reg, bool committed) {
    if (!reg.length())
        return;
    assert (reg[0] == 'p');
    commit_status[reg] = committed;
}

void decoder::print_regs() {

    ofstream reg_stream("regs.out");
    // print out registers
    reg_stream << "register content: " << endl;
    for (auto itm : areg_2_preg) {
        reg_stream  << itm.first << " (" << itm.second << "):  " << REGS[GET_REG_NUM(itm.second)] << endl;
    }
}

void decoder::free_regs() {
    for (auto itm : preg_2_areg) {
        if (commit_status[itm.first] && itm.first != areg_2_preg[itm.second]) {
            preg_2_areg.erase(itm.first);
            _free_lst.push_back(itm.first); 
        }
    }
    
    sort(_free_lst.begin(), _free_lst.end(), [](const string a, const string b) {
        if (a.length() > b.length())
            return true;
        else if (a.length() < b.length())
            return false;
        else 
            return a > b;
   }); 
}

bool decoder::can_rename() {
    if (_free_lst.size() == 0) {
        cout << "[CYCLE: " << CYCLE << "] DECODE STALL" << endl;
        DECODE_STALL++;
        return false;
    }
    return true;
}