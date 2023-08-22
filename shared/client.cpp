#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "Start.h"
#include "TTClient.h"

#define DEFAULT_HOSTNAME "terminaltavern.ddns.net"
#define DEFAULT_PORT 10777

int main(int argc, char* argv[]) {
    Start start;
    clear();
    refresh();

    ServerInfo serverInfo = start.getServerInfo();

    Coord2D initPos{5, 5};
    Player player(-1, initPos, start.getPlayerPixelMap(), start.getUsername());

    boost::asio::io_service ioService;
    WINDOW* win = initscr();
    TTClient client(ioService, win, serverInfo, player);
    client.run();
    ioService.run();
}