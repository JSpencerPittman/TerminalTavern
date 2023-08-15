#include <boost/asio.hpp>
#include "TTServer.h"

int main() {
    boost::asio::io_context ioContext;
    TTServer server(ioContext);
    ioContext.run();
}