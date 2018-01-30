#include "server.h"

stringstream fileData;

int getKb() {
	std::string line;
	std::ifstream self("/proc/self/status");
	int vmData, vmStk, vmPte;
	while(!self.eof()) {
		std::getline(self, line, ':');
		if (line == "VmPTE") {
			self >> vmPte;
		} else if (line == "VmData") {
			self >> vmData;
		} else if (line == "VmStk") {
			self >> vmStk;
		}
		std::getline(self, line);
	}
	return vmData - vmStk - vmPte;
}

void Message::prepareBoard(const vector<char> & board){
	ostringstream oss;
	copy(board.begin(),board.end(),ostream_iterator<char>(oss));
	string boardString = oss.str();

	string msg = "board " + boardString;
	vec = vector<char>(msg.length() + 1);
	strcpy(&vec[0], msg.c_str());

	data = &vec[0];
	size = msg.length();
}

void createServerInstance(uWS::Hub &h) {

	h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
		cout << "onHttpRequest" << endl;

		// cout << req.getUrl().toString() << endl;
		string requestedFile = req.getUrl().toString();
		cout << requestedFile << endl;

		if (requestedFile.at(0) == '/'){
			if (requestedFile.length() == 1) {
				loadFile("./index.html");
			} else {
				loadFile("."+requestedFile);
			}
		}

		if (fileData.str().length())
			res->end(fileData.str().data(), fileData.str().length());
		else
			res->end(nullptr, 0);


		// loadFile(requestedFile);
		

		// cout << req.getHeader("get").value << endl;
		// cout << req.getHeader("").key << endl;

		// if (req.getUrl().valueLength == 1) {

			// loadFile();
			// if (!fileData.str().length()) {
			// 	cout << "Failed to load index.html" << endl;
			// }

			
		// } else {
			
		// }
	});

	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
		cout << "onConnection" << endl;
		Message reply;
		reply.prepareBoard(START_BOARD);
		cout << reply.data << endl;
		uWS::WebSocket<uWS::SERVER>::PreparedMessage *preparedMessage = 
			uWS::WebSocket<uWS::SERVER>::prepareMessage(reply.data, reply.size, uWS::TEXT, false);
		ws->sendPrepared(preparedMessage);
		ws->finalizeMessage(preparedMessage);
	});

	h.onMessage([&h](uWS::WebSocket<uWS::SERVER> *ws, char *data, size_t length, uWS::OpCode opCode) {
		cout << "onMessage" << endl;
		if (length && length < 4096) {
			istringstream iss(string(data,length));
			vector<string> tokens{
				istream_iterator<string>{iss},
				istream_iterator<string>{}};

			cout << iss.str() << endl;
			
			if(tokens[0] == "checkMove"){
				cout << "Check Move" << endl;
			}
		}
	});

	h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
		cout << "onDisconnection" << endl;
	});

	h.getDefaultGroup<uWS::SERVER>().startAutoPing(30000);
	if (h.listen(3000)) {
		cout << "Listening to port 3000" << endl;
	} else {
		cout << "Failed to listen to port" << endl;
	}
}

void loadFile(string fname) {
	fileData.str("");
	fileData << std::ifstream (fname).rdbuf();
	if (fileData.str().length() > 1)
		cout << fname << " loaded successfully. " << fileData.str().length() << " bytes loaded." << endl;
	else 
		cout << "Could not load " << fname << endl;
}