#ifndef TERMINALTAVERN_ASYNCCLIENT_H
#define TERMINALTAVERN_ASYNCCLIENT_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Room.h"
#include "util.h"
#include "Packet.h"

#define REDRAW_FREQUENCY 100
#define REFRESH_RATE 50

using boost::asio::ip::tcp;

class TTClient {
public:
    TTClient(boost::asio::io_service& ioService, ServerInfo serverInfo, Player player);
    void run();
private:
    void sendPacket(const Packet& packet);
    int retrievePlayerID();
    PlayerMap retrievePlayerMap();
    static Direction getInput();

    std::string getData();

    void refreshClient();

    tcp::endpoint resolveEndpoint();
    static bool isIPAddress(const std::string& s);
    static bool isNumber(const std::string& s);

    // Tavern
    int playerID_;
    Player player_;
    Room room_;

    // Main execution loop
    boost::asio::steady_timer timer_;
    int refCnt_;

    // Networking
    ServerInfo serverInfo_;
    tcp::resolver resolver_;
    tcp::socket socket_;
};


#endif //TERMINALTAVERN_ASYNCCLIENT_H
