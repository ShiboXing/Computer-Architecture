#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../globals.h"

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
        vector<int> *free_list;
    public:
        decoder();
};