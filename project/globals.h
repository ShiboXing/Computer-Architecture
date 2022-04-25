#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#define REG_SIZE 32
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
// resvation stations count: 
#define RES_CNT unordered_map<string, int>({\
    {"INT", 4},\
    {"LOAD", 2},\
    {"STORE", 2},\
    {"FPADD", 3},\
    {"FPMULT", 4},\
    {"FPDIV", 2},\
    {"BU", 1},\
})
#define TYPE_MAP unordered_map<string, string>({\
    {"add", "INT"},\
    {"addi", "INT"},\
    {"fadd", "FPADD"},\
    {"fsub", "FPADD"},\
    {"fmul", "FPMULT"},\
    {"fdiv", "FPDIV"},\
    {"bne", "BU"},\
    {"fld", "LOAD"},\
    {"fsd", "STORE"},\
})
#define GET_REG_NUM(reg_str) stoi(reg_str.substr(1, reg_str.length()-1))

extern int NF;
extern int NW;
extern int NR;
extern int NB;
extern int PC;
extern int CYCLE;
extern float REGS[32];
extern unordered_map<string, int> TAGS; // map tag string to PC addr
extern unordered_map<int, int> MEM;
