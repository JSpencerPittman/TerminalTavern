#include "Room.h"

Room::Room(WINDOW *win): win_(win) {
    this->winWidth_ = getmaxx(win);
    this->winHeight_ = getmaxy(win);

    cbreak();
    noecho();
    curs_set(0); // Hide cursor
    nodelay(win, TRUE);

    clear();
    drawBorder();
    refresh();
}

void Room::addPlayer(Player p) {
    mvaddch(p.getPos().y, p.getPos().x, p.getPlayerID());
    refresh();

    playerMap_.addPlayer(p);
}

void Room::removePlayer(int pID) {
    Coord2D pos = playerMap_.getPlayerPos(pID);
    mvaddch(pos.y, pos.x, ' ');
    refresh();

    playerMap_.removePlayer(pID);
}

void Room::movePlayer(int pID, Direction dir) {
    Player player = playerMap_.getPlayer(pID);
    Coord2D currPos = player.getPos();
    player.move(dir);
    Coord2D futPos = player.getPos();

    if (!inBounds(futPos)) return;

    mvaddch(currPos.y, currPos.x, ' ');
    mvaddch(futPos.y, futPos.x, '0');
    refresh();

    playerMap_.movePlayer(pID, dir);
}

void Room::teleportPlayer(int pID, Coord2D pos) {
    if (!inBounds(pos)) return;

    Coord2D currPos = playerMap_.getPlayerPos(pID);
    mvaddch(currPos.y, currPos.x, ' ');
    mvaddch(pos.y, pos.x, '0');
    refresh();

    playerMap_.teleportPlayer(pID, pos);
}

void Room::teleportPlayer(int pID, int x, int y) {
    teleportPlayer(pID, {x,y});
}

void Room::alignWithServer(PlayerMap &serverMap) {
    std::vector<int> serverIDS = serverMap.getPlayerIDS();
    std::vector<int> clientIDS = playerMap_.getPlayerIDS();

    int si = 0, ci = 0, playerID;
    while(si < (int)serverIDS.size() && ci < (int)clientIDS.size()) {
        if (serverIDS[si] == clientIDS[ci]) {
            // Check for an updated position
            playerID = serverIDS[si];

            Coord2D servPos = serverMap.getPlayerPos(playerID);
            Coord2D clientPos = playerMap_.getPlayerPos(playerID);

            if (servPos.x != clientPos.x || servPos.y != clientPos.y)
                teleportPlayer(playerID, servPos);

            si++;
            ci++;
        } else if(serverIDS[si] < clientIDS[ci]) {
            // Add new player
            playerID = serverIDS[si];
            addPlayer(serverMap.getPlayer(playerID));
            si++;
        } else {
            // Remove player
            playerID = clientIDS[ci];
            removePlayer(playerID);
            ci++;
        }
    }

    // Add remaining server IDs
    while(si != (int)serverIDS.size()) {
        playerID = serverIDS[si];
        addPlayer(serverMap.getPlayer(playerID));
        si++;
    }

    // Remove remaining client IDs
    while(ci != (int)clientIDS.size()) {
        playerID = clientIDS[si];
        removePlayer(playerID);
        ci++;
    }
}

void Room::redrawRoom() {
    clear();

    std::vector<int> playerIDs = playerMap_.getPlayerIDS();
    for (auto playerId: playerIDs) {
        Coord2D pos = playerMap_.getPlayerPos(playerId);
        mvaddch(pos.y, pos.x, '0');
    }

    drawBorder();
    refresh();
}

void Room::drawBorder() {
    // Border around window
    attron(A_BOLD);
    box(win_, 0, 0);
    attroff(A_BOLD);
}

bool Room::inBounds(Coord2D loc) const {
    if(loc.x <= 0 || loc.x >= this->winWidth_ - 1) return false;
    if(loc.y <= 0 || loc.y >= this->winHeight_ - 1) return false;
    return true;
}