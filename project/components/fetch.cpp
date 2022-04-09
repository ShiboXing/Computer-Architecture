#include "components.h"

using namespace std;

string fetcher::fetch_next() {
    string line;
    
    if (ins_stream->is_open()) {
        if (!getline(*ins_stream, line))
            ins_stream->close();
        else
            PC++;
    } else {
        cout << "fetcher file stream closed!" << endl;
    }

    return line;
}

fetcher::fetcher(string fname) {
    ins_stream = new ifstream(fname);
}
