//  denovel
#include <iostream>
#include <fstream> //read only file


using namespace std;

#include "denovel.h"

/*
int main (int argc, const char * argv[])
{
    if (argc < 2 || argc > 2){
        cerr << "Usage: denovel [path to compressed novel]" << endl;
        return EXIT_FAILURE;
    }
    ifstream file( argv[1] );
    if (!file){
        cerr << "Error opening file.";
        return EXIT_FAILURE;
    }
    words_list my_dict;
    parse_dict(file, my_dict);
    string* decompressed = new string();
    decompress(my_dict, file, *decompressed);
    file.close();
    cout << *decompressed << endl;
    delete decompressed;
    return 0;
}

*/