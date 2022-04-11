#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../globals.h"
#include "../structures/structures.h"

using namespace std;

class back_writer;

// bool status: Issue, Read op, Exec Completed, Write Result
class ins_queue {
    public:
        ins_queue() { }
        // status table, maps PC to {Issue, Execute, Write Result} status 
        unordered_map<int, bool*> st_tb; 
        deque<pair<int, instruction&>> ins_q;
        deque<instruction*> issue_q;
        void add_ins(instruction &ins);
        instruction *pop_unissued();
};

class res_station {
    private:
        unordered_map<string, vector<res_record*>*> _board;
        unordered_map<string, res_record*> _deps; // dependencies
        bool _find_dep(res_record &rr); // search for dependecies (true data hazards)
        bool _execute_rec(res_record &rr);
    public:
        res_station();
        bool issue(instruction &ins);
        void execute(back_writer &bck_wrter);
};

class regs {
    public:
        float regs[32];
};

class fetcher {
    private:
        ifstream *ins_stream;
    public:
        fetcher(string fname); 
        string fetch_next();
};

class decoder {
    private:
        vector<int> _free_lst;
        unordered_map<string, string> _reg_lst;
    public:
        decoder();
        void rename(instruction &ins);
};

// common data bus
class CDB {
    private:
        int bandwidth;
        deque<res_record*> inflight;
    public:
        CDB(int bw);
        
};

class ROB {
    private: 
        int _max_entries;
        deque<res_record*> entries;
    public:
        void add_entry(res_record &rr);
};