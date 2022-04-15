#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

#define NF 4
#define NW 4
#define NR 16
#define NB 4
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

// resvationa stations count: 
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

extern int PC;
extern int CYCLE;
extern float REGS[32];
extern unordered_map<string, int> TAG_TB; // map tag string to PC addr
extern unordered_map<int, int> MEM;


