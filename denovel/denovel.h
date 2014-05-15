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
#include <iostream>

typedef std::vector<std::string> words_list;

void decompress (std::istream& instream, std::ostream& outstream);

void compress (std::istream& instream, std::ostream& outstream);

#endif /* defined(__denovel__denovel__) */
