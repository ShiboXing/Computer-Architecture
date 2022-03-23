#include <unordered_map>
#include <string>
#include <deque>
#include "../globals.h"

using namespace std;

class instruction {
    private:
        string _split_one(string ins, int *start);
        void _fill_info(vector<string> &ins);
    public:
        int _pc;
        bool is_mem;
        string _type;
        vector<string> _info;
        instruction(int pc_ind, string &ins);
};  

class res_record {
    private:
        string _op;
        vector<string> _reg_info;
    public:
        res_record(string &op_code);
};