#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../globals.h"
#include "../structures/structures.h"

using namespace std;

// bool status: Issue, Read op, Exec Completed, Write Result
class ins_queue {
    public:
        ins_queue() { }
        // status table, maps PC to {Issue, Execute, Write Result} status 
        unordered_map<int, bool*> st_tb; 
        deque<pair<int, instruction&> > ins_q;
        void add_ins(instruction &ins, int pc_ind);
        
        // instruction& operator[](int);
};

class res_station {
    private:
        unordered_map<string, vector<res_record*>*> _board;
        bool find_dep(res_record &rr); // search for dependecies (true data hazards)
        int num_recs;
    public:
        res_station();
        bool issue(instruction &ins);
};

class reg_status {
    public:
        float regs[32];
};

class fetcher {
    private:
        ifstream *ins_stream;
    public:
        int _fn;
        fetcher(int fn, string fname) {
            _fn = fn;
            ins_stream = new ifstream(fname);
        }
        
        string fetch_next();
};

class decoder {
    private:
        vector<int> _free_lst;
        unordered_map<string, string> _reg_lst;
        ins_queue &ins_tb;
    public:
        decoder(ins_queue &ins_tb_ref);
        void rename_last();
};
