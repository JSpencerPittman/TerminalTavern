#ifndef TERMINALTAVERN_ROOM_H
#define TERMINALTAVERN_ROOM_H

#include <ncurses.h>
#include <vector>
#include <algorithm>

#include "util.h"
#include "Player.h"

class Room {
public:
    explicit Room(WINDOW *win);

    void addPlayer(Player p);
    void removePlayer(int pID);
    void movePlayer(int pID, Direction dir);
    void teleportPlayer(int pID, Coord2D pos);
    void teleportPlayer(int pID, int x, int y);

    void alignWithServer(PlayerMap& serverMap);
    void redrawRoom();

private:
    bool inBounds(Coord2D loc) const;

    WINDOW *win;
    int winWidth, winHeight;
    PlayerMap playerMap;
};



#endif //TERMINALTAVERN_ROOM_H
