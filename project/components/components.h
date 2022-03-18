#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../globals.h"
#include "../structures/structures.h"

using namespace std;

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
        ins_table &ins_tb;
    public:
        decoder(ins_table &ins_tb_ref);
        void rename_last();
};