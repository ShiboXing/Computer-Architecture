#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Fetch {
    private:
        ifstream *ins_stream;
    public:
        int _fn;
        Fetch(int fn, string fname) {
            _fn = fn;
            ins_stream = new ifstream(fname);
        }

        void fetch_next();
};