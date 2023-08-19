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

#define REDRAW_FREQUENCY 100
#define REFRESH_RATE 50

#if BOOST_VERSION >= BOOST_1_82
typedef boost::asio::ip::port_type PortType;
#elif BOOST_VERSION >= BOOST_1_74
typedef int PortType;
#else
    typedef int PortType;
#endif

using boost::asio::ip::tcp;

class TTClient {
public:
    TTClient(boost::asio::io_service& ioService, std::string hostname,  PortType port);
    void run();
private:
    void sendAction(const Action& action);
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
    Room room_;

    // Main execution loop
    boost::asio::steady_timer timer_;
    int refCnt_;

    // Networking
    std::string hostname_;
    PortType port_;
    tcp::resolver resolver_;
    tcp::socket socket_;
};


#endif //TERMINALTAVERN_ASYNCCLIENT_H
