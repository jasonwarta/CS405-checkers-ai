// #include <uWS/uWS.h>
// #include <vector>
// #include <fstream>
// #include <iostream>
// #include <sstream>
// #include <cstdint>
// #include <bitset>
// #include <algorithm>
// #include <iterator>

// #include "def.h"

// using std::cout;
// using std::endl;
// using std::string;
// using std::bitset;
// using std::istringstream;
// using std::stringstream;
// using std::copy;
// using std::vector;
// using std::istream_iterator;

// std::stringstream indexHtml;

// struct Message {
// 	char* data;
// 	size_t size;
// };

// const bitset<96> START("100100100100100100100100100100100100001001001001001001001001010010010010010010010010010010010010");

// int getKb() {
// 	std::string line;
// 	std::ifstream self("/proc/self/status");
// 	int vmData, vmStk, vmPte;
// 	while(!self.eof()) {
// 		std::getline(self, line, ':');
// 		if (line == "VmPTE") {
// 			self >> vmPte;
// 		} else if (line == "VmData") {
// 			self >> vmData;
// 		} else if (line == "VmStk") {
// 			self >> vmStk;
// 		}
// 		std::getline(self, line);
// 	}
// 	return vmData - vmStk - vmPte;
// }

// Message prepareBoard(const bitset<96> board) {
// 	string msg = "board " + board.to_string();
// 	return { (char*)(msg.c_str()), msg.size() };
// }

// void loadHtml() {
// 	indexHtml.str("");
// 	indexHtml << std::ifstream ("index.html").rdbuf();
// }

// int main() {
// 	uWS::Hub h;

// 	h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
// 		cout << "onHttpRequest" << endl;
// 		if (req.getUrl().valueLength == 1) {

// 			loadHtml();
// 			if (!indexHtml.str().length()) {
// 				std::cerr << "Failed to load index.html" << std::endl;
// 				return -1;
// 			}

// 			res->end(indexHtml.str().data(), indexHtml.str().length());
// 		} else {
// 			res->end(nullptr, 0);
// 		}
// 	});

// 	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
// 		cout << "onConnection" << endl;
// 		Message reply = prepareBoard(START);
// 		uWS::WebSocket<uWS::SERVER>::PreparedMessage *preparedMessage = 
// 			uWS::WebSocket<uWS::SERVER>::prepareMessage(reply.data, reply.size, uWS::TEXT, false);
// 		ws->sendPrepared(preparedMessage);
// 		ws->finalizeMessage(preparedMessage);
// 	});

// 	h.onMessage([&h](uWS::WebSocket<uWS::SERVER> *ws, char *data, size_t length, uWS::OpCode opCode) {
// 		cout << "onMessage" << endl;
// 		if (length && length < 4096) {
// 			istringstream iss(string(data,length));
// 			vector<string> tokens{
// 				istream_iterator<string>{iss},
// 				istream_iterator<string>{}};

// 			cout << iss.str() << endl;
			
// 			if(tokens[0] == "checkMove"){
// 				cout << "Check Move" << endl;
// 			}
// 		}
// 	});

// 	h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
// 		cout << "onDisconnection" << endl;
// 	});

// 	h.getDefaultGroup<uWS::SERVER>().startAutoPing(30000);
// 	if (h.listen(3000)) {
// 		std::cout << "Listening to port 3000" << std::endl;
// 	} else {
// 		std::cerr << "Failed to listen to port" << std::endl;
// 		return -1;
// 	}
// 	h.run();
// }