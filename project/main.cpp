#include "components.h"
#include <iostream>

int main() {
    Fetch f(4, "ins.dat");
    cout << f._fn << endl;   

    f.fetch_next();
    f.fetch_next(); 
    return 0;  
}