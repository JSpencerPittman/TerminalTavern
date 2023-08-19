#include <boost/asio.hpp>
#include "TTClient.h"

int main() {
    boost::asio::io_service ioService;
    TTClient client(ioService);
    client.run();
    ioService.run();
}