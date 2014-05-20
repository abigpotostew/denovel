// denovel, compress + decompress novels based on:
//   http://www.reddit.com/r/dailyprogrammer/comments/25o2bd/5162014_challenge_162_hard_novel_compression_pt_3/
// by stewart bracken
// copy at will.

#include <iostream>
#include "denovel.h" // compress / decompress

using namespace std;

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
        }
        
        if ( algorithm_ptr != nullptr ){
            return algorithm_ptr ( argv[2], argv[3] );
        }
    }
    DIE();
}

