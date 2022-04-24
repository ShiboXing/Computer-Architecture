#include "globals.h"

using namespace std;

void fetcher::add_codepth(string fname) {
    ins_stream = new ifstream(fname);
}

string fetcher::fetch_next() {
    string line;
    
    if (!ins_stream->eof()) {
        if (!getline(*ins_stream, line)) {
            cout << "fetch stream is closed at PC " << PC << endl;
        }
    }

    return line;
}

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

void fetcher::set_pc(int pc) {
    ins_stream->seekg(0); // reset to the beginning
    string line;
    for (int i=0; i<pc && !ins_stream->eof(); i++)
        getline(*ins_stream, line); 

    PC = pc;
}