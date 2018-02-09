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
#include "threadUtils.h"

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
	void prepareReply(string messageType, const string & boardString, string playerColor = "");
	void prepareReply(string messageType, const string & boardString, string playerColor, string otherMoves);
	void prepareBasicMessage(string messageType, string message);
};

typedef uWS::WebSocket<uWS::SERVER> WebSocket;
typedef basic_string<wchar_t> wstring;

void createServerInstance(uWS::Hub &h, Communicator &comm);
void loadFile(stringstream & fileData, string fname = "index.html");
void sendStartBoard();
void sendMessage(WebSocket *ws, Message & message);
void sendMove(WebSocket * ws, string boardString);

#endif
