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
// #include "server.h"

using std::cout;
using std::endl;
using std::string;
using std::bitset;
using std::istringstream;
using std::stringstream;
using std::ostringstream;
using std::copy;
using std::vector;
using std::ostream_iterator;
using std::istream_iterator;
using std::basic_string;
using std::strcpy;
using std::ifstream;

struct Message {
	char* data;
	size_t size;
	vector<char> vec;

	void prepareBoard(const vector<char> & board);
};

typedef basic_string<wchar_t> wstring;

const vector<char> START_BOARD { 
	'r','r','r','r',
	'r','r','r','r',
	'r','r','r','r',
	'_','_','_','_',
	'_','_','_','_',
	'b','b','b','b',
	'b','b','b','b',
	'b','b','b','b' };

int getKb();
void createServerInstance(uWS::Hub &h);
void loadFile(stringstream & fileData, string fname = "index.html");

#endif
