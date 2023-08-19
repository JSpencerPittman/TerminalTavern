#ifndef TERMINALTAVERN_ASYNCCLIENT_H
#define TERMINALTAVERN_ASYNCCLIENT_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include "Player.h"
#include "Room.h"

#define REDRAW_FREQUENCY 100
#define REFRESH_RATE 50

#define SERVER_HOSTNAME "127.0.0.1"
#define SERVER_PORT 9999

using boost::asio::ip::tcp;

class TTClient {
public:
    TTClient(boost::asio::io_service& ioService, WINDOW* win);
    void run();
private:
    void sendAction(const Action& action);
    int retrievePlayerID();
    PlayerMap retrievePlayerMap();
    static Direction getInput();

    std::string getData();

    void refreshClient();

    tcp::socket socket_;
    int playerID_;
    boost::asio::steady_timer timer_;
    int refCnt_;
    WINDOW* win_;
    Room room_;
};


#endif //TERMINALTAVERN_ASYNCCLIENT_H
