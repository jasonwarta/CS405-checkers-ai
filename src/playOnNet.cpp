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

std::string NetworkGame::getLastMove()
{
    std::stringstream ss;
    ss << "curl -s " << address_ << "/?info_game=true -d '{";
    ss << "\"name\":\"" << gameName_ << "\"";
    ss << "}'";

    std::cout << ss.str() << std::endl;
    std::string result = getStdoutFromCommand(ss.str());
    
    if (result == "")
        return "error: no result";

    json_t data = JSON_parse(result);
    if (data["error"])
        return to_string(data["error"]);

    if (data["boards"]) {
        std::string lastBoard = "";
        for (auto &b : data["boards"]) {
            lastBoard = to_string(b);
        }
        return lastBoard;
    }

    return "error parsing JSON";
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
    std::string theBoard = getLastMove();
    
    while(true && moveCounter < 200) {
        
        std::cout << theBoard << " " << theBoard.size() << std::endl;
        if (theBoard.find("error") != std::string::npos)
        {   
            std::cout << "Error getting move" << std::endl;
            std::cout << "Error: " << theBoard << std::endl;
            break;
        }

        std::string move = CheckerBoard(theBoard, redTeam_).getRandoMove();
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

        while(true) {
            std::string lastMove = getLastMove();
            if (std::string(lastMove).compare(move) != 0)
                break;

            std::cout << "waiting" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        theBoard = getLastMove();
        
        if (std::find(possibleBoards.begin(), possibleBoards.end(), theBoard) == possibleBoards.end())
            std::cout << "Invalid Move Detected" << std::endl;
    }
}