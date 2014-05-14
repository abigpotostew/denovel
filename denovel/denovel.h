//
//  denovel.h
//  denovel
//
//  Created by Stewart Bracken on 5/14/14.
//  Copyright (c) 2014 Stewart Bracken. All rights reserved.
//

#ifndef __denovel__denovel__
#define __denovel__denovel__

#include <vector>
#include <fstream> //read only file

typedef std::vector<std::string> words_list;

void parse_dict(std::ifstream& file, words_list& dict);

void decompress (words_list& d, std::ifstream& file, std::string& output);

void compress (std::istream& instream, std::ostream& outstream);

#endif /* defined(__denovel__denovel__) */
