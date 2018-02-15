#include "server.h"
#include "consts.h"
#include "MinMaxTree.h"

//Bugs
//____b____________bbBr__b__b_b__b

// PvC=true, CvC=false
bool gameMode = true;

// red=true, black=false
bool computerColor = false;
bool currentTurn = true;

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

void sendMove(WebSocket * ws, string boardString, Communicator &comm) {
	Message msg;
	//CheckerBoard boardClass(boardString, computerColor, redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard);
	CheckerBoard boardClass(boardString, computerColor); // redMoveBoard, redJumpBoard, blackMoveBoard, blackJumpBoard);
	// cout << "recieved move" << endl;
	string move = minMaxTreeBase(boardString, 4, false);

	vector<string> allMoves = boardClass.getAllRandoMoves();
	string otherMoves = "";
	for( auto move : allMoves) {
		otherMoves += move + ",";
	}

	// currentBoard = move;
	comm.setBoard(move);
	msg.prepareReply(
		"move",
		move,
		computerColor ? "red" : "black",
		otherMoves );
	sendMessage(ws,msg);
}

void sendStartBoard(WebSocket * ws, Communicator & comm) {
	Message msg;

	if(computerColor) { // computer color is red
		sendMove(ws, START_BOARD_STRING, comm);
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

void createServerInstance(uWS::Hub &h, Communicator & comm) {

	h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {

		// cout << "onHttpRequest" << endl;

		string requestedFile = req.getUrl().toString();
		// cout << requestedFile << endl;

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

	h.onConnection([&h,&comm](WebSocket *ws, uWS::HttpRequest req) {
		// cout << "onConnection" << endl;
		sendStartBoard(ws,comm);
	});

	h.onMessage([&h,&comm](WebSocket *ws, char *data, size_t length, uWS::OpCode opCode) {
		// cout << "onMessage" << endl;
		if (length && length < 4096) {
			istringstream iss(string(data,length));
			vector<string> tokens{
				istream_iterator<string>{iss},
				istream_iterator<string>{}};

			// cout << iss.str() << endl;

			Message reply;

			if (tokens[0] == "checkMove") {
				// check validity of player move and send board back to client, if valid

				vector<char> board(tokens[1].begin(), tokens[1].end());
				string playerColor = tokens[2];



				// Put function call to check move here
				// cout << "Check Move for " << playerColor << endl;

				reply.prepareReply("confirmMove",board,playerColor);
				comm.setBoard(tokens[1]);
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
				// cout << "watching" << endl;
			}

			else if (tokens[0] == "computerMove") {
				// send a computer move

				sendMove(ws, tokens[1], comm);
			}

			else if (tokens[0] == "resetGame") {
				// reset game state

				// cout << "Reset Game" << endl;
				sendStartBoard(ws,comm);
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

			// cout << "Sent reply" << endl;
			// cout << reply.data << endl;
		}
	});

	h.onDisconnection([&h](WebSocket *ws, int code, char *message, size_t length) {
		// cout << "onDisconnection" << endl;
	});

	h.getDefaultGroup<uWS::SERVER>().startAutoPing(30000);
	if (h.listen(3000)) {
		// cout << "Listening to port 3000" << endl;
	} else {
		// cout << "Failed to listen to port" << endl;
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
