#include "TCPConnection.h"

TCPConnection::pointer TCPConnection::create(boost::asio::io_context &ioContext,
                                             PlayerMap* playerMap, MessageHistory* history) {
    return pointer(new TCPConnection(ioContext, playerMap, history));
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

TCPConnection::TCPConnection(boost::asio::io_context &ioContext,
                             PlayerMap* playerMap, MessageHistory* history)
        : socket_(ioContext), playerMap_(playerMap), history_(history), playerID_(-1) {}

void TCPConnection::handlePacket(const boost::system::error_code& e) {
    std::istream input_stream(&buf_);
    std::string data;
    std::getline(input_stream, data);

    if(e == boost::asio::error::eof) {
        std::cout << "Terminating player #" << playerID_
                  << "..." << std::endl;
        playerMap_->removePlayer(playerID_);
        return;
    }

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
        case  Packet::Operation::REFRESHROOM:
            handleRefreshRoom((RefreshRoomPacket*)packet);
            break;
        case Packet::Operation::MESSAGE:
            handleSendMessage((SendMessagePacket*)packet);
            break;
        case Packet::Operation::REFRESHCHAT:
            handleRefreshChat((RefreshChatPacket*)packet);
            break;
        default:
            break;
    }

    buf_.consume(buf_.size());
    start();
}

void TCPConnection::handleMovePlayer(MovePacket *packet) {
    std::cout << "Moving player #" << playerID_
              << "..." << std::endl;
    playerMap_->movePlayer(playerID_,
                           packet->direction());
    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleAddPlayer(AddPacket* packet) {
    std::cout << "Adding player #" << packet->player().getPlayerID()
        << "..." << std::endl;

    playerID_ = packet->player().getPlayerID();
    username_ = packet->player().getUsername();
    playerMap_->addPlayer(packet->player());

    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleDeletePlayer(DeletePacket* packet) {}

void TCPConnection::handleRequestID(RequestIDPacket* /*packet*/) {
    std::cout << "Sending available player ID..." << std::endl;
    playerID_ = playerMap_->availableID();
    std::string serialPlayerID = std::to_string(playerID_);
    sendData(serialPlayerID);
}

void TCPConnection::handleRefreshRoom(RefreshRoomPacket* /*packet*/) {
    std::string serialPlayerMap = playerMap_->serialize();
    sendData(serialPlayerMap);
}

void TCPConnection::handleSendMessage(SendMessagePacket* packet) {
    Message newMsg{username_, packet->message()};
    history_->addMessage(newMsg);
    std::string serialHistory = history_->serialize();
    sendData(serialHistory);
}

void TCPConnection::handleRefreshChat(RefreshChatPacket* /*packet*/) {
    std::string serialHistory = history_->serialize();
    sendData(serialHistory);
}

void TCPConnection::sendData(std::string &data) {
    boost::asio::async_write(socket_,
                             boost::asio::buffer(data + '\n'),
                             boost::asio::use_future);
}