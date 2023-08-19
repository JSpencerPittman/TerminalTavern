#ifndef TERMINALTAVERN_ASYNCCLIENT_H
#define TERMINALTAVERN_ASYNCCLIENT_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include "Player.h"
#include "Room.h"

using boost::asio::ip::tcp;

class TTClient {
public:
    TTClient(boost::asio::io_service& ioService);
    void run();
private:
    void sendAction(const Action& action);
    int retrievePlayerID();
    PlayerMap retrievePlayerMap();
    Direction getInput(WINDOW* win);

    std::string getData();

    void refreshClient(Room& room, WINDOW* win);

    tcp::socket socket_;
    int playerID_;
    boost::asio::steady_timer timer_;
    int refCnt_;
};


#endif //TERMINALTAVERN_ASYNCCLIENT_H
