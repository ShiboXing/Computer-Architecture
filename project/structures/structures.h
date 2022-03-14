#include <unordered_map>
#include <string>
#include "../globals.h"

using namespace std;

/**
    singleton register hash map
*/
class rename_map {
    private:
        unordered_map<int, int> mapping;
        static rename_map *rm_;
    public:
        int& operator[](int key) {
            return mapping[key];
        }

        static rename_map *get_map();
};

class instruction {
    private:
        int _info[4];
        int _split_one(string ins, string *tmp_val, int start);
    public:
        instruction(string &ins);
};  

// bool status: Issue, Read op, Exec Completed, Write Result
class ins_table {
    private:
        unordered_map<int, bool*> st_tb; 
        unordered_map<int, bool*> ins_tb;
    public:
        ins_table() { }

        void add_status(string ins);

        void add_instruciton(string ins);
};