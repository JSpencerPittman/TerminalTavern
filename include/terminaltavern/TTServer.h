#ifndef TERMINALTAVERN_ASYNCSERV_H
#define TERMINALTAVERN_ASYNCSERV_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "Action.h"
#include "Player.h"

using boost::asio::ip::tcp;

class TCPConnection: public boost::enable_shared_from_this<TCPConnection>
{
public:
    typedef boost::shared_ptr<TCPConnection> pointer;

    static pointer create(boost::asio::io_context& ioContext, PlayerMap* playerMap);

    tcp::socket& socket();

    // Read in serialized action asynchronously
    void start();

private:
    TCPConnection(boost::asio::io_context& ioContext, PlayerMap* playerMap);

    // Process the user's request
    void handle_action(const boost::system::error_code& e);

    void handleActionRequestID(const Action& action);
    void handleActionAddPlayer(const Action& action);
    void handleActionMovePlayer(const Action& action);
    void handleActionDeletePlayer(const Action& action);
    void handleActionRefresh(const Action& action);

    void sendData(std::string& data);

    tcp::socket socket_;
    std::string message_;
    boost::asio::streambuf buf_;
    PlayerMap *playerMap_;
};

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
};

#endif
