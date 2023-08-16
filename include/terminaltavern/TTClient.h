#ifndef TERMINALTAVERN_ASYNCCLIENT_H
#define TERMINALTAVERN_ASYNCCLIENT_H

#include <boost/asio.hpp>
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

    tcp::socket socket_;
    int playerID_;
};


#endif //TERMINALTAVERN_ASYNCCLIENT_H
