#include <unordered_map>
#include <chrono>
#include <string>

using namespace std;

typedef chrono::time_point<chrono::system_clock> Tp;

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
        int info[4];
        int _get_one(string ins, int start);
    public:
        instruction(string ins);
};  

// bool status: Issue, Read op, Exec Completed, Write Result
class ins_table {
    private:
        unordered_map<Tp*, bool*> st_tb; 
        unordered_map<Tp*, bool*> ins_tb;
    public:
        ins_table() { }

        void add_status(Tp &ts);

        void add_instruciton(string ins);
};



