#include <uWS/uWS.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <bitset>

#include "def.h"

using std::cout;
using std::endl;
using std::string;
using std::bitset;

std::vector<std::string> storedMessages;
std::vector<int> excludedMessages;
std::stringstream indexHtml;
std::string pidString;
int connections = 0;

const bitset<96> START("100100100100100100100100100100100100001001001001001001001001010010010010010010010010010010010010");

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

// char* prepareBoard(const bitset<96> board) {
//     return (char*)(START.to_string().c_str());
// }

int main() {
    uWS::Hub h;

    // storedMessages.push_back(START.to_string());

    indexHtml << std::ifstream ("index.html").rdbuf();
    if (!indexHtml.str().length()) {
        std::cerr << "Failed to load index.html" << std::endl;
        return -1;
    }

    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
        cout << "onHttpRequest" << endl;
        if (req.getUrl().valueLength == 1) {
            res->end(indexHtml.str().data(), indexHtml.str().length());
        } else {
            // i guess this should be done more gracefully?
            res->end(nullptr, 0);
        }
    });

    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        cout << "onConnection" << endl;
        uWS::WebSocket<uWS::SERVER>::PreparedMessage *preparedMessage = 
            uWS::WebSocket<uWS::SERVER>::prepareMessage((char*)(START.to_string().c_str()), 96, uWS::TEXT, false);
        ws->sendPrepared(preparedMessage);
        ws->finalizeMessage(preparedMessage);
    });

    h.onMessage([&h](uWS::WebSocket<uWS::SERVER> *ws, char *data, size_t length, uWS::OpCode opCode) {
        cout << "onMessage" << endl;
        if (length && length < 4096) {
            cout << string(data,length) << endl;
        }
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        cout << "onDisconnection" << endl;
    });

    h.getDefaultGroup<uWS::SERVER>().startAutoPing(30000);
    if (h.listen(3000)) {
        std::cout << "Listening to port 3000" << std::endl;
    } else {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }
    h.run();
}