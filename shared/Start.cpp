#include "Start.h"

Start::Start() {
    win_ = initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    curs_set(0);
    clear();
    refresh();

    getmaxyx(stdscr, height_, width_);

    Connect connect = connectWindow();
    clear();
    refresh();

    if(connect == CUSTOM)
        serverInfo_ = serverWindow();
    else
        serverInfo_ = ServerInfo{DEFAULT_HOSTNAME, DEFAULT_PORT};
    clear();
    refresh();

    pixmap_ = pixmapWindow();
    clear();
    refresh();

    username_ = usernameWindow();
    clear();
    refresh();
}

Start::Connect Start::connectWindow() {
    drawLogo();

    // Create the connection menu
    WINDOW *menuwin = newwin(CONNECT_HEIGHT,
                             CONNECT_WIDTH,
                             height_-CONNECT_MARGIN_LOWER,
                             (width_ - CONNECT_WIDTH)/2);
    box(menuwin, 0, 0);
    keypad(menuwin, TRUE);
    curs_set(0);

    std::vector<std::string> options{"Default", "Custom"};

    bool loop = true;
    int selection = 0, key;
    while(loop) {

        // Redraw the menu
        for(int i = 0; i < (int)options.size(); i++) {
            if(i == selection)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i+1, 6, options[i].c_str());
            if(i == selection)
                wattroff(menuwin, A_REVERSE);
        }
        wrefresh(menuwin);

        // Wait for the user's key
        key = wgetch(menuwin);
        switch(key) {
            case KEY_UP:
                selection = 0;
                break;
            case KEY_DOWN:
                selection = 1;
                break;
            case ENTER:
                loop = false;
                break;
            default:
                break;
        }
    }

    if(selection == 0) return Connect::DEFAULT;
    else return Connect::CUSTOM;
}

ServerInfo Start::getServerInfo() const {
    return serverInfo_;
}

PlayerPixelMap Start::getPlayerPixelMap() const {
    return pixmap_;
}

std::string Start::getUsername() const {
    return username_;
}

void Start::drawLogo() {
    Logo logo;

    int leftSpacing = (width_ - logo.cols())/2;
    for(int r = 0; r < logo.rows(); r++) {
        move(r+LOGO_MARGIN_UPPER, leftSpacing);
        printw(logo.getRow(r).c_str());
    }

    refresh();
}

ServerInfo Start::serverWindow() {
    drawLogo();

    // Draw server panel
    WINDOW *serverwin = newwin(SERVER_HEIGHT,
                               SERVER_WIDTH,
                               (height_/2)+SERVER_MARGIN_UPPER,
                               (width_ - SERVER_WIDTH)/2);
    box(serverwin, 0, 0);

    Coord2D host{2, 2};
    Coord2D port{2, 4};

    mvwprintw(serverwin, host.y, host.x+2, "Hostname:");
    mvwprintw(serverwin, port.y, port.x+2, "    Port:");

    int bottomServerPanel = (height_/2) + SERVER_MARGIN_UPPER + SERVER_HEIGHT;
    std::string cont = " Continue ";

    auto drawContinue = [this, bottomServerPanel, cont]() {
        mvwprintw(win_,
                  bottomServerPanel+SERVER_CONTINUE_MARGIN_UPPER,
                  (width_-(int)cont.size())/2,
                  cont.c_str());
    };

    drawContinue();
    wrefresh(serverwin);
    refresh();

    int selection = 0, key;
    bool loop = true;
    std::string hostname;
    std::string portnum;

    int textStart = 14;

    auto active = [serverwin](Coord2D pos)
        {mvwaddch(serverwin, pos.y, pos.x, '*');};
    auto inactive = [serverwin](Coord2D pos)
        {mvwaddch(serverwin, pos.y, pos.x, ' ');};

    while(loop) {
        switch (selection) {
            case 0:
                active(host);
                inactive(port);
                wrefresh(serverwin);
                drawContinue();
                refresh();
                break;
            case 1:
                inactive(host);
                active(port);
                wrefresh(serverwin);
                drawContinue();
                refresh();
                break;
            case 2:
                inactive(host);
                inactive(port);
                wrefresh(serverwin);
                wattron(win_, A_REVERSE);
                drawContinue();
                wattroff(win_, A_REVERSE);
                refresh();
                break;
            default:break;
        }
        wrefresh(serverwin);

        key = getch();
        switch(key) {
            case KEY_UP:
                selection = std::max(selection-1, 0);
                break;
            case KEY_DOWN:
                selection = std::min(selection+1, 2);
                break;
            case BACKSPACE:
                if(selection == 0) {
                    mvwaddch(serverwin, host.y, textStart-1+(hostname.size()), ' ');
                    hostname.pop_back();
                } else if(selection == 1) {
                    mvwaddch(serverwin, port.y, textStart - 1 + (portnum.size()), ' ');
                    portnum.pop_back();
                }
                break;
            case ENTER:
                loop = false;
                break;
            default:
                if(selection == 0) {
                    mvwaddch(serverwin, host.y, textStart + hostname.size(), key);
                    hostname.push_back((char) key);
                } else if(selection == 1) {
                    mvwaddch(serverwin, port.y, textStart + portnum.size(), key);
                    portnum.push_back((char) key);
                }
                break;
        }
        wrefresh(serverwin);
        refresh();
    }

    return {hostname, std::stoi(portnum)};
}

