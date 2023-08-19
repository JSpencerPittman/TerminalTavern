#include <boost/asio.hpp>
#include "TTClient.h"

int main() {
    boost::asio::io_service ioService;
    WINDOW* win = initscr();
    TTClient client(ioService, win);
    client.run();
    ioService.run();
}