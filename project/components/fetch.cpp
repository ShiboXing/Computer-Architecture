#include "components.h"

using namespace std;

// add the code file
void fetcher::add_codepth(string fname) {
    ins_stream = new ifstream(fname);
}

// fetch instruction from the current PC
string fetcher::fetch_next() {
    string line;

    if (!ins_stream->eof()) 
        getline(*ins_stream, line);
    
    return line;
}

// scan all tags in the code file, sufficing the compiler's role herein
void fetcher::scan_tags() {
    ins_stream->seekg(0); // reset to the beginning
    string line;
    int split_pos;
    int tmp_pc = 0;
    while (getline(*ins_stream, line)) {
        if ((split_pos = line.find(':')) != string::npos) { // line has a tag
            string tag = line.substr(0, split_pos);       
            TAGS[tag] = tmp_pc;
        } 
        tmp_pc++;
    }
}

// set the PC to target (used by branching or flushing)
void fetcher::set_pc(int pc) {
    ins_stream->seekg(0); // reset to the beginning
    string line;
    for (int i=0; i<pc; i++)
        getline(*ins_stream, line); 

    PC = pc;
}