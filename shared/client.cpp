#include <boost/asio.hpp>
#include <cstring>
#include <string>
#include "TTClient.h"

#define DEFAULT_HOSTNAME "terminaltavern.ddns.net"
#define DEFAULT_PORT 10777

int main(int argc, char* argv[]) {
    bool customDestination = argc == 3;
    std::string hostname = customDestination ? argv[1] : DEFAULT_HOSTNAME;
    int port = customDestination ? std::stoi(argv[2]) : DEFAULT_PORT;

    boost::asio::io_service ioService;
    TTClient client(ioService, hostname, port);
    client.run();
    ioService.run();
}