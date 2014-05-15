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
//#include <fstream> //read only file
#include <cstdlib> //atoi
#include <algorithm> //split
#include <iterator> //split
#include <unordered_map> //word list

using namespace std;

void parse_dict(istream& instream, words_list& dict){
    int dict_length;
    string line;
    getline(instream, line);
    istringstream(line) >> dict_length;
    for (int i=0; i<dict_length; ++i) {
        getline(instream, line);
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

void decompress (istream& instream, ostream& outstream){
    words_list* dict = new words_list();
    parse_dict(instream, *dict);
    string* line = new string();
    while (getline (instream, *line)){ // read line by line, split by '\n'
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
                    word = (*dict)[word_id]; //copy the word
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
                                outstream << '!';
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
                    outstream << separator;
                    separator = 0;
                }
                //Append a word
                if (word.length()>0) {
                    outstream << word;
                    word.clear();
                    has_new_line = false;
                }
            }
        }
        line->clear();
        delete tokens;
    }
    delete line;
    delete dict;
    flush(outstream);
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

bool is_letter ( char c ){
    return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' );
}

void compress (std::istream& instream, std::ostream& outstream){
    typedef unordered_map<string, size_t> dict_map;
    typedef pair<string,size_t> word_id_pair;
    typedef pair<dict_map::iterator, bool> insert_result;
    
    string line, word;
    string* compressed_buffer = new string();
    words_list* line_words = new words_list();
    dict_map* unique_dict = new dict_map();
    words_list* dictionary = new words_list();
    
    while (getline(instream, line)) {
        split(line, *line_words);
        for ( words_list::iterator itor = line_words->begin();
              itor != line_words->end(); ++itor){
            word = *itor;
            size_t word_idx = dictionary->size();
            
            char operating_char = 0;
            char punctuation = 0;
            
            bool is_capitalized = true, is_uppercase = true, first = true;
            int idx=0;
            for (auto str_itor = word.begin(); str_itor != word.end(); ++str_itor) {
                char c = *str_itor;
                if ( !is_letter(c) ){ //some punctuation
                    punctuation = c;
                    if(idx+1 != word.length()){
                        string second_word = word.substr(idx+1);
                        if (itor+1==line_words->end())
                            itor = line_words->insert(line_words->end(), second_word)-1;
                        else
                            itor = line_words->insert(itor+1, second_word) - 1;
                    }
                    word = word.substr(0,idx);
                    break;
                } else if ( !is_upper(c) ){
                    if (first)
                        is_capitalized = false;
                    is_uppercase = false;
                } else{
                    *str_itor += 32;
                }
                first = false;
                ++idx;
            }
            
            if (is_capitalized){
                if (word.length() == 1 || !is_uppercase)
                    operating_char = '^';
                else
                    operating_char = '!';
            }
            
            insert_result res = unique_dict->insert (
                                    word_id_pair (word, word_idx) );
            bool insert_success = res.second;
            if (insert_success){
                dictionary->push_back(word);
            }else{
                word_idx = res.first->second;
            }
            //push the word index to the compressed data buffer
            compressed_buffer->append (to_string (word_idx));
            if (operating_char){ //optionally push capitalization operator
                compressed_buffer->push_back (operating_char);
            }
            compressed_buffer-> push_back(' ');
            if (punctuation){ //optionally push punctuation character
                compressed_buffer->push_back (punctuation);
                compressed_buffer->push_back (' ');
            }
        }
        compressed_buffer->append("R ");
        line_words->clear();
    }
    //print dictionary count and words
    outstream << dictionary->size();
    for (int i=0;i<dictionary->size(); ++i){
        outstream << '\n' << (*dictionary)[i];
    }
    //print compressed data buffer
    outstream << '\n' << *compressed_buffer << "E ";
    
    delete compressed_buffer;
    delete line_words;
    delete unique_dict;
    delete dictionary;
    
    flush(outstream);
}