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
using std::iter_swap;

#include "checkers.h"

/* Message struct facilitates setting up messages to send to the client
 * 
 * For sending a new board at start of game or on reset event, use 
 * prepareReply( "board", START_BOARD )
 * 
 * To send the confirmation that a player move is valid, use
 * prepareReply( "confirmMove", board, playerColor )
 *  *NOTE: playerColor is "red" or "black". This function is only used if playing with an opponent using the website interface
 * 
 * To send a computer's move, use
 * prepareReply( "move", board, playerColor )
 *  *NOTE: playerColor is the color that is assigned to the computer
 * 
 */


struct Message {
	char* data;
	size_t size;
	vector<char> vec;

	void prepareReply(string messageType, const vector<char> & board, string playerColor = "");
};

typedef uWS::WebSocket<uWS::SERVER> WebSocket;
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
