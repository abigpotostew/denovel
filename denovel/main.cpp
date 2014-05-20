//  denovel, compress
#include <iostream>

using namespace std;

#include "denovel.h"

#define DIE_M(msg) cerr << msg << endl; return EXIT_FAILURE
#define DIE() DIE_M("Usage: denovel -c|-d infile outfile")

int main (int argc, const char * argv[])
{
    int (*algorithm_ptr)(const char*, const char*) = nullptr;
    if (argc == 4){
        if (strcmp(argv[1], "-d")==0) {
            algorithm_ptr = decompress;
        }else if (strcmp(argv[1], "-c") == 0){
            algorithm_ptr = compress;
        }else{
            DIE();
        }
        return algorithm_ptr ( argv[2], argv[3] );
    }
    DIE();
}