PlayerPixelMap Start::pixmapWindow() {
    drawLogo();

    // Draw pixel map panel
    WINDOW* pmap[3][3];

    int beginX = (width_/2) - 4;
    for(int r = 0; r < 3; r++) {
        for(int c = 0; c < 3; c++) {
            pmap[r][c] = newwin(3, 3, 9+r*3, beginX+c*3);
            box(pmap[r][c], 0, 0);
            mvwaddch(pmap[r][c], 1, 1, '0');
            wrefresh(pmap[r][c]);
        }
    }

    // Draw continue button
    std::string cont = "Continue";
    mvwprintw(win_, 20, (width_-(int)cont.size())/2, cont.c_str());
    refresh();

    Coord2D user{0, 0};
    int key;
    bool loop = true;

    while(loop) {
        // Redraw pixel map
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (r == user.y && c == user.x)
                    wattron(pmap[r][c], A_REVERSE);
                char ch = (char) (mvwinch(pmap[r][c], 1, 1) & A_CHARTEXT);
                mvwaddch(pmap[r][c], 1, 1, ch);
                if (r == user.y && c == user.x)
                    wattroff(pmap[r][c], A_REVERSE);
                wrefresh(pmap[r][c]);
            }
        }

        // Redraw continue button
        if (user.y == 3) wattron(win_, A_REVERSE);
        mvwprintw(win_, 20, (width_ - (int) cont.size()) / 2, cont.c_str());
        if (user.y == 3) wattroff(win_, A_REVERSE);
        refresh();

        // Handle next key
        key = getch();
        switch (key) {
            case KEY_UP:
                user.y = std::max(0, user.y - 1);
                break;
            case KEY_DOWN:
                user.y = std::min(3, user.y + 1);
                break;
            case KEY_LEFT:
                user.x = std::max(0, user.x - 1);
                break;
            case KEY_RIGHT:
                user.x = std::min(2, user.x + 1);
                break;
            case ENTER:
                if (user.y == 3) loop = false;
                break;
            case BACKSPACE:
                wattron(pmap[user.y][user.x], A_REVERSE);
                mvwaddch(pmap[user.y][user.x], 1, 1, ' ');
                wattroff(pmap[user.y][user.x], A_REVERSE);
                break;
            default:
                wattron(pmap[user.y][user.x], A_REVERSE);
                mvwaddch(pmap[user.y][user.x], 1, 1, key);
                wattroff(pmap[user.y][user.x], A_REVERSE);
                break;
        }
    }

    PlayerPixelMap::PixelMap pixmap;
    for(int r = 0; r < 3; r++) {
        std::vector<char> row;
        for(int c = 0; c < 3; c++) {
            char ch = (char) (mvwinch(pmap[r][c], 1, 1) & A_CHARTEXT);
            row.push_back(ch);
        }
        pixmap.push_back(row);
    }

    return PlayerPixelMap(pixmap);
}

std::string Start::usernameWindow() {
    drawLogo();

    // Draw username area
    std::string usernameLabel = "Username";
    mvwprintw(win_, height_/2, (width_- (int)usernameLabel.size())/2,
              usernameLabel.c_str());

    // Draw continue message
    std::string cont = "Continue";
    auto drawContinue = [this, cont]() {
        mvwprintw(win_, height_-5, (width_-(int)cont.size())/2, cont.c_str());
    };

    drawContinue();
    refresh();

    bool loop = true;
    int selection = 0, key;
    std::string input;

    auto writeUsername = [this](std::string input) {
        if(input.empty()) return;
        std::string line(input.size(), '-');
        mvwprintw(win_, (height_ / 2) + 2,
                  (width_ - (int) line.size()) / 2, line.c_str());
        mvwprintw(win_, (height_ / 2) + 4,
                  (width_ - (int) line.size()) / 2, line.c_str());

        input.insert(input.begin(), '|');
        input.insert(input.end(), '|');
        mvwprintw(win_, (height_ / 2) + 3,
                  (width_ - (int) input.size()) / 2, input.c_str());
    };
    auto clearUsername = [this](const std::string& input) {
        std::string filler(input.size(), ' ');
        mvwprintw(win_, (height_ / 2) + 2,
                  (width_ - (int) filler.size()) / 2, filler.c_str());
        mvwprintw(win_, (height_ / 2) + 4,
                  (width_ - (int) filler.size()) / 2, filler.c_str());
        filler.push_back(' ');
        filler.push_back(' ');
        mvwprintw(win_, (height_ / 2) + 3,
                  (width_ - (int) filler.size()) / 2, filler.c_str());
    };

    while(loop) {
        key = getch();

        switch(key) {
            case KEY_UP:
                selection = 0;
                break;
            case KEY_DOWN:
                selection = 1;
                break;
            case ENTER:
                if(selection == 1) loop = false;
                break;
            case BACKSPACE:
                if(input.empty()) break;
                clearUsername(input);
                input.pop_back();
                writeUsername(input);
                break;
            default:
                clearUsername(input);
                input.push_back(key);
                writeUsername(input);
                break;
        }

        if(selection) wattron(win_, A_REVERSE);
        drawContinue();
        if(selection) wattroff(win_, A_REVERSE);

        refresh();
    }

    return input;
}