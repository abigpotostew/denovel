//
//  denovel.cpp
//  denovel
//
//  Created by Stewart Bracken on 5/14/14.
//  Copyright (c) 2014 Stewart Bracken. All rights reserved.
//

#include "denovel.h"

#include <iostream> //cin
#include <sstream> //convert string -> int
#include <fstream> //read only file
#include <cstdlib> //atoi
#include <algorithm> //split
#include <iterator> //split
#include <unordered_map> //word list

using namespace std;

void parse_dict(ifstream& file, words_list& dict){
    int dict_length;
    string line;
    getline(file, line);
    istringstream(line) >> dict_length;
    for (int i=0; i<dict_length; ++i) {
        getline(file, line);
        dict.push_back(line);
    }
}

bool is_number (char token){
    return token>='0' && token <= '9';
}

void split (string& line, vector<string>& token_buff){
    istringstream iss(line);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(token_buff));
}

void decompress (words_list& d, ifstream& file, string& output){
    string* line = new string();
    while (getline (file, *line)){ // read line by line, split by '\n'
        vector<string>* tokens = new vector<string>();
        split(*line, *tokens); // split line by whitespace into token groups
        bool has_new_line = true;
        for (auto itor = tokens->begin(); itor != tokens->end(); ++itor) {
            string token = *itor;
            int current_char=0;
            string word;
            char separator = 0;
            // step over each characters in token group
            while (current_char < token.length()){
                // Capture up to a non numerical character
                while (is_number (token[current_char])) ++current_char;
                // convert a number index to word
                if (current_char > 0){
                    int word_id = atoi (token.substr (0, current_char).c_str());
                    word = d[word_id]; //copy the word
                    separator = ' ';
                }
                //convert operator character to output
                if (current_char < token.length()){
                    char operator_token = token[current_char];
                    ++current_char;
                    switch (operator_token){
                        case '^': // print word Uppercase
                            word[0] -= 32;
                            break;
                        case '!': // print UPPER case or !
                            if (current_char==1) {
                                output.push_back ('!');
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
                // Append a separator if we aren't on a new line.
                if ( separator &&
                    ((has_new_line && separator != ' ') || !has_new_line) ) {
                    output.push_back (separator);
                    separator = 0;
                }
                //Append a word
                if (word.length()>0) {
                    output.append (word);
                    word.clear();
                    has_new_line = false;
                }
            }
        }
        line->clear();
        delete tokens;
    }
    delete line;
}

bool is_upper ( char c ){
    return c < 'a';
}

bool is_upper ( string& word ){
    return is_upper (word[0]);
}

// returns true if only the first letter of the string is capitalized
bool is_capitalized ( string& word ){
    if ( word.length() > 0 && is_upper (word) ){
        if (word.length() == 1 || is_upper (word[1]) )
            return true;
    }
    return false;
}

void compress (std::istream& instream, std::ostream& outstream){
    typedef unordered_map<string, size_t> dict_map;
    typedef pair<string,size_t> word_id_pair;
    typedef pair<dict_map::iterator, bool> insert_result;
    
    string line;//, word;
    string* compressed_buffer = new string();
    words_list* line_words = new words_list();
    dict_map* unique_dict = new dict_map();
    words_list* dictionary = new words_list();
    
    while (getline(instream, line)) {
        split(line, *line_words);
        //for ( words_list::iterator itor = line_words->begin();
        //      itor != line_words->end(); ++itor){
        for (string word : *line_words){
            //word = *itor;
            size_t word_idx = dictionary->size();
            char operating_char = 0;
            //convert word to lower-case and catch operators
            if ( is_capitalized (word) ){
                operating_char = '^';
                word = to_lower(word);
            }
            
            
            insert_result res = unique_dict->insert (
                                    word_id_pair (word, word_idx) );
            bool insert_success = res.second;
            if (insert_success){
                dictionary->push_back(word);
            }else{
                word_idx = res.first->second;
            }
            if (compressed_buffer->length() > 0){
                compressed_buffer->push_back(' ');
            }
            if (operating_char){
                compressed_buffer->push_back(operating_char);
            }
            //push the word index to the compressed data buffer
            compressed_buffer->append (to_string (word_idx));
        }
        line_words->clear();
    }
    outstream << dictionary->size();
    for (int i=0;i<dictionary->size(); ++i){
        outstream << '\n' << (*dictionary)[i];
    }
    outstream << '\n' << *compressed_buffer << endl;
    
    delete compressed_buffer;
    delete line_words;
    delete unique_dict;
    delete dictionary;
}






