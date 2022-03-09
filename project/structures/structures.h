#include <unordered_map>
#include <chrono>

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
        rename_map() { }

        int& operator[](int key) {
            return mapping[key];
        }

        static rename_map *get_map();
};

// bool status: Issue, Read op, Exec Completed, Write Result
class ins_table {
    private:
        unordered_map<Tp*, bool*> ins; 
    public:
        ins_table() {}

        void add_ins(Tp &ts);
};



