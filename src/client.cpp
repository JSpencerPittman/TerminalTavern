#include <boost/asio.hpp>
#include "TTClient.h"

int main() {
    boost::asio::io_context ioContext;
    TTClient client(ioContext);
    client.run();
}