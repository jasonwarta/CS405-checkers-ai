#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <uWS/uWS.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <bitset>
#include <algorithm>
#include <iterator>

#include "def.h"

using std::cout;
using std::endl;
using std::string;
using std::bitset;
using std::istringstream;
using std::stringstream;
using std::copy;
using std::vector;
using std::istream_iterator;

struct Message {
	char* data;
	size_t size;
};

const bitset<96> START("100100100100100100100100100100100100001001001001001001001001010010010010010010010010010010010010");

int getKb();
void createServerInstance(uWS::Hub &h);
Message prepareBoard(const bitset<96> board);
void loadHtml();

#endif
