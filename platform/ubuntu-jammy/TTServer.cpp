#include "TTServer.h"

TTServer::TTServer(boost::asio::io_context &ioContext, int port)
    : ioContext_(ioContext),
      acceptor_(ioContext, tcp::endpoint(tcp::v4(), port)),
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
