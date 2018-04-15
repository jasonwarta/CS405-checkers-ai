#include <uWS/uWS.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <bitset>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <mutex>
#include <iomanip>
#include <memory>

#include "Player.h"
#include "Game.h"

#include "checkers.h"
#include "consts.h"
#include "MinimaxWithAlphaBeta.h"

#include "Player.h"
#include "Game.h"

#include "defs.h"

typedef uWS::WebSocket<uWS::SERVER> WebSocket;

bool gameMode = true;
bool computerColor = false;
bool currentTurn = true;

struct Message {
	char* data;
	size_t size;
	std::vector<char> vec;

	void prepareReply(std::string messageType, const std::vector<char> & board, std::string playerColor = "")
	{
		std::ostringstream oss;
		copy(board.begin(),board.end(),std::ostream_iterator<char>(oss));
		std::string boardString = oss.str();

		std::string msg = messageType + " " + boardString + " " + playerColor;
		vec = std::vector<char>(msg.length() + 1);
		strcpy(&vec[0], msg.c_str());

		data = &vec[0];
		size = msg.length();
	}

	void prepareReply(std::string messageType, const std::string & boardString, std::string playerColor = "")
	{
		std::string msg = messageType + " " + boardString + " " + playerColor;
		vec = std::vector<char>(msg.length() + 1);
		strcpy(&vec[0], msg.c_str());

		data = &vec[0];
		size = msg.length();
	}

	void prepareReply(std::string messageType, const std::string & boardString, std::string playerColor, std::string otherMoves)
	{
		std::string msg = messageType + " " + boardString + " " + playerColor + " " + otherMoves;
		vec = std::vector<char>(msg.length() + 1);
		strcpy(&vec[0], msg.c_str());

		data = &vec[0];
		size = msg.length();
	}

	void prepareBasicMessage(std::string messageType, std::string msg)
	{
		vec = std::vector<char>(msg.length() + 1);
		strcpy(&vec[0], msg.c_str());

		data = &vec[0];
		size = msg.length();
	}
};

void sendMessage(WebSocket *ws, Message & message);
void sendMove(WebSocket * ws, NeuralNet * net, std::string & boardString);
void loadFile(std::stringstream & fileData, std::string fname = "index.html");
void sendStartBoard(WebSocket * ws, NeuralNet * net);
void createServerInstance(uWS::Hub &h, NeuralNet * net);



void sendMessage(WebSocket *ws, Message & message) {
	WebSocket::PreparedMessage *preparedMessage =
			WebSocket::prepareMessage(message.data, message.size, uWS::TEXT, false);
	ws->sendPrepared(preparedMessage);
	ws->finalizeMessage(preparedMessage);
}

void sendMove(WebSocket * ws, NeuralNet * net, std::string & boardString) {
	Message msg;

	std::string strCopy = boardString;
	MinimaxWithAlphaBeta tree(strCopy,8,computerColor,net);
	std::string move = tree.getBestBoard();

	CheckerBoard checkers(boardString, computerColor);
	std::vector<std::string> vec = checkers.getAllRandoMoves();
	std::string otherMoves = "";

	for(std::string & s : vec) {
		otherMoves += s + ",";
	}

	msg.prepareReply(
		"move",
		move,
		computerColor ? "red" : "black",
		otherMoves );
	sendMessage(ws,msg);
}

void loadFile(std::stringstream & fileData, std::string fname) {
	fileData = std::stringstream("");
	fileData << std::ifstream (fname).rdbuf();
	if (fileData.str().length() > 1)
		std::cout << fname << " loaded successfully. " << fileData.str().length() << " bytes loaded." << std::endl;
	else
		std::cout << "Could not load " << fname << std::endl;
}

void sendStartBoard(WebSocket * ws, NeuralNet * net) {
	Message msg;
	std::string board = "";
	board = "rrrrrrrrrrrr________bbbbbbbbbbbb";
	if(computerColor) { // computer color is red
		sendMove(ws, net, board);
	} else { // computer color is black
		msg.prepareReply("board",board);
		sendMessage(ws,msg);
	}
}

void createServerInstance(uWS::Hub &h, NeuralNet * net) {

	h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {

		// std::cout << "onHttpRequest" << std::endl;

		std::string requestedFile = req.getUrl().toString();
		// std::cout << requestedFile << std::endl;

		std::stringstream fileData;
		if (requestedFile.at(0) == '/'){
			if (requestedFile.length() == 1) {
				loadFile(fileData,"./index.html");
			} else {
				loadFile(fileData,"."+requestedFile);
			}
		}

		if (fileData.str().length())
			res->end(fileData.str().data(), fileData.str().length());
		else
			res->end(nullptr, 0);
	});

	h.onConnection([&h,&net](WebSocket *ws, uWS::HttpRequest req) {
		// std::cout << "onConnection" << std::endl;
		sendStartBoard(ws,net);
	});

	h.onMessage([&h,&net](WebSocket *ws, char *data, size_t length, uWS::OpCode opCode) {
		// std::cout << "onMessage" << std::endl;
		if (length && length < 4096) {
			std::istringstream iss(std::string(data,length));
			std::vector<std::string> tokens{
				std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>{}};

			// std::cout << iss.str() << std::endl;

			Message reply;

			if (tokens[0] == "checkMove") {
				// check validity of player move and send board back to client, if valid

				std::vector<char> board(tokens[1].begin(), tokens[1].end());
				std::string playerColor = tokens[2];



				// Put function call to check move here
				// std::cout << "Check Move for " << playerColor << std::endl;

				reply.prepareReply("confirmMove",board,playerColor);
				// comm.setBoard(tokens[1]);
				// currentBoard = tokens[1];
				sendMessage(ws,reply);
			}

			else if (tokens[0] == "changeGameMode") {
				if (tokens[1] == "PvC") {
					gameMode = true;
				}

				else if (tokens[1] == "CvC") {
					gameMode = false;
				}
			}

			else if (tokens[0] == "watching") {
				// std::cout << "watching" << std::endl;
			}

			else if (tokens[0] == "computerMove") {
				// send a computer move
				sendMove(ws, net, tokens[1]);
			}

			else if (tokens[0] == "resetGame") {
				// reset game state

				// std::cout << "Reset Game" << std::endl;
				sendStartBoard(ws, net);
			}

			else if (tokens[0] == "changePlayerColorTo") {
				// recieved signal to reset player color, change computer color to the opposite color

				if (tokens[1] == "black") {
					computerColor = true;
					reply.prepareBasicMessage("changeComputerColorTo","red");
				}
				else if (tokens[1] == "red") {
					computerColor = false;
					reply.prepareBasicMessage("changeComputerColorTo","black");
				}


			}

			// std::cout << "Sent reply" << std::endl;
			// std::cout << reply.data << std::endl;
		}
	});

	h.onDisconnection([&h](WebSocket *ws, int code, char *message, size_t length) {
		// std::cout << "onDisconnection" << std::endl;
	});

	h.getDefaultGroup<uWS::SERVER>().startAutoPing(30000);
	if (h.listen(3000)) {
		std::cout << "Listening to port 3000" << std::endl;
	} else {
		std::cout << "Failed to listen to port" << std::endl;
	}
}

int main(int argc, char const *argv[]) {
	if(argc != 2) {
		std::cout << "you must pass a filename for the net to load" << std::endl;
		return 0;
	}

	std::ifstream ifs;
	ifs.open(argv[1]);
	NeuralNet * ai = new NeuralNet(ifs);
	ifs.close();

	uWS::Hub h;
	createServerInstance(h,ai);
	h.run();

	return 0;
}