#include "components.h"

using namespace std;

void Fetch::fetch_next() {
    
    if (ins_stream->is_open()) {
        string line;
        if (!getline(*ins_stream, line))
            ins_stream->close();
        else
            cout << line << endl;    
    }
}