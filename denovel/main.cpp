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
#include <cstdlib> //atoi
#include <algorithm>
#include <iterator>

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

void parse_dict(std::ifstream& file, dictionary& dict){
    int dict_length;
    std::string line;
    getline(file, line);
    std::istringstream(line) >> dict_length;
    for (int i=0; i<dict_length; ++i) {
        getline(file, line);
        dict.push_back(line);
    }
}

bool isNumber(char* token){
    return *token>0 && *token < 108;
}

void decompress(dictionary& d, std::ifstream& file, std::string& output){
    std::string line, token;
    std::string word;
    char separator = '\0';
    bool has_new_line = true;
    while (std::getline (file, line)){
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        copy(std::istream_iterator<std::string>(iss),
             std::istream_iterator<std::string>(),
             std::back_inserter<std::vector<std::string> >(tokens));
        for (auto itor=tokens.begin(); itor!=tokens.end(); ++itor) {
        token = *itor;
        
        int i=0;
        while (i<token.length()){
            // Capture up to a non numerical character
            while (token[i] >= '0' && token[i] <= '9') ++i;
            // convert a number index to word
            if (i>0){
                int word_id = atoi(token.substr(0,i).c_str());
                word = d[word_id]; //copy the word
            }
            bool needs_space = true;
            //convert special character to output
            if (i < token.length()){
                char operating = token[i];
                ++i;
                switch (operating){
                    case '^': // print word Uppercase
                        if (word[0] >= 'a') word[0] -= 32; break;
                    case '!': // print UPPER case or !
                        if (i==0) {
                            output.push_back('!');
                        }
                        else{
                            for (int i=0; i<word.size();++i)
                                if (word[i]<'a') word[i] += 32;
                        }
                        break;
                    case '-': // hyphenate previous and next word
                        separator = '-';
                        break;
                    case '.':
                    case ',':
                    case '?':
                    case ';':
                    case ':':
                        output.push_back(operating);
                        needs_space = false;
                        break;
                    case 'R':
                    case 'r':
                        output.push_back('\n');
                        has_new_line = true;
                        needs_space = false;
                        break;
                    case 'E':
                    case 'e':
                        return;
                    default:
                        continue;
                }
            }
            if (separator){
                output.push_back(separator);
                separator = 0;
            } else if (!has_new_line && needs_space){
                output.push_back(' ');
            }
            output.append(word);
            word.clear();
        }
        }
    }
}

int main(int argc, const char * argv[])
{
    std::string compressed_file_path = "novel_compressed";
    std::ifstream file( compressed_file_path );
    dictionary my_dict;
    parse_dict(file, my_dict);
    std::string* decompressed = new std::string();
    decompress(my_dict, file, *decompressed);
    file.close();
    std::cout << *decompressed << std::endl;
    return 0;
}

