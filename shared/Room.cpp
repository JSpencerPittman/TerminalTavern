#include "Room.h"

Room::Room(WINDOW *win): win_(win), winWidth_(getmaxx(win)), winHeight_(getmaxy(win)) {}

void Room::start() {
    cbreak();
    noecho();
    curs_set(0); // Hide cursor
    nodelay(win_, TRUE);

    clear();
    drawBorder();
    refresh();
}

void Room::addPlayer(Player p) {
    playerMap_.addPlayer(p);
    drawCharacter(p.getPlayerID());
    refresh();
}

void Room::removePlayer(int pID) {
    eraseCharacter(pID);
    refresh();

    playerMap_.removePlayer(pID);
}

void Room::movePlayer(int pID, Direction dir) {
    Player player = playerMap_.getPlayer(pID);
    Coord2D currPos = player.getPos();
    player.move(dir);
    Coord2D futPos = player.getPos();

    if (!inBounds(futPos)) return;

    eraseCharacter(pID);
    playerMap_.movePlayer(pID, dir);
    drawCharacter(pID);
    refresh();
}

void Room::teleportPlayer(int pID, Coord2D pos) {
    if (!inBounds(pos)) return;

    eraseCharacter(pID);
    playerMap_.teleportPlayer(pID, pos);
    drawCharacter(pID);
    refresh();
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
    for (auto playerId: playerIDs)
        drawCharacter(playerId);

    drawBorder();
    refresh();
}

void Room::drawBorder() {
    // Border around window
    attron(A_BOLD);
    box(win_, 0, 0);
    attroff(A_BOLD);
}

void Room::drawCharacter(int pID) {
    Player p = playerMap_.getPlayer(pID);
    Coord2D pos = p.getPos();
    PlayerPixelMap::Unravel unravel = p.getPixelMap().unravelPixelMapAtLocation(pos);

    auto itr = unravel.begin();
    for(; itr != unravel.end(); ++itr)
        mvaddch(itr->first.y, itr->first.x, itr->second);
}

void Room::eraseCharacter(int pID) {
    Player p = playerMap_.getPlayer(pID);
    Coord2D pos = p.getPos();
    PlayerPixelMap::Unravel unravel = p.getPixelMap().unravelPixelMapAtLocation(pos);

    auto itr = unravel.begin();
    for(; itr != unravel.end(); ++itr)
        mvaddch(itr->first.y, itr->first.x, ' ');
}

bool Room::inBounds(Coord2D loc) const {
    if(loc.x <= 0 || loc.x >= this->winWidth_ - 1) return false;
    if(loc.y <= 0 || loc.y >= this->winHeight_ - 1) return false;
    return true;
}