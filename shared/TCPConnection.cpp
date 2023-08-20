#include "TCPConnection.h"

TCPConnection::pointer TCPConnection::create(boost::asio::io_context &ioContext, PlayerMap* playerMap) {
    return pointer(new TCPConnection(ioContext, playerMap));
}

tcp::socket &TCPConnection::socket() {
    return socket_;
}

void TCPConnection::start() {
    boost::asio::async_read_until(socket_, buf_, '\n',
                                  boost::bind(&TCPConnection::handlePacket,
                                              shared_from_this(),
                                              boost::asio::placeholders::error));
}

TCPConnection::TCPConnection(boost::asio::io_context &ioContext, PlayerMap* playerMap)
        : socket_(ioContext), playerMap_(playerMap) {}

void TCPConnection::handlePacket(const boost::system::error_code& e) {
    std::istream input_stream(&buf_);
    std::string data;
    std::getline(input_stream, data);

    if(e == boost::asio::error::eof) return;

    Packet* packet = Packet::deserialize(data);

    switch (packet->operation()) {
        case Packet::Operation::MOVE:
            handleMovePlayer((MovePacket*)packet);
            break;
        case Packet::Operation::ADD:
            handleAddPlayer((AddPacket*)packet);
            break;
        case Packet::Operation::DEL:
            handleDeletePlayer((DeletePacket*)packet);
            break;
        case Packet::Operation::REQID:
            handleRequestID((RequestIDPacket*)packet);
            break;
        case  Packet::Operation::REFRESH:
            handleRefresh((RefreshPacket*)packet);
            break;
        default:
            break;
    }

    buf_.consume(buf_.size());
    start();
}

void TCPConnection::handleMovePlayer(MovePacket *packet) {
    std::cout << "Moving player #" << packet->playerID()
              << "..." << std::endl;
    playerMap_->movePlayer(packet->playerID(),
                           packet->direction());

    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleAddPlayer(AddPacket* packet) {
    std::cout << "Adding player #" << packet->player().getPlayerID()
        << "..." << std::endl;

    playerMap_->addPlayer(packet->player());

    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleDeletePlayer(DeletePacket* packet) {
    std::cout << "Terminating player #" << packet->playerID()
        << "..." << std::endl;
    playerMap_->removePlayer(packet->playerID());
}

void TCPConnection::handleRequestID(RequestIDPacket* /*packet*/) {
    std::cout << "Sending available player ID..." << std::endl;
    std::string serialPlayerID = std::to_string(playerMap_->availableID());
    sendData(serialPlayerID);
}

void TCPConnection::handleRefresh(RefreshPacket* /*packet*/) {
    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::sendData(std::string &data) {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(data + '\n'),
                             boost::asio::use_future);
}