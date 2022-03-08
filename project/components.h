#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

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

class rename_map {
    private:
        unordered_map<int, int> mapping;
    public:
        rename_map() { }    

        int& operator[](int key) {
            return mapping[key];
        }
};
// class Decoder {
//     private:
        
// }