#include "TTServer.h"

#include <iostream>

TCPConnection::pointer TCPConnection::create(boost::asio::io_context &ioContext, PlayerMap* playerMap) {
    return pointer(new TCPConnection(ioContext, playerMap));
}

tcp::socket &TCPConnection::socket() {
    return socket_;
}

void TCPConnection::start() {
    boost::asio::async_read_until(socket_, buf_, '\n',
                                  boost::bind(&TCPConnection::handle_action,
                                              shared_from_this(),
                                              boost::asio::placeholders::error));
}

TCPConnection::TCPConnection(boost::asio::io_context &ioContext, PlayerMap* playerMap)
    : socket_(ioContext), playerMap_(playerMap) {}

void TCPConnection::handle_action(const boost::system::error_code& e) {
    std::istream input_stream(&buf_);
    std::string data;
    std::getline(input_stream, data);

    if(e == boost::asio::error::eof) return;

    Action action = Action::deserialize(data);

    switch (action.getOperation()) {
        case Action::REQID:
            handleActionRequestID(action);
            break;
        case Action::ADD:
            handleActionAddPlayer(action);
            break;
        case Action::MOVE:
            handleActionMovePlayer(action);
            break;
        case Action::DEL:
            handleActionDeletePlayer(action);
            break;
        case Action::REFRESH:
            handleActionRefresh(action);
            break;
        default:
            break;
    }

    buf_.consume(buf_.size());
    start();
}

void TCPConnection::handleActionRequestID(const Action &action) {
    std::cout << "Sending available player ID..." << std::endl;
    std::string serialPlayerID = std::to_string(playerMap_->availableID());
    sendData(serialPlayerID);
}

void TCPConnection::handleActionAddPlayer(const Action &action) {
    std::cout << "Adding player #" << action.getPlayerID() << "..." << std::endl;
    Player newPlayer{action.getPlayerID(), action.getX(), action.getY()};
    playerMap_->addPlayer(newPlayer);

    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleActionMovePlayer(const Action &action) {
    std::cout << "Moving player #" << action.getPlayerID() << "..." << std::endl;
    int pID = action.getPlayerID();
    Direction dir = action.getDirection();
    playerMap_->movePlayer(pID, dir);

    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleActionDeletePlayer(const Action &action) {
    std::cout << "Terminating player #" << action.getPlayerID() << "..." << std::endl;
    int pID = action.getPlayerID();
    playerMap_->removePlayer(pID);
}

void TCPConnection::handleActionRefresh(const Action &action) {
    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::sendData(std::string &data) {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(data + '\n'),
                             boost::asio::use_future);
}

TTServer::TTServer(boost::asio::io_context &ioContext)
    : ioContext_(ioContext),
      acceptor_(ioContext, tcp::endpoint(tcp::v4(), 9999)),
      playerMap_() {
    start_accept();
}

void TTServer::start_accept() {
    std::cout << "Awaiting connection..." << std::endl;
    TCPConnection::pointer newConnection = TCPConnection::create(ioContext_, &playerMap_);
    acceptor_.async_accept(newConnection->socket(),
                           boost::bind(&TTServer::handle_accept, this,
                                       newConnection, boost::asio::placeholders::error));
}

void TTServer::handle_accept(TCPConnection::pointer newConnection,
                              const boost::system::error_code &error) {
    std::cout << "Connection established..." << std::endl;
    if(!error) {
        newConnection->start();
    }

    start_accept();
}
