//  denovel, compress
#include <iostream>

using namespace std;

#include "denovel.h"

int main (int argc, const char * argv[])
{
    if (argc == 2 && strcmp(argv[1], "-d")==0)
        decompress( cin, cout );
    else if (argc == 1)
        compress( cin, cout );
    else{
        cerr << "Usage: denovel [-d]" << endl;
        return 1;
    }
    return 0;
}

