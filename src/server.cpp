#include "server.h"
#include "consts.h"

// red=true, black=false
bool computerColor = false;

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

void Message::prepareReply(string messageType, const string & boardString, string playerColor){
	string msg = messageType + " " + boardString + " " + playerColor;
	vec = vector<char>(msg.length() + 1);
	strcpy(&vec[0], msg.c_str());

	data = &vec[0];
	size = msg.length();
}

void Message::prepareReply(string messageType, const string & boardString, string playerColor, string otherMoves){
	string msg = messageType + " " + boardString + " " + playerColor + " " + otherMoves;
	vec = vector<char>(msg.length() + 1);
	strcpy(&vec[0], msg.c_str());

	data = &vec[0];
	size = msg.length();
}

void Message::prepareBasicMessage(string messageType, string msg) {
	vec = vector<char>(msg.length() + 1);
	strcpy(&vec[0], msg.c_str());

	data = &vec[0];
	size = msg.length();
}

void sendMove(WebSocket * ws, string boardString) {
	Message msg;
	//CheckerBoard boardClass(boardString, computerColor, redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard);
	CheckerBoard boardClass(boardString, computerColor); // redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard);
	cout << "recieved move" << endl;
	string move = boardClass.getRandoMove();

	vector<string> allMoves = boardClass.getAllRandoMoves();
	string otherMoves = "";
	for( auto move : allMoves) {
		otherMoves += move + ",";
	}

	msg.prepareReply(
		"move",
		move,
		computerColor ? "red" : "black",
		otherMoves );
	sendMessage(ws,msg);
}

void sendStartBoard(WebSocket * ws) {
	Message msg;

	if(computerColor) { // computer color is red
		sendMove(ws, START_BOARD_STRING);
	} else { // computer color is black
		msg.prepareReply("board",START_BOARD_STRING);
		sendMessage(ws,msg);
	}
}

void sendMessage(WebSocket *ws, Message & message) {
	WebSocket::PreparedMessage *preparedMessage = 
			WebSocket::prepareMessage(message.data, message.size, uWS::TEXT, false);
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

	h.onConnection([&h](WebSocket *ws, uWS::HttpRequest req) {
		cout << "onConnection" << endl;
		sendStartBoard(ws);
	});

	h.onMessage([&h](WebSocket *ws, char *data, size_t length, uWS::OpCode opCode) {
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
				// convert board from string to vector<char>
				vector<char> board(tokens[1].begin(), tokens[1].end());

				sendMove(ws, tokens[1]);

				// // get computer's color
				// string computerColorString = tokens[2];

				// // cout << "Get move" << endl;

				// CheckerBoard boardClass(tokens[1], computerColor, redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard);

				// // cout << "recieved move" << endl;

				// string move = boardClass.getRandoMove();
				// vector<string> allMoves = boardClass.getAllRandoMoves();
				// string otherMoves = "";
				// for( auto move : allMoves) {
				// 	otherMoves += move + ",";
				// }

				// move += " " + otherMoves;

				// // cout << "sending move" << endl;

				// // stringify and send new board to client
				// reply.prepareReply("move", move, computerColor ? "red" : "black");
				// sendMessage(ws,reply);	
			}

			else if (tokens[0] == "resetGame") {
				cout << "Reset Game" << endl;
				sendStartBoard(ws);
			}

			else if (tokens[0] == "changePlayerColorTo") {
				if (tokens[1] == "black") {
					computerColor = true;
					reply.prepareBasicMessage("changeComputerColorTo","red");
				}
				else if (tokens[1] == "red") {
					computerColor = false;
					reply.prepareBasicMessage("changeComputerColorTo","black");
				}

				
			}

			cout << "Sent reply" << endl;
			cout << reply.data << endl;
		}
	});

	h.onDisconnection([&h](WebSocket *ws, int code, char *message, size_t length) {
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