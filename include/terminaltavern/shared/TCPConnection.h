#ifndef TERMINALTAVERN_TCPCONNECTION_H
#define TERMINALTAVERN_TCPCONNECTION_H

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "Player.h"
#include "Packet.h"
#include "util.h"

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
    void handlePacket(const boost::system::error_code& e);

    void handleMovePlayer(MovePacket* packet);
    void handleAddPlayer(AddPacket* packet);
    void handleDeletePlayer(DeletePacket* packet);
    void handleRequestID(RequestIDPacket* packet);
    void handleRefresh(RefreshPacket* packet);

    void sendData(std::string& data);

    tcp::socket socket_;
    std::string message_;
    boost::asio::streambuf buf_;
    PlayerMap *playerMap_;
};


#endif //TERMINALTAVERN_TCPCONNECTION_H
