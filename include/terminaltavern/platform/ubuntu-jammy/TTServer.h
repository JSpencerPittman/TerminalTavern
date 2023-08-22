#ifndef TERMINALTAVERN_ASYNCSERV_H
#define TERMINALTAVERN_ASYNCSERV_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "TCPConnection.h"
#include "Player.h"
#include "Chat.h"
#include "util.h"

using boost::asio::ip::tcp;

class TTServer
{
public:
    TTServer(boost::asio::io_context& ioContext, int port);

private:
    // Start listening for incoming users on a new socket
    void start_accept();

    // Once a user connects go ahead and start that TCP Connection
    //  and then set up a new socket to listen for incoming connections
    void handle_accept(TCPConnection::pointer newConnection,
                       const boost::system::error_code& error);

    boost::asio::io_context& ioContext_;
    tcp::acceptor acceptor_;
    PlayerMap playerMap_;
    MessageHistory history_;
};

#endif
