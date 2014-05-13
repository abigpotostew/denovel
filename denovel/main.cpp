//
//  main.cpp
//  denovel
//
//  Created by Stewart Bracken on 5/12/14.
//  Copyright (c) 2014 Stewart Bracken. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream> //read only file
#include <sstream> //convert string -> int

typedef std::vector<std::string> dictionary;

char* read_file (const char* path){
    std::ifstream file (path);
    if (file){
        file.seekg (0, file.end);
        long long length = file.tellg();
        file.seekg (0, file.beg);
        
        char* buffer = new char[length];
        file.read (buffer,length);
        
        if (file){
            return buffer;
        }
    }
    std::cerr << "Error reading file." << std::endl;
    exit(EXIT_FAILURE);
    return 0;
}

char* parse_dict_compressed(std::ifstream& file, dictionary& dict){
    int dict_length;
    std::string line;
    getline(file, line);
    std::istringstream(line) >> dict_length;
    for (int i=0; i<dict_length; ++i) {
        getline(file, line);
        dict.push_back(line);
    }
    long long curr = file.tellg();
    file.seekg(0, file.end);
    long long length = file.tellg() - curr;
    file.seekg(std::streampos(curr));
    char* buffer = new char[length];
    file.read(buffer, length);
    return buffer;
}

void decompress(dictionary& d, char* compressed){
    
}

int main(int argc, const char * argv[])
{
    char* compressed_file_path = "novel_compressed";
    std::ifstream file( compressed_file_path );
    dictionary my_dict;
    char* compressed = parse_dict_compressed(file, my_dict);
    file.close();
    std::cout << my_dict.size() << "\n" << compressed << std::endl;
    
    
    delete compressed;
    return 0;
}

