#include <unordered_map>
#include <string>
#include <deque>
#include "../globals.h"

using namespace std;

/**
    singleton register hash map
*/
class instruction {
    private:
        string _split_one(string ins, int *start);
        void _fill_info(vector<string> &ins);
    public:
        bool is_mem;
        vector<string> _info;
        instruction(string &ins);
};  

// bool status: Issue, Read op, Exec Completed, Write Result
class ins_table {
    public:
        ins_table() { }
        unordered_map<int, bool*> st_tb; 
        deque<pair<int, instruction&> > ins_q;
        void add_ins(instruction &ins, int pc_ind);
        
        // instruction& operator[](int);
};