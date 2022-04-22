#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../globals.h"
#include "../structures/structures.h"

using namespace std;

class CDB;
class ROB;
class back_writer;

class ins_queue {
    public:
        deque<instruction*> ins_q;
        void add_ins(instruction &ins);
        instruction *pop_unissued();
};

class res_station {
    private:
        unordered_map<string, vector<res_record*>*> _board;
        bool _find_dep(res_record &rr, ROB &rob); // search for dependecies (true data hazards)
        bool _execute_rec(res_record &rr);
    public:
        res_station();
        bool issue(instruction &ins, ROB &rob);
        bool execute(back_writer &bck_wrter);
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
        ofstream *decode_stream;
        vector<int> _free_lst;
        unordered_map<string, string> areg_2_preg;
        unordered_map<string, string> preg_2_areg;
        void _output_mapping(vector<string> &info);
    public:
        decoder();
        void print_regs();
        bool rename(instruction &ins);
        void free_reg(string reg);
};

class back_writer {
    private:
        deque<res_record*> entries;
    public:
        void add_entry(res_record &rr);
        bool write_back(CDB &bus);
};

// common data bus
class CDB {
    private:
        int bandwidth;
        deque<res_record*> inflight;
    public:
        CDB(int bw);
        bool add_entry(res_record &rr);
        void flush();
};

class ROB {
    private: 
        int _max_entries;
    public:
        deque<res_record*> entries;
        ROB(int nr);
        bool is_full();
        bool commit(CDB &bus, decoder &d);
        void add_entry(res_record &rr);
};
