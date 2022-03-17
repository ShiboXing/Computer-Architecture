#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

#define INS_ID unordered_map<string, int>({\
    {"add", 1},\
    {"addi", 2},\
    {"fld", 3},\
    {"fsd", 4},\
    {"fadd", 5},\
    {"fsub", 6},\
    {"fmul", 7},\
    {"fdiv", 8},\
    {"bne", 9},\
})

#define INS_LAT unordered_map<string, int>({\
    {"add", 1},\
    {"addi", 1},\
    {"fld", 1},\
    {"fsd", 1},\
    {"fadd", 3},\
    {"fsub", 3},\
    {"fmul", 4},\
    {"fdiv", 8},\
    {"bne", 1},\
})

extern int PC;
extern unordered_map<string, int> TAG_TB; // map tag string to PC addr
extern unordered_map<int, int> MEM;
