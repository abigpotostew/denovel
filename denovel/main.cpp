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

bool is_number(char token){
    return token>='0' && token <= '9';
}

void split(std::string& line, std::vector<std::string>& token_buff){
    std::istringstream iss(line);
    copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter<std::vector<std::string> >(token_buff));
}

void decompress(dictionary& d, std::ifstream& file, std::string& output){
    std::string line;
    while (std::getline (file, line)){
        std::vector<std::string> tokens;
        split(line,tokens);
        bool has_new_line = true;
        for (auto itor=tokens.begin(); itor!=tokens.end(); ++itor) {
            std::string token = *itor;
            int current_char=0;
            std::string word;
            char separator = 0;
            while (current_char<token.length()){
                // Capture up to a non numerical character
                while (is_number(token[current_char])) ++current_char;
                // convert a number index to word
                if (current_char>0){
                    int word_id = atoi(token.substr(0,current_char).c_str());
                    word = d[word_id]; //copy the word
                    separator = ' ';
                }
                //convert special character to output
                if (current_char < token.length()){
                    char operator_token = token[current_char];
                    ++current_char;
                    switch (operator_token){
                        case '^': // print word Uppercase
                            word[0] -= 32;
                            break;
                        case '!': // print UPPER case or !
                            if (current_char==1) {
                                output.push_back('!');
                            }
                            else{
                                for (int i=0; i<word.size();++i)
                                    word[i] -= 32;
                            }
                            break;
                        case '-': // hyphenate previous and current word
                            has_new_line = true;
                        case '.': case ',': case '?': case ';': case ':':
                            separator = operator_token;
                            break;
                        case 'R': case 'r':
                            separator = '\n';
                            has_new_line = true;
                            break;
                        case 'E': case 'e':
                            return;
                        default:
                            continue;
                    }
                }
                if ( separator && ((has_new_line && separator != ' ') || !has_new_line) ) {
                    output.push_back(separator);
                    separator = 0;
                }
                if (word.length()>0) {
                    output.append(word);
                    word.clear();
                    has_new_line = false;
                }
            }
        }
    }
}

int main(int argc, const char * argv[])
{
    std::string compressed_file_path = "novel_compressed";
    std::ifstream file( compressed_file_path );
    if (!file){
        return EXIT_FAILURE;
    }
    dictionary my_dict;
    parse_dict(file, my_dict);
    std::string* decompressed = new std::string();
    decompress(my_dict, file, *decompressed);
    file.close();
    std::cout << *decompressed << std::endl;
    return 0;
}

