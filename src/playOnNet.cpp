#include "./playOnNet.h"

std::string getStdoutFromCommand(std::string cmd) {
    std::string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}

Status NetworkGame::getLastMove()
{
    std::stringstream ss;
    ss << "curl -s " << address_ << "/?info_game=true -d '{";
    ss << "\"name\":\"" << gameName_ << "\"";
    ss << "}'";

    std::cout << ss.str() << std::endl;
    std::string result = getStdoutFromCommand(ss.str());
    
    if (result == "")
        return std::make_pair("error","no result");

    json_t data = JSON_parse(result);
    if (data["error"])
        return std::make_pair("error",to_string(data["error"]));

    if (data["boards"]) {
        std::string lastBoard = "";
        std::string status = to_string(data["status"]);
        for (auto &b : data["boards"]) {
            lastBoard = to_string(b);
        }
        return std::make_pair(status, lastBoard);
    }

    return std::make_pair("error","couldn't parse JSON");
}

std::string NetworkGame::sendMove(std::string move) {
    std::stringstream ss;
    ss << "curl -s " << address_ << "/?play_game=true -d '{";
    ss << "\"name\":\"" << gameName_ << "\"";
    ss << ",";
    ss << "\"board\":\"" << move << "\"";
    ss << "}'";

    std::cout << ss.str() << std::endl;
    std::string result = getStdoutFromCommand(ss.str());
    std::cout << result << std::endl;

    if (result == "")
        return "error: no result";

    json_t data = JSON_parse(result);

    if (data["error"])
        return "error: " + to_string(data["error"]);

    return "success";
}

void NetworkGame::playGame()
{
    std::cout << "starting game" << std::endl;

    uint moveCounter = 0;
    Status boardStatus = getLastMove();
    std::string myStatusString = redTeam_ ? "red_turn" : "black_turn";
    
    while(boardStatus.first != myStatusString) {
        std::cout << boardStatus.first << " " << boardStatus.second << std::endl;
        if (boardStatus.first == "red_won" || boardStatus.first == "black_won")
            return;

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        boardStatus = getLastMove();
    }

    std::string theBoard = boardStatus.second;

    while(true) {
        
        std::cout << theBoard << " " << theBoard.size() << std::endl;
        if (theBoard.find("error") != std::string::npos)
        {   
            std::cout << "Error getting move" << std::endl;
            std::cout << "Error: " << theBoard << std::endl;
            break;
        }

        std::shared_ptr<Clock> clock = std::make_shared<Clock>(std::chrono::system_clock::now());
        std::string move = MinimaxWithAlphaBeta(theBoard,10,redTeam_,clock,net_,false).getBestBoard(&std::cout);
        // std::string move = CheckerBoard(theBoard, redTeam_).getRandoMove();
        std::cout << "sending move: " << move << std::endl;

        std::string result = sendMove(move);
        if (result.find("error") != std::string::npos)
        {
            std::cout << "Error sending move" << std::endl;
            std::cout << result << std::endl;
            break;
        }
        std::cout << "sent move" << std::endl;

        std::vector<std::string> possibleBoards = CheckerBoard(move, !redTeam_).getAllRandoMoves();

        boardStatus = getLastMove();
        while(boardStatus.first != myStatusString) {
            std::cout << boardStatus.first << " " << boardStatus.second << std::endl;
            if(boardStatus.first == "red_won" || boardStatus.first == "black_won")
                break;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            boardStatus = getLastMove();
        }
        
        theBoard = getLastMove().second;
        
        if (std::find(possibleBoards.begin(), possibleBoards.end(), theBoard) == possibleBoards.end())
            std::cout << "Invalid Move Detected" << std::endl;
    }
}