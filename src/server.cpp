#include "server.h"

void Message::prepareReply(string messageType, const vector<char> & board, string playerColor){
	ostringstream oss;
	copy(board.begin(),board.end(),ostream_iterator<char>(oss));
	string boardString = oss.str();

	string msg = messageType + " " + boardString + " " + playerColor;
	vec = vector<char>(msg.length() + 1);
	strcpy(&vec[0], msg.c_str());

	data = &vec[0];
	size = msg.length();
}

void sendMessage(uWS::WebSocket<uWS::SERVER> *ws, Message & message) {
	uWS::WebSocket<uWS::SERVER>::PreparedMessage *preparedMessage = 
			uWS::WebSocket<uWS::SERVER>::prepareMessage(message.data, message.size, uWS::TEXT, false);
	ws->sendPrepared(preparedMessage);
	ws->finalizeMessage(preparedMessage);
}

void createServerInstance(uWS::Hub &h) {

	h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {

		cout << "onHttpRequest" << endl;

		string requestedFile = req.getUrl().toString();
		cout << requestedFile << endl;

		stringstream fileData;
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

	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
		cout << "onConnection" << endl;
		Message reply;
		reply.prepareReply("board",START_BOARD);
		cout << reply.data << endl;
		sendMessage(ws,reply);
	});

	h.onMessage([&h](uWS::WebSocket<uWS::SERVER> *ws, char *data, size_t length, uWS::OpCode opCode) {
		cout << "onMessage" << endl;
		if (length && length < 4096) {
			istringstream iss(string(data,length));
			vector<string> tokens{
				istream_iterator<string>{iss},
				istream_iterator<string>{}};

			cout << iss.str() << endl;

			Message reply;

			if (tokens[0] == "checkMove") {
				vector<char> board(tokens[1].begin(), tokens[1].end());
				string playerColor = tokens[2];
				// Put function call to check move here
				cout << "Check Move for " << playerColor << endl;

				reply.prepareReply("confirmMove",board,"red");
				sendMessage(ws,reply);
			}

			else if (tokens[0] == "computerMove") {
				vector<char> board(tokens[1].begin(), tokens[1].end());
				string computerColor = tokens[2];

				// trigger computer move then send another msg
				
				iter_swap(board.begin()+19, board.begin()+23);
				reply.prepareReply("move",board,"black");
				sendMessage(ws,reply);	
			}

			else if (tokens[0] == "resetGame") {
				
				reply.prepareReply("board",START_BOARD);
				sendMessage(ws,reply);
			}

			cout << "Sent reply" << endl;
			cout << reply.data << endl;
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

void loadFile(stringstream & fileData, string fname) {
	fileData = stringstream("");
	fileData << std::ifstream (fname).rdbuf();
	if (fileData.str().length() > 1)
		cout << fname << " loaded successfully. " << fileData.str().length() << " bytes loaded." << endl;
	else 
		cout << "Could not load " << fname << endl;
}