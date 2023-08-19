#include <boost/asio.hpp>
#include "TTServer.h"

#define DEFAULT_PORT 9999

int main(int argc, char* argv[]) {
    int port = (argc == 2) ? std::stoi(argv[1]) : DEFAULT_PORT;

    boost::asio::io_context ioContext;
    TTServer server(ioContext, port);
    ioContext.run();
}