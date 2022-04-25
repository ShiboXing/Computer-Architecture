#include <vector>
#include <set>
#include "../structures/structures.h"

using namespace std;

class CDB;
class ROB;
class BTB;
class back_writer;

class ins_queue {
    public:
        deque<instruction*> ins_q;
        void add_ins(instruction &ins);
        void branch_flush();
};

class fetcher {
    private:
        ifstream *ins_stream;
    public:
        string fetch_next();
        void add_codepth(string fname); 
        void set_pc(int pc);
        void scan_tags();
};

class res_station {
    private:
        unordered_map<string, vector<res_record*>*> _board;
        bool _execute_rec(res_record &rr);
    public:
        res_station();
        bool can_issue(instruction &ins);
        bool issue(instruction &ins, ROB &rob);
        bool execute(back_writer &bck_wrter, ROB &rob);
};

class decoder {
    private:
        ofstream *decode_stream;
        vector<string> _free_lst;
        unordered_map<string, string> areg_2_preg;
        unordered_map<string, string> preg_2_areg;
        unordered_map<string, bool> commit_status;

        unordered_map< int, vector<string> > f_snapshots; // important to store the static-allocated objects
        unordered_map< int, unordered_map<string, string> > a_snapshots;
        unordered_map< int, unordered_map<string, string> > p_snapshots;

        void _output_mapping(vector<string> &info, vector<string> &aregs, bool is_branch, int pc);
    public:
        decoder();
        void print_regs();
        void free_regs();
        void update_commit(string reg, bool committed);
        void flush_mappings(int pc); // store info as snapshots, used by branch prediction flushing
        bool rename(instruction &ins);
        bool can_rename();
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
        void free_dep(res_record *rr);
    public:
        deque<res_record*> entries;
        ROB(int nr);
        bool is_full();
        bool commit(CDB &bus, decoder &d);
        void find_dep(res_record &rr); // search for dependecies (true data hazards)
        void find_mem_dep(res_record &rr);
        void add_entry(res_record &rr); 
        void add_ref(res_record *rr);
        void branch_flush(decoder &d, ins_queue &q, fetcher &f, BTB &btb);
};

class BTB {
    private: 
        const int max_size = 16;
        unordered_map<int, int> targets;
        unordered_map<int, bool> predicts; // 1 bit predictor, bool type
        unordered_map<int, int> lru_stamps; // map pc to cycle count, for eviction

    public:
        void write_entry(int pc, int target);
        int predict(int pc);
};