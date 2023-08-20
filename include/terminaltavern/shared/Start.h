#ifndef TERMINALTAVERN_START_H
#define TERMINALTAVERN_START_H

#include <string>
#include <utility>
#include <iostream>
#include <ncurses.h>
#include <menu.h>
#include "util.h"
#include "Logo.h"
#include "Player.h"

// Keys
#define ENTER 10
#define BACKSPACE 127

// Default Server Settings
#define DEFAULT_HOSTNAME "terminaltavern.ddns.net"
#define DEFAULT_PORT 10777

// Logo
#define LOGO_MARGIN_UPPER 2 // How far logo is from the top

// Connect Panel
#define CONNECT_HEIGHT 4
#define CONNECT_WIDTH 20
#define CONNECT_MARGIN_LOWER 12

// Server Panel
#define SERVER_HEIGHT 7
#define SERVER_WIDTH 34
#define SERVER_MARGIN_UPPER (-1)
#define SERVER_CONTINUE_MARGIN_UPPER 2

class Start {
public:
    enum Connect {DEFAULT, CUSTOM};
    typedef std::pair<std::string, int> Server;

    Start();
    Connect connectWindow();
    ServerInfo serverWindow();
    PlayerPixelMap pixmapWindow();
    std::string usernameWindow();

    ServerInfo getServerInfo() const;
    PlayerPixelMap getPlayerPixelMap() const;
    std::string getUsername() const;

private:
    void drawLogo();

    WINDOW* win_;
    int width_, height_;

    ServerInfo serverInfo_;
    PlayerPixelMap pixmap_;
    std::string username_;
};


#endif //TERMINALTAVERN_START_H
