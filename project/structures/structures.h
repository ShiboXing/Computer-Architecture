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
        vector<string> _info;

        instruction(int pc_ind, string &ins);
};  

class res_record {
    public:
        string fi;
        string fj;
        string fk;
        res_record *qj;
        res_record *qk;
        string _op;
        float _imm;
        int _pc;
        int tag;
        int cycles_left;

        res_record(vector<string> &info, int pc_ind);
        res_record();

        bool execute();
};